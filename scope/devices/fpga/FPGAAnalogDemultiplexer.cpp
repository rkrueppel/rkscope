#include "StdAfx.h"
#include "FPGAAnalogDemultiplexer.h"
#include "parameters/Inputs.h"

namespace scope {

	FPGAAnalogDemultiplexer::FPGAAnalogDemultiplexer()
		: FPGAIO5771(NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_UserCommandIdle
		, NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_PLLLocked
		, NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_Configured
		, NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_UserError
		, NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorU8_UserCommandStatus
		, NiFpga_AnalogDemultiplexerV2_NI5771_ControlU8_UserCommand
		, NiFpga_AnalogDemultiplexerV2_NI5771_ControlU16_UserData0
		, NiFpga_AnalogDemultiplexerV2_NI5771_ControlU8_UserData1
		, NiFpga_AnalogDemultiplexerV2_NI5771_ControlBool_UserCommandCommit) {
	
		status = NiFpga_Initialize();

		char* const Bitfile = "devices\\fpga\\" NiFpga_AnalogDemultiplexerV2_NI5771_Bitfile;

		// Load but do not run yet
		status = NiFpga_Open(Bitfile, NiFpga_AnalogDemultiplexerV2_NI5771_Signature, "RIO0", 0, &session);
		DBOUT(L"FPGADemultiplexer: FPGA Session opened");

		// Reset the non-running FPGA to clear any bad stuff (e.g. after a program crash with open FPGA before)
		status = NiFpga_Reset(session);

		// YOU HAVE TO WAIT THAT LONG!!!
		// Otherwise you will occasionally (!!!) into strange problems (communication with fpga error -61046)
		// This took me very very long to figure out...
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		// Run after reset
		status = NiFpga_Run(session, 0);
		
		CheckIOModule(session);

		fifos[0] = NiFpga_AnalogDemultiplexerV2_NI5771_TargetToHostFifoU64_ToHostA1FIFO;
		fifos[1] = NiFpga_AnalogDemultiplexerV2_NI5771_TargetToHostFifoU64_ToHostA2FIFO;
		reqpixels[0] = NiFpga_AnalogDemultiplexerV2_NI5771_ControlU32_RequestedpixelsA1;
		reqpixels[1] = NiFpga_AnalogDemultiplexerV2_NI5771_ControlU32_RequestedpixelsA2;
		smplsperpixel = NiFpga_AnalogDemultiplexerV2_NI5771_ControlU16_Samplesperpixel;

	}

	FPGAAnalogDemultiplexer::~FPGAAnalogDemultiplexer(void) {
		status = NiFpga_Close(session, 0);
		status = NiFpga_Finalize();
		DBOUT(L"FPGAAnalogDemultiplexer::~FPGAAnalogDemultiplexer session closed");
	}

	void FPGAAnalogDemultiplexer::Initialize(parameters::InputsFPGA* _parameters) {
		if ( !initialized ) {
			parameters = dynamic_cast<parameters::InputsFPGAAnalogDemultiplexer*>(_parameters);

			SetClockSource(session, 0);

			FPGAInterface::Initialize(_parameters);
		}
	}

	double FPGAAnalogDemultiplexer::SetPixeltime(const uint32_t& _area, const double& _pixeltime) {
		// sampling rate of the NI 5771 is 1.5GHz
		uint16_t samplesperpixel = round2ui16(_pixeltime * 1E-6 * 1.5E9);

		// only set the pixeltime from area 1 (this holds for all areas)!
		if ( _area == 0 ) {
			DBOUT(L"FPGADemultiplexer::SetPixeltime Clockcycles per pixel" << samplesperpixel);
			// Since the Demultiplexer VI is working on arrays of 8 samples clockcycles has to be a multiple of 8
			if ( samplesperpixel%8 != 0 ) {
				samplesperpixel -= (samplesperpixel%8);
				DBOUT(L"FPGADemultiplexer::SetPixeltime Coerced samples per pixel" << samplesperpixel);
			}
			status = NiFpga_WriteU16(session, smplsperpixel, samplesperpixel);
		}
		return static_cast<double>(samplesperpixel)*1E6/1.5E9;
	}

	double FPGAAnalogDemultiplexer::SetLinetime(const uint32_t& _area, const double& _linetime) {
		// Not supported by FPGA VI
		return _linetime;
	}

	void FPGAAnalogDemultiplexer::SetTriggering(const bool& _waitfortrigger) {
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_AnalogDemultiplexerV2_NI5771_ControlBool_Waitfortrigger, _waitfortrigger);
	}

	void FPGAAnalogDemultiplexer::SetContinuousAcquisition(const bool& _cont) {
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_AnalogDemultiplexerV2_NI5771_ControlBool_Acquirecontinuously, _cont);
	}

	void FPGAAnalogDemultiplexer::SetRequestedPixels(const uint32_t& _area, const uint32_t& _reqpixels) {
		status = NiFpga_WriteU32(session, reqpixels[_area], _reqpixels);
		DBOUT(L"FPGAAnalogDemultiplexer::SetRequestedPixels area " << _area << L": " << _reqpixels);
	}

	void FPGAAnalogDemultiplexer::StartAcquisition() {
		NiFpga_Bool alreadyrunning = false;
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_AnalogDemultiplexerV2_NI5771_ControlBool_Acquire, &alreadyrunning);
		
		// Only clear fifos if not alreay running (e.g. by a previous StartAcquisition from another area!!) Leads to nasty bugs!
		if ( !alreadyrunning ) {
			ClearFIFOs();
			SetChannelProps();
			status = NiFpga_WriteBool(session, (uint32_t)NiFpga_AnalogDemultiplexerV2_NI5771_ControlBool_Acquire, true);
		}
	}

	void FPGAAnalogDemultiplexer::StopAcquisition() {
		DBOUT(L"FPGAAnalogDemultiplexer::StopAcquisition");
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_AnalogDemultiplexerV2_NI5771_ControlBool_Acquire, false);
	}

	int32_t FPGAAnalogDemultiplexer::ReadPixels(DaqMultiChunk<2, 2, uint16_t>& _chunk, const double& _timeout, bool& _timedout) {
		size_t remaining = 0;

		// we need enough space
		assert(_chunk.data.size() >= 2 * 2 * _chunk.PerChannel());

		NiFpga_Status stat = NiFpga_Status_Success;

		// Make temporary vector to hold the U64 data from both channels
		std::vector<uint64_t> u64data(_chunk.PerChannel());
		
		// Get the desired bitshift for each channel
		std::array<uint8_t, 4> bitshift;
		bitshift[0] = parameters->BitshiftA1Ch1();
		bitshift[1] = parameters->BitshiftA1Ch2();
		bitshift[2] = parameters->BitshiftA2Ch1();
		bitshift[3] = parameters->BitshiftA2Ch2();

		// Sets the desired baseline on the FPGA. Do it here so changes in GUI get transferred to the FPGA quickly.
		//SetChannelProps();

		for (uint32_t a = 0; a < 2; a++) {
			// Do the read from the FIFO
			stat = NiFpga_ReadFifoU64(session
				, fifos[a]								// select correct fifo
				, u64data.data()
				, _chunk.PerChannel()
				, static_cast<uint32_t>(_timeout * 1000)			// FPGA C API takes timeout in milliseconds, to be consistent with DAQmx we have _timeout in seconds
				, &remaining);
			_timedout = (stat == NiFpga_Status_FifoTimeout);

			DBOUT(L"FPGAAnalogDemultiplexer::ReadPixels area " << a << L" remaining: " << remaining);

			// avoid throwing exception on time out (since FpgaStatus status could throw on all errors)
			if (_timedout)
				return -1;
			status = stat;

			// U64 from FIFO has Ch1 in the higher 32 bits, Ch2 in the lower 32 bits
			auto itch1 = _chunk.GetDataStart(a);
			auto itch2 = _chunk.GetDataStart(a) + _chunk.PerChannel();
			std::for_each(std::begin(u64data), std::end(u64data), [&](const uint64_t& u64) {
				*itch1 = static_cast<uint16_t>((u64 >> 32) >> bitshift[0]);			// shift 32 bits to the right to get Ch1, then do bitshift
				*itch2 = static_cast<uint16_t>((u64 & 0xffff) >> bitshift[1]);		// leave only lower 16 bits to get Ch2, then do bitshift
				itch1++;
				itch2++;
			});
		}

		if ( status.Success() )
			return _chunk.PerChannel();
		return -1;
	}

	void FPGAAnalogDemultiplexer::SetChannelProps() {
		status = NiFpga_WriteU8(session, (uint32_t)NiFpga_AnalogDemultiplexerV2_NI5771_ControlU8_BaselineU8Ch1, parameters->BaselineCh1());
		status = NiFpga_WriteU8(session, (uint32_t)NiFpga_AnalogDemultiplexerV2_NI5771_ControlU8_BaselineU8Ch2, parameters->BaselineCh2());
		status = NiFpga_WriteU8(session, (uint32_t)NiFpga_AnalogDemultiplexerV2_NI5771_ControlU8_CutoffU8Ch1, parameters->CutoffCh1());
		status = NiFpga_WriteU8(session, (uint32_t)NiFpga_AnalogDemultiplexerV2_NI5771_ControlU8_CutoffU8Ch2, parameters->CutoffCh2());
		DBOUT(L"FPGAAnalogDemultiplexer::SetChannelProps baseline " << parameters->BaselineCh1() << L" " << parameters->BaselineCh2());
	}

	void FPGAAnalogDemultiplexer::CheckFPGADiagnosis() {
		NiFpga_Bool b(0);
		
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_ToHostA1FIFOOverflow, &b);
		parameters->diagnosis.ToHostOverflowA1 = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_ToHostA2FIFOOverflow, &b);
		parameters->diagnosis.ToHostOverflowA1 = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_InterloopFIFOOverflow, &b);
		parameters->diagnosis.InterloopOverflow = (b!=0);
		status = NiFpga_ReadBool(session, NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_InterloopFIFOtimeout, &b);
		parameters->diagnosis.InterloopTimeout = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_IOModuleAIOverRange, &b);
		parameters->diagnosis.AIOverRange = (b!=0);
		status = NiFpga_ReadBool(session, (uint32_t)NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_Acquiring, &b);
		parameters->diagnosis.Acquiring = (b!=0);
	}

	void FPGAAnalogDemultiplexer::ClearFIFOs() {
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_AnalogDemultiplexerV2_NI5771_ControlBool_ClearInterloopFIFO, 1);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		status = NiFpga_WriteBool(session, (uint32_t)NiFpga_AnalogDemultiplexerV2_NI5771_ControlBool_ClearInterloopFIFO, 0);

		// Stop FIFOs (clears them)
		for ( auto f : fifos )
			status = NiFpga_StopFifo(session, f);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		// Restart FIFOs
		for ( auto f : fifos )
			status = NiFpga_StartFifo(session, f);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		DBOUT(L"FPGAAnalogDemultiplexer::ClearFIFOs()");
	}

}

