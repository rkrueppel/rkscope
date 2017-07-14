#include "StdAfx.h"
#include "FPGAAnalogIntegrator.h"
#include "parameters/Inputs.h"
#include "helpers/DaqChunk.h"

namespace scope {

FPGAAnalogIntegrator::FPGAAnalogIntegrator()
	: FPGAIO5771(NiFpga_AnalogIntegrator_NI5771_IndicatorBool_UserCommandIdle
	, NiFpga_AnalogIntegrator_NI5771_IndicatorBool_PLLLocked
	, NiFpga_AnalogIntegrator_NI5771_IndicatorBool_Configured
	, NiFpga_AnalogIntegrator_NI5771_IndicatorBool_UserError
	, NiFpga_AnalogIntegrator_NI5771_IndicatorU8_UserCommandStatus
	, NiFpga_AnalogIntegrator_NI5771_ControlU8_UserCommand
	, NiFpga_AnalogIntegrator_NI5771_ControlU16_UserData0
	, NiFpga_AnalogIntegrator_NI5771_ControlU8_UserData1
	, NiFpga_AnalogIntegrator_NI5771_ControlBool_UserCommandCommit) {
	assert(SCOPE_NAREAS == 1);
	
	status = NiFpga_Initialize();

	char* const Bitfile = "devices\\fpga\\" NiFpga_AnalogIntegrator_NI5771_Bitfile;

	// Load but do not run yet
	status = NiFpga_Open(Bitfile, NiFpga_AnalogIntegrator_NI5771_Signature, "RIO0", 0, &session);
	DBOUT(L"FPGAAnalogIntegrator: FPGA Session opened");

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
	fifos[0] = NiFpga_AnalogIntegrator_NI5771_TargetToHostFifoU32_ToHostCh1FIFO;
	fifos[1] = NiFpga_AnalogIntegrator_NI5771_TargetToHostFifoU32_ToHostCh2FIFO;
}

FPGAAnalogIntegrator::~FPGAAnalogIntegrator() {
	status = NiFpga_Close(session, 0);
	status = NiFpga_Finalize();
	DBOUT(L"FPGAAnalogIntegrator::~FPGAAnalogIntegrator session closed");
}

void FPGAAnalogIntegrator::Initialize(parameters::InputsFPGA* _parameters) {
	if ( !initialized ) {
		parameters = dynamic_cast<parameters::InputsFPGAAnalogIntegrator*>(_parameters);

		SetClockSource(session, 0);

		FPGAInterface::Initialize(_parameters);
	}
}

double FPGAAnalogIntegrator::SetPixeltime(const uint32_t& _area, const double& _pixeltime) {
	// sampling rate of the NI 5771 is 1.5GHz
	uint16_t samplesperpixel = round2ui16(_pixeltime * 1E-6 * 1.5E9);
	status = NiFpga_WriteU16(session, NiFpga_AnalogIntegrator_NI5771_ControlU32_Samplesperpixel, samplesperpixel);

	DBOUT(L"FPGAAnalogIntegrator::SetPixeltime samples per pixel" << samplesperpixel);
	// Since the AnalogIntegrator VI is working on arrays of 8 samples clockcycles has to be a multiple of 8
	if ( samplesperpixel%8 != 0 ) {
		samplesperpixel -= (samplesperpixel%8);
		DBOUT(L"FPGAAnalogIntegrator::SetPixeltime Coerced samples per per pixel" << samplesperpixel);
	}

	return static_cast<double>(samplesperpixel)*1E6/1.5E9;
}

double FPGAAnalogIntegrator::SetLinetime(const uint32_t& _area, const double& _linetime) {
	// sampling rate of the NI 5771 is 1.5GHz
	uint16_t samplesperline = round2ui32(_linetime * 1E-6 * 1.5E9);
	status = NiFpga_WriteU16(session, NiFpga_AnalogIntegrator_NI5771_ControlU32_Samplesperline, samplesperline);

	DBOUT(L"FPGAAnalogIntegrator::SetPixeltime samples per line" << samplesperline);
	// Since the AnalogIntegrator VI is working on arrays of 8 samples clockcycles has to be a multiple of 8
	if ( samplesperline%8 != 0 ) {
		samplesperline -= (samplesperline%8);
		DBOUT(L"FPGAAnalogIntegrator::SetPixeltime Coerced samples per per pixel" << samplesperline);
	}

	return static_cast<double>(samplesperline)*1E6/1.5E9;
}

void FPGAAnalogIntegrator::SetTriggering(const bool& _waitfortrigger) {
	status = NiFpga_WriteBool(session, NiFpga_AnalogIntegrator_NI5771_ControlBool_Waitfortrigger, _waitfortrigger);
}

void FPGAAnalogIntegrator::SetContinuousAcquisition(const bool& _cont) {
	status = NiFpga_WriteBool(session, NiFpga_AnalogIntegrator_NI5771_ControlBool_Acquirecontinuously, _cont);
}

void FPGAAnalogIntegrator::SetRequestedPixels(const uint32_t& _area, const uint32_t& _reqpixels) {
	status = NiFpga_WriteU32(session, NiFpga_AnalogIntegrator_NI5771_ControlU32_Requestedpixels, _reqpixels);
} 

void FPGAAnalogIntegrator::StartAcquisition() {
	ClearFIFOs();
	SetChannelProps();
	status = NiFpga_WriteBool(session, NiFpga_AnalogIntegrator_NI5771_ControlBool_Acquire, true);
}

int32_t FPGAAnalogIntegrator::ReadPixels(DaqChunk& _chunk, const double& _timeout, bool& _timedout) {
	size_t remaining = 0;

	// only two channels and one area supported in FPGA vi
	assert( (_chunk.NChannels() == 2) && (_chunk.Area() == 0) );
	
	// we need enough space
	assert(_chunk.data.size() >= _chunk.PerChannel() * _chunk.NChannels());

	NiFpga_Status stat = NiFpga_Status_Success;

	std::vector<uint32_t> u32data(_chunk.PerChannel());
	std::vector<const uint8_t> bitshift(_chunk.NChannels());
	bitshift[0] = parameters->BitshiftCh1();
	bitshift[1] = parameters->BitshiftCh2();

	// Read each channels fifo
	for ( uint32_t c = 0 ; c < _chunk.NChannels() ; c++ ) {
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

		std::transform(std::begin(u32data), std::end(u32data), std::begin(_chunk.data)+c*_chunk.PerChannel(), [&](const uint32_t& u32) {
			return static_cast<uint16_t>(u32 >> bitshift[c]);
		} );

	}

	if ( status.Success() )
		return _chunk.PerChannel();

	return -1;
}

void FPGAAnalogIntegrator::StopAcquisition() {
	status = NiFpga_WriteBool(session, NiFpga_AnalogIntegrator_NI5771_ControlBool_Acquire, false);
}

void FPGAAnalogIntegrator::SetChannelProps() {
	// We want to right-shift bits thus *-1
	//status = NiFpga_WriteI8(session, NiFpga_AnalogIntegrator_NI5771_ControlI8_BitshiftCh1, -1 * parameters->BitshiftCh1());
	//status = NiFpga_WriteI8(session, NiFpga_AnalogIntegrator_NI5771_ControlI8_BitshiftCh2, -1 * parameters->BitshiftCh2());
	// We subtract baseline from cumulated 8 samples, thus 8*
	status = NiFpga_WriteU8(session, NiFpga_AnalogIntegrator_NI5771_ControlU8_Baselinex8Ch1, parameters->BaselineCh1());
	status = NiFpga_WriteU8(session, NiFpga_AnalogIntegrator_NI5771_ControlU8_Baselinex8Ch2, parameters->BaselineCh2());

	DBOUT(L"FPGAAnalogIntegrator::SetChannelProps " << parameters->BitshiftCh1() << L" " << parameters->BaselineCh1());
}

void FPGAAnalogIntegrator::CheckFPGADiagnosis() {
	NiFpga_Bool b;
	status = NiFpga_ReadBool(session, NiFpga_AnalogIntegrator_NI5771_IndicatorBool_ToHostCh1FIFOOverflow, &b);
	parameters->diagnosis.ToHostOverflowCh1 = (b!=0);
	status = NiFpga_ReadBool(session, NiFpga_AnalogIntegrator_NI5771_IndicatorBool_ToHostCh2FIFOOverflow, &b);
	parameters->diagnosis.ToHostOverflowCh2 = (b!=0);
	status = NiFpga_ReadBool(session, NiFpga_AnalogIntegrator_NI5771_IndicatorBool_InterloopFIFOOverflow, &b);
	parameters->diagnosis.InterloopOverflow = (b!=0);
	status = NiFpga_ReadBool(session, NiFpga_AnalogIntegrator_NI5771_IndicatorBool_InterloopFIFOTimeout, &b);
	parameters->diagnosis.InterloopTimeout = (b!=0);
	status = NiFpga_ReadBool(session, NiFpga_AnalogIntegrator_NI5771_IndicatorBool_Acquiring, &b);
	parameters->diagnosis.Acquiring = (b!=0);
	status = NiFpga_ReadBool(session, NiFpga_AnalogIntegrator_NI5771_IndicatorBool_IOModuleAIOverRange, &b);
	parameters->diagnosis.AIOverRange = (b!=0);
}

void FPGAAnalogIntegrator::ClearFIFOs() {
	// Clear interloop fifo
	status = NiFpga_WriteBool(session, NiFpga_AnalogIntegrator_NI5771_ControlBool_ClearInterloopFIFO, true);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	status = NiFpga_WriteBool(session, NiFpga_AnalogIntegrator_NI5771_ControlBool_ClearInterloopFIFO, false);

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