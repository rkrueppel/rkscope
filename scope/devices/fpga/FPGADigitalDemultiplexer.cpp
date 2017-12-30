#include "StdAfx.h"
#include "FPGADigitalDemultiplexer.h"
#include "parameters/Inputs.h"

namespace scope {

	FPGADigitalDemultiplexer::FPGADigitalDemultiplexer()
		: FPGAIO6587((uint32_t)NiFpga_DigitalDemultiplexerV3_IndicatorBool_Onboard_Clock_Ready
		, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlU16_Onboard_Clock_Write_Data
		, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlBool_Onboard_Clock_Write
		, (uint32_t)NiFpga_DigitalDemultiplexerV3_IndicatorBool_Xpoint_Switch_Ready
		, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlU8_ClockSource
		, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlBool_Xpoint_Switch_Write
		, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlBool_Commit
		, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlBool_Acq_Reset)
		, samplingrate(1E9) {
		
		status = NiFpga_Initialize();

		char* const Bitfile = "devices\\fpga\\" NiFpga_DigitalDemultiplexerV3_Bitfile;

		// Load but do not run yet
		status = NiFpga_Open(Bitfile, NiFpga_DigitalDemultiplexerV3_Signature, "RIO0", 0, &session);
		DBOUT(L"FPGADemultiplexer: FPGA Session opened\n");

		// Reset the non-running FPGA to clear any bad stuff (e.g. after a program crash with open FPGA before)
		status = NiFpga_Reset(session);

		// YOU HAVE TO WAIT THAT LONG!!!
		// Otherwise you will occasionally (!!!) into strange problems (communication with fpga error -61046)
		// This took me very very long to figure out...
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		// Run after reset
		status = NiFpga_Run(session, 0);
		
		CheckIOModule(session);

		fifos[0] = NiFpga_DigitalDemultiplexerV3_TargetToHostFifoU16_ToHostArea1Ch1FIFO;
		fifos[1] = NiFpga_DigitalDemultiplexerV3_TargetToHostFifoU16_ToHostArea1Ch2FIFO;
		fifos[2] = NiFpga_DigitalDemultiplexerV3_TargetToHostFifoU16_ToHostArea2Ch1FIFO;
		fifos[3] = NiFpga_DigitalDemultiplexerV3_TargetToHostFifoU16_ToHostArea2Ch2FIFO;
		reqpixels[0] = NiFpga_DigitalDemultiplexerV3_ControlU32_RequestedpixelsA1;
		reqpixels[1] = NiFpga_DigitalDemultiplexerV3_ControlU32_RequestedpixelsA2;
		smplsperpixel[0] = NiFpga_DigitalDemultiplexerV3_ControlU16_SamplesperpixelA1;
		smplsperpixel[1] = NiFpga_DigitalDemultiplexerV3_ControlU16_SamplesperpixelA2;
		smplsperpixelacq[0] = NiFpga_DigitalDemultiplexerV3_ControlU16_SamplesperpixelAcqA1;
		smplsperpixelacq[1] = NiFpga_DigitalDemultiplexerV3_ControlU16_SamplesperpixelAcqA2;
	}

	FPGADigitalDemultiplexer::~FPGADigitalDemultiplexer(void) {
		status = NiFpga_Close(session, 0);
		status = NiFpga_Finalize();
		DBOUT(L"FPGADemultiplexer::~FPGADemultiplexer session closed");
	}

	void FPGADigitalDemultiplexer::Initialize(parameters::InputsFPGA* _parameters) {
		if ( !initialized ) {
			parameters = dynamic_cast<parameters::InputsFPGADigitalDemultiplexer*>(_parameters);

			assert( (parameters->samplingrate() > 100E6) && (parameters->samplingrate() < 1.6E9));

			samplingrate = parameters->samplingrate();
			// Clock frequency is 0.5x sampling rate, due to double data rate
			WriteOnboardClockFrequency(session, samplingrate / 2);
			// YOU HAVE TO WAIT THAT LONG!!!
			// Otherwise you will occasionally (!!!) into strange problems (communication with fpga error -61046)
			// This took me very very long to figure out...
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			SetClockSource(session);

			InitializeAcquisition(session);

			// Set counting mode
			status = NiFpga_WriteBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlBool_Countmode, parameters->countmode());

			FPGAInterface::Initialize(_parameters);
		}
	}

	double FPGADigitalDemultiplexer::SetPixeltime(const uint32_t& _area, const double& _pixeltime) {
		// sampling rate is twice the IO modules frequency (because of dual data rate transfer)
		uint16_t clockcycles = round2ui16(_pixeltime * 1E-6 * samplingrate);
		DBOUT(L"FPGADemultiplexer::SetPixeltime Clockcycles per pixel" << clockcycles);
		// Since the Demultiplexer VI is working on arrays of 60 samples clockcycles has to be a multiple of 60
		if ( clockcycles%60 != 0 ) {
			clockcycles -= (clockcycles%60);
			DBOUT(L"FPGADemultiplexer::SetPixeltime Coerced clockcycles per pixel" << clockcycles);
		}
		status = NiFpga_WriteU16(session, smplsperpixel[_area], clockcycles);
		status = NiFpga_WriteU16(session, smplsperpixelacq[_area], clockcycles);
		pixeltimes[_area] = static_cast<double>(clockcycles)*1E6/samplingrate;
		return pixeltimes[_area];
	}

	double FPGADigitalDemultiplexer::SetLinetime(const uint32_t& _area, const double& _linetime) {
		// Not supported by FPGA VI
		return _linetime;
	}

	void FPGADigitalDemultiplexer::SetTriggering(const bool& _waitfortrigger) {
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlBool_Waitfortrigger, _waitfortrigger);
	}

	void FPGADigitalDemultiplexer::SetContinuousAcquisition(const bool& _cont) {
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlBool_Acquirecontinuously, _cont);
	}

	void FPGADigitalDemultiplexer::SetRequestedPixels(const uint32_t& _area, const uint32_t& _reqpixels) {
		status = NiFpga_WriteU32(session, reqpixels[_area], _reqpixels);
	}

	void FPGADigitalDemultiplexer::StartAcquisition() {
		NiFpga_Bool alreadyrunning = false;
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlBool_Acquire, &alreadyrunning);
		if ( !alreadyrunning ) {
			ClearFIFOs();						// Only clear them if not alreay running (e.g. by a previous StartAcquisition from another area!!) Leads to nasty bugs!
			status = NiFpga_WriteBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlBool_Acquire, true);
		}
	}

	void FPGADigitalDemultiplexer::StopAcquisition() {
		DBOUT(L"FPGADigitalDemultiplexer::StopAcquisition");
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlBool_Acquire, false);
	}

	void FPGADigitalDemultiplexer::ResetAcquisition() {
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlBool_Acq_Reset, false);
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlBool_Acq_Reset, true);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlBool_Acq_Reset, false);
	}

	int32_t FPGADigitalDemultiplexer::ReadPixels(DaqMultiChunk<2, 2, uint16_t>& _chunk, const double& _timeout, bool& _timedout) {
		size_t remaining = 0;

		// we need enough space
		assert(_chunk.data.size() >= 2 * 2 * _chunk.PerChannel() );

		NiFpga_Status stat = NiFpga_Status_Success;

		for (uint32_t a = 0; a < 2; a++) {
			for (uint32_t c = 0; c < 2; c++) {
				stat = NiFpga_ReadFifoU16(session
					, fifos[a * 2 + c]										// select correct fifo
					, &(*(_chunk.GetDataStart(a) + c * _chunk.PerChannel()))// offset start in vector for second channel pixels
					, _chunk.PerChannel()
					, static_cast<uint32_t>(_timeout * 1000)				// FPGA C API takes timeout in milliseconds, to be consistent with DAQmx we have _timeout in seconds
					, &remaining);
				_timedout = (stat == NiFpga_Status_FifoTimeout);

				// avoid throwing exception on time out (since FpgaStatus status could throw on all errors)
				if (_timedout)
					return -1;
				status = stat;
			}
		}
		
		if ( status.Success() )
			return _chunk.PerChannel();
		return -1;
	}

	void FPGADigitalDemultiplexer::CheckFPGADiagnosis() {
		NiFpga_Bool b(0);
		uint16_t ui = 0;
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_IndicatorBool_ToHostFIFOOverflowA1Ch1, &b);
		parameters->diagnosis.ToHostOverflowA1Ch1 = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_IndicatorBool_ToHostFIFOOverflowA1Ch2, &b);
		parameters->diagnosis.ToHostOverflowA1Ch2 = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_IndicatorBool_ToHostFIFOOverflowA2Ch1, &b);
		parameters->diagnosis.ToHostOverflowA2Ch1 = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_IndicatorBool_ToHostFIFOOverflowA2Ch2, &b);
		parameters->diagnosis.ToHostOverflowA2Ch2 = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_IndicatorBool_InterloopFIFOoverflow, &b);
		parameters->diagnosis.InterloopOverflow = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_IndicatorBool_InterloopFIFOtimeout, &b);
		parameters->diagnosis.InterloopTimeout = (b!=0);
		status = NiFpga_ReadU16(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_IndicatorU16_LaserpulsesperpixelA1, &ui);
		DBOUT(L"Laserpulsecount " << ui);
		parameters->diagnosis.MeasuredLaserFreq = static_cast<double>(ui) / (pixeltimes[0] * 1E-6);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_IndicatorBool_Acquiring, &b);
		parameters->diagnosis.Acquiring = (b!=0);
	}

	void FPGADigitalDemultiplexer::ClearFIFOs() {
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlBool_ClearInterloopFIFO, 1);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlBool_ClearInterloopFIFO, 0);

		// Stop FIFOs (clears them)
		for ( auto f : fifos )
			status = NiFpga_StopFifo(session, f);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		// Restart FIFOs
		for ( auto f : fifos )
			status = NiFpga_StartFifo(session, f);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	void FPGADigitalDemultiplexer::SetCountMode(const bool& _mode) {
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_DigitalDemultiplexerV3_ControlBool_Countmode, _mode);
	}

}