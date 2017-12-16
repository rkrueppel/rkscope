#include "StdAfx.h"
#include "FPGAPhotonCounter.h"
#include "parameters/Inputs.h"
#include "helpers/DaqMultiChunk.h"

namespace scope {

	FPGAPhotonCounter::FPGAPhotonCounter()
		: FPGAIO6587((uint32_t)NiFpga_PhotonCounterV2_IndicatorBool_Onboard_Clock_Ready
		, (uint32_t)NiFpga_PhotonCounterV2_ControlU16_Onboard_Clock_Write_Data
		, (uint32_t)NiFpga_PhotonCounterV2_ControlBool_Onboard_Clock_Write
		, (uint32_t)NiFpga_PhotonCounterV2_IndicatorBool_Xpoint_Switch_Ready
		, (uint32_t)NiFpga_PhotonCounterV2_ControlU8_ClockSource
		, (uint32_t)NiFpga_PhotonCounterV2_ControlBool_Xpoint_Switch_Write
		, (uint32_t)NiFpga_PhotonCounterV2_ControlBool_Commit
		, (uint32_t)NiFpga_PhotonCounterV2_ControlBool_Acq_Reset) {
		status = NiFpga_Initialize();
		char* const Bitfile = "devices\\fpga\\" NiFpga_PhotonCounterV2_Bitfile;

		// Load but do not run yet
		status = NiFpga_Open(Bitfile, NiFpga_PhotonCounterV2_Signature, "RIO0", NiFpga_OpenAttribute_NoRun, &session);
		DBOUT(L"FPGAPhotonCounter: FPGA Session opened");

		// Reset the non-running FPGA to clear any bad stuff (e.g. after a program crash with open FPGA before)
		status = NiFpga_Reset(session);

		// YOU HAVE TO WAIT THAT LONG!!!
		// Otherwise you will occasionally (!!!) into strange problems (communication with fpga error -61046)
		// This took me very very long to figure out...
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		// Run after reset
		status = NiFpga_Run(session, 0);
	
		CheckIOModule(session);

		// Set the fifo constants
		fifos[0] = NiFpga_PhotonCounterV2_TargetToHostFifoU16_ToHostCh1FIFO;
		fifos[1] = NiFpga_PhotonCounterV2_TargetToHostFifoU16_ToHostCh2FIFO;
	}

	FPGAPhotonCounter::~FPGAPhotonCounter(void) {
		status = NiFpga_Close(session, 0);
		status = NiFpga_Finalize();
		DBOUT(L"FPGAPhotonCounter::~FPGAPhotonCounter session closed");
	}

	void FPGAPhotonCounter::Initialize(parameters::InputsFPGA* _parameters) {
		if ( !initialized ) {
			parameters = dynamic_cast<parameters::InputsFPGAPhotonCounter*>(_parameters);

			assert( (parameters->samplingrate() > 100E6) && (parameters->samplingrate() < 1.6E9));
			samplingrate = parameters->samplingrate();

			WriteOnboardClockFrequency(session, samplingrate / 2);
			// YOU HAVE TO WAIT THAT LONG!!!
			// Otherwise you will occasionally (!!!) into strange problems (communication with fpga error -61046)
			// This took me very very long to figure out...
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			SetClockSource(session);

			InitializeAcquisition(session);

			SetCountMode(parameters->countmode());

			FPGAInterface::Initialize(_parameters);
		}
	}

	double FPGAPhotonCounter::SetPixeltime(const uint32_t& _area, const double& _pixeltime) {
		// sampling rate is twice the IO modules frequency (because of dual data rate transfer)
		uint16_t samplesperpixel = round2ui16(_pixeltime * 1E-6 * samplingrate);
		status = NiFpga_WriteU16(session, (uint32_t)NiFpga_PhotonCounterV2_ControlU16_Samplesperpixel, samplesperpixel);
		status = NiFpga_WriteU16(session, (uint32_t)NiFpga_PhotonCounterV2_ControlU16_SamplesperpixelAcq, samplesperpixel);
		return static_cast<double>(samplesperpixel)*1E6/samplingrate;
	}

	double FPGAPhotonCounter::SetLinetime(const uint32_t& _area, const double& _linetime) {
		// sampling rate is twice the IO modules frequency (because of dual data rate transfer)
		uint16_t samplesperline = round2ui16(_linetime * 1E-6 * samplingrate);
		status = NiFpga_WriteU16(session, (uint32_t)NiFpga_PhotonCounterV2_ControlU32_Samplesperline, samplesperline);
		return static_cast<double>(samplesperline*1E6/samplingrate);
	}

	void FPGAPhotonCounter::SetTriggering(const bool& _waitfortrigger) {
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_PhotonCounterV2_ControlBool_Waitfortrigger, _waitfortrigger);
	}

	void FPGAPhotonCounter::SetContinuousAcquisition(const bool& _cont) {
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_PhotonCounterV2_ControlBool_Acquirecontinuously, _cont);
	}

	void FPGAPhotonCounter::SetRequestedPixels(const uint32_t& _area, const uint32_t& _reqpixels) {
		status = NiFpga_WriteU32(session, (uint32_t)NiFpga_PhotonCounterV2_ControlU32_Requestedpixels, _reqpixels);
	}

	void FPGAPhotonCounter::StartAcquisition() {
		if ( !initialized )
			throw ScopeException("Initialize FPGAPhotonCounter before StartAcquisition!");
		ClearFIFOs();
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_PhotonCounterV2_ControlBool_Acquire, true);
	}

	int32_t FPGAPhotonCounter::ReadPixels(const uint32_t& _area, config::DaqMultiChunkType& _chunk, const double& _timeout, bool& _timedout) {
		size_t remaining = 0;

		assert(_chunk.NChannels() <= 2 );										// only two channels supported in FPGA vi

		NiFpga_Status stat = NiFpga_Status_Success;
	
		for ( uint32_t c = 0 ; c < _chunk.NChannels() ; c++ ) {
			stat = NiFpga_ReadFifoU16(session
					, fifos[c]
					, &_chunk.data[c * _chunk.PerChannel()]					// offset start in vector for second channel pixels
					, _chunk.PerChannel()
					, static_cast<uint32_t>(_timeout * 1000)				// FPGA C API takes timeout in milliseconds, to be consistent with DAQmx we have _timeout in seconds
					, &remaining);
	
			_timedout = (stat == NiFpga_Status_FifoTimeout);
			if ( _timedout )
				return -1;													// avoid throwing exception on time out (since FpgaStatus status could throw on all errors)
			status = stat;
		}

		if ( status.Success() )
			return _chunk.PerChannel();

		return -1;
	}

	void FPGAPhotonCounter::StopAcquisition() {
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_PhotonCounterV2_ControlBool_Acquire, false);
	}

	void FPGAPhotonCounter::CheckFPGADiagnosis() {
		NiFpga_Bool b;
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_PhotonCounterV2_IndicatorBool_ToHostFIFOOverflowCh1, &b);
		parameters->diagnosis.ToHostOverflowCh1 = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_PhotonCounterV2_IndicatorBool_ToHostFIFOOverflowCh2, &b);
		parameters->diagnosis.ToHostOverflowCh2 = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_PhotonCounterV2_IndicatorBool_InterloopFIFOoverflow, &b);
		parameters->diagnosis.InterloopOverflow = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_PhotonCounterV2_IndicatorBool_InterloopFIFOtimeout, &b);
		parameters->diagnosis.InterloopTimeout = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_PhotonCounterV2_IndicatorBool_Acquiring, &b);
		parameters->diagnosis.Acquiring = (b!=0);
	}

	void FPGAPhotonCounter::ClearFIFOs() {
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_PhotonCounterV2_ControlBool_ClearInterloopFIFOs, 1);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_PhotonCounterV2_ControlBool_ClearInterloopFIFOs, 0);
		// Stop FIFOs (clears them)
		for ( auto f : fifos )
			status = NiFpga_StopFifo(session, f);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		// Restart FIFOs
		for ( auto f : fifos )
			status = NiFpga_StartFifo(session, f);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	void FPGAPhotonCounter::SetCountMode(const bool& _mode) {
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_PhotonCounterV2_ControlBool_Countingmode, _mode);
	}

}
