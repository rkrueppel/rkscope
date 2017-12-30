#include "StdAfx.h"
#include "FPGAResonanceScanner_NI5771.h"
#include "parameters/Inputs.h"

namespace scope {

	FPGAResonanceScannerNI5771::FPGAResonanceScannerNI5771()
		: FPGAIO5771((uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_IndicatorBool_UserCommandIdle
		, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_IndicatorBool_PLLLocked
		, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_IndicatorBool_Configured
		, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_IndicatorBool_UserError
		, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_IndicatorU8_UserCommandStatus
		, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_ControlU8_UserCommand
		, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_ControlU16_UserData0
		, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_ControlU8_UserData1
		, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_ControlBool_UserCommandCommit) {
	
		status = NiFpga_Initialize();

		char* const Bitfile = "devices\\fpga\\" NiFpga_AnalogIntegrator_NI5771_Resonance_Bitfile;

		// Load but do not run yet
		status = NiFpga_Open(Bitfile, NiFpga_AnalogIntegrator_NI5771_Resonance_Signature, "RIO0", 0, &session);
		DBOUT(L"FPGAResonanceScannerNI5771: FPGA Session opened");

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
		fifos[0] = NiFpga_AnalogIntegrator_NI5771_Resonance_TargetToHostFifoU32_ToHostCh1FIFO;
		fifos[1] = NiFpga_AnalogIntegrator_NI5771_Resonance_TargetToHostFifoU32_ToHostCh2FIFO;
	}

	FPGAResonanceScannerNI5771::~FPGAResonanceScannerNI5771() {
		status = NiFpga_Close(session, 0);
		status = NiFpga_Finalize();
		DBOUT(L"FPGAResonanceScannerNI5771::~FPGAResonanceScannerNI5771 session closed");
	}

	void FPGAResonanceScannerNI5771::Initialize(parameters::InputsFPGA* _parameters) {
		if ( !initialized ) {
			parameters = dynamic_cast<parameters::InputsFPGAAnalogIntegrator*>(_parameters);

			SetClockSource(session, 0);

			uint32_t chunksize = 1000000;

			status = NiFpga_ConfigureFifo(session, fifos[0], 5*chunksize);
			status = NiFpga_ConfigureFifo(session, fifos[1], 5*chunksize);

			FPGAInterface::Initialize(_parameters);
		}
	}

	double FPGAResonanceScannerNI5771::SetPixeltime(const uint32_t& _area, const double& _pixeltime) {
		// sampling rate of the NI 5771 is 1.5GHz
		uint16_t samplesperpixel = round2ui16(_pixeltime * 1E-6 * 1.5E9);
		status = NiFpga_WriteU16(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_ControlU32_Samplesperpixel, samplesperpixel);

		DBOUT(L"FPGAResonanceScannerNI5771::SetPixeltime samples per pixel" << samplesperpixel);
		// Since the AnalogIntegrator VI is working on arrays of 8 samples clockcycles has to be a multiple of 8
		if ( samplesperpixel%8 != 0 ) {
			samplesperpixel -= (samplesperpixel%8);
			DBOUT(L"FPGAResonanceScannerNI5771::SetPixeltime Coerced samples per per pixel" << samplesperpixel);
		}

		return static_cast<double>(samplesperpixel)*1E6/1.5E9;
	}

	double FPGAResonanceScannerNI5771::SetLinetime(const uint32_t& _area, const double& _linetime) {
		// sampling rate of the NI 5771 is 1.5GHz
		uint16_t samplesperline = round2ui16(_linetime * 1E-6 * 1.5E9);
		status = NiFpga_WriteU16(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_ControlU32_Samplesperline, samplesperline);

		DBOUT(L"FPGAResonanceScannerNI5771::SetPixeltime samples per line" << samplesperline);
		// Since the AnalogIntegrator VI is working on arrays of 8 samples clockcycles has to be a multiple of 8
		if ( samplesperline%8 != 0 ) {
			samplesperline -= (samplesperline%8);
			DBOUT(L"FPGAResonanceScannerNI5771::SetPixeltime Coerced samples per per pixel" << samplesperline);
		}

		return static_cast<double>(samplesperline)*1E6/1.5E9;
	}

	void FPGAResonanceScannerNI5771::SetTriggering(const bool& _waitfortrigger) {
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_ControlBool_Waitfortrigger, _waitfortrigger);
	}

	void FPGAResonanceScannerNI5771::SetContinuousAcquisition(const bool& _cont) {
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_ControlBool_Acquirecontinuously, _cont);
	}

	void FPGAResonanceScannerNI5771::SetRequestedPixels(const uint32_t& _area, const uint32_t& _reqpixels) {
		status = NiFpga_WriteU32(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_ControlU32_Requestedpixels, _reqpixels);
	} 

	void FPGAResonanceScannerNI5771::StartAcquisition() {
		ClearFIFOs();
		SetChannelProps();
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_ControlBool_Acquire, true);
	}

	int32_t FPGAResonanceScannerNI5771::ReadPixels(DaqMultiChunkResonance<2, 1, uint16_t>& _chunk, const double& _timeout, bool& _timedout) {
		size_t remaining = 0;
	
		// should be always true (because it is handled inside DaqChunk), but better to check anyway...
		assert(_chunk.data.size() >= _chunk.PerChannel() * 2);

		NiFpga_Status stat = NiFpga_Status_Success;

		std::vector<uint32_t> u32data(_chunk.PerChannel());
		std::vector<int32_t> bitshift(2);
		bitshift[0] = parameters->BitshiftCh1();
		bitshift[1] = parameters->BitshiftCh2();

		// Read each channels fifo
		for ( uint32_t c = 0 ; c < 2 ; c++ ) {
			stat = NiFpga_ReadFifoU32(session
					, fifos[c]
					, u32data.data()
					, _chunk.PerChannel()
					, static_cast<uint32_t>(_timeout * 1000)				// FPGA C API takes timeout in milliseconds, to be consistent with DAQmx we have _timeout in seconds
					, &remaining);
	
			_timedout = (stat == NiFpga_Status_FifoTimeout);

			// avoid throwing exception on time out (since FpgaStatus status could throw on all errors)
			// return immediately, makes no sense to also wait for the other channels, since anyway something went seriously wrong
			if ( _timedout )
				return -1;

			// this could throw on error (if we would use FPGAStatus instead of FPGAStatusSafe)
			status = stat;

			// isolate pixel uint16 from uint32
			auto itdata = std::begin(_chunk.data) + c * _chunk.PerChannel();
			for(auto u32 : u32data) {
				*itdata = u32 & 0x0000ffff;
				itdata++;
			}
		}

		// isolate sync bool from uint32
		auto itsync = std::begin(_chunk.resSync);
		for(auto u32 : u32data) {
			*itsync = ((u32 >> 31) != 0);
			itsync++;
		}

		if ( status.Success() )
			return _chunk.PerChannel();

		return -1;
	}

	void FPGAResonanceScannerNI5771::StopAcquisition() {
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_ControlBool_Acquire, false);
	}

	void FPGAResonanceScannerNI5771::SetChannelProps() {
		// We want to right-shift bits thus *-1
		//status = NiFpga_WriteI8(session, NiFpga_AnalogIntegrator_NI5771_Resonance_ControlI8_BitshiftCh1, -1 * parameters->BitshiftCh1());
		//status = NiFpga_WriteI8(session, NiFpga_AnalogIntegrator_NI5771_Resonance_ControlI8_BitshiftCh2, -1 * parameters->BitshiftCh2());
		// We subtract baseline from cumulated 8 samples, thus 8*
		status = NiFpga_WriteU8(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_ControlU8_Baselinex8Ch1, parameters->BaselineCh1());
		status = NiFpga_WriteU8(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_ControlU8_Baselinex8Ch2, parameters->BaselineCh2());

		DBOUT(L"FPGAResonanceScannerNI5771::SetChannelProps " << parameters->BitshiftCh1() << L" " << parameters->BaselineCh1());
	}

	void FPGAResonanceScannerNI5771::CheckFPGADiagnosis() {
		NiFpga_Bool b;
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_IndicatorBool_ToHostCh1FIFOOverflow, &b);
		parameters->diagnosis.ToHostOverflowCh1 = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_IndicatorBool_ToHostCh2FIFOOverflow, &b);
		parameters->diagnosis.ToHostOverflowCh2 = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_IndicatorBool_InterloopFIFOOverflow, &b);
		parameters->diagnosis.InterloopOverflow = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_IndicatorBool_InterloopFIFOTimeout, &b);
		parameters->diagnosis.InterloopTimeout = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_IndicatorBool_Acquiring, &b);
		parameters->diagnosis.Acquiring = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_IndicatorBool_IOModuleAIOverRange, &b);
		parameters->diagnosis.AIOverRange = (b!=0);
	}

	void FPGAResonanceScannerNI5771::ClearFIFOs() {
		// Clear interloop fifo
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_ControlBool_ClearInterloopFIFO, true);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_AnalogIntegrator_NI5771_Resonance_ControlBool_ClearInterloopFIFO, false);

		// Stop to host FIFOs (clears them)
		for ( auto f : fifos )
			status = NiFpga_StopFifo(session, f);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		// Restart to host FIFOs
		for ( auto f : fifos )
			status = NiFpga_StartFifo(session, f);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

}