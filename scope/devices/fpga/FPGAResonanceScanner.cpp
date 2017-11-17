#include "StdAfx.h"
#include "FPGAResonanceScanner.h"
#include "parameters/Inputs.h"
#include "helpers/DaqMultiChunk.h"
#include "helpers/DaqMultiChunkResonance.h"

namespace scope {

FPGAResonanceScanner::FPGAResonanceScanner()
	: FPGAIO5751(NiFpga_ResonanceScanner_IndicatorBool_Configured) {
	assert(SCOPE_NAREAS == 1);
	
	status = NiFpga_Initialize();

	char* const Bitfile = "devices\\fpga\\" NiFpga_ResonanceScanner_Bitfile;

	// Load but do not run yet
	status = NiFpga_Open(Bitfile, NiFpga_ResonanceScanner_Signature, "RIO0", 0, &session);
	DBOUT(L"FPGAResonanceScanner: FPGA Session opened");

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
	fifos[0] = NiFpga_ResonanceScanner_TargetToHostFifoU32_ToHostCh1FIFO;
	fifos[1] = NiFpga_ResonanceScanner_TargetToHostFifoU32_ToHostCh2FIFO;
}

FPGAResonanceScanner::~FPGAResonanceScanner() {
	status = NiFpga_Close(session, 0);
	status = NiFpga_Finalize();
	DBOUT(L"FPGAResonanceScanner::~FPGAResonanceScanner session closed");
}

void FPGAResonanceScanner::Initialize(parameters::InputsFPGA* _parameters) {
	if ( !initialized ) {
		parameters = dynamic_cast<parameters::InputsFPGAResonanceScanner*>(_parameters);

		SetClockSource(session, 0);
		
		uint32_t chunksize = 1000000;

		status = NiFpga_ConfigureFifo(session, fifos[0], 5*chunksize);
		status = NiFpga_ConfigureFifo(session, fifos[1], 5*chunksize);

		FPGAInterface::Initialize(_parameters);
	}
}

double FPGAResonanceScanner::SetPixeltime(const uint32_t& _area, const double& _pixeltime) {
	uint16_t samplesperpixel = round2ui16(_pixeltime * 1E-6 * parameters->AcquisitionClockRate());
	status = NiFpga_WriteU16(session, NiFpga_ResonanceScanner_ControlU32_Samplesperpixel, samplesperpixel);

	DBOUT(L"FPGAResonanceScanner::SetPixeltime samples per pixel: " << samplesperpixel);

	return static_cast<double>(samplesperpixel)*1E6/parameters->AcquisitionClockRate();
}

double FPGAResonanceScanner::SetLinetime(const uint32_t& _area, const double& _linetime) {
	uint16_t samplesperline = round2ui16(_linetime * 1E-6 * parameters->AcquisitionClockRate());
	status = NiFpga_WriteU16(session, (uint32_t)NiFpga_ResonanceScanner_ControlU32_Samplesperline, samplesperline);

	DBOUT(L"FPGAResonanceScanner::SetPixeltime samples per line" << samplesperline);

	return static_cast<double>(samplesperline)*1E6/parameters->AcquisitionClockRate();
}

void FPGAResonanceScanner::SetTriggering(const bool& _waitfortrigger) {
	status = NiFpga_WriteBool(session, (uint32_t)NiFpga_ResonanceScanner_ControlBool_Waitfortrigger, _waitfortrigger);
}

void FPGAResonanceScanner::SetContinuousAcquisition(const bool& _cont) {
	status = NiFpga_WriteBool(session, (uint32_t)NiFpga_ResonanceScanner_ControlBool_Acquirecontinuously, _cont);
}

void FPGAResonanceScanner::SetRequestedPixels(const uint32_t& _area, const uint32_t& _reqpixels) {
	status = NiFpga_WriteU32(session, (uint32_t)NiFpga_ResonanceScanner_ControlU32_Requestedpixels, _reqpixels);
} 

void FPGAResonanceScanner::StartAcquisition() {
	ClearFIFOs();
	SetChannelProps();
	status = NiFpga_WriteBool(session, (uint32_t)NiFpga_ResonanceScanner_ControlBool_Acquire, true);
}

int32_t FPGAResonanceScanner::ReadPixels(const uint32_t& _area, DaqMultiChunk<SCOPE_NBEAM_AREAS, uint16_t>& _chunk, const double& _timeout, bool& _timedout) {
	size_t remaining = 0;

	// only two channels and one area supported in FPGA vi
	assert( (_chunk.NChannels() == 2) && (_area == 0) );
	
	// we need enough space
	assert(_chunk.data.size() >= _chunk.PerChannel() * _chunk.NChannels());

	NiFpga_Status stat = NiFpga_Status_Success;

	std::vector<uint32_t> u32data(_chunk.PerChannel());
	std::vector<int32_t> bitshift(_chunk.NChannels());
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

		// Isolate pixel uint16 from uint32
		std::transform(std::begin(u32data), std::end(u32data), std::begin(_chunk.data) + c*_chunk.PerChannel(), [](const uint32_t& _u32) {
			return static_cast<uint16_t>(_u32 & 0x0000ffff);
		});
	}

	// We have to upcast here to get access to the resonance sync vector
	auto chunkres = dynamic_cast<DaqMultiChunkResonance<SCOPE_NBEAM_AREAS, uint16_t>&>(_chunk);
	// Isolate sync uint16/bool from uint32
	std::transform(std::begin(u32data), std::end(u32data), std::begin(chunkres.resSync), [](const uint32_t& _u32) {
		return (_u32 >> 31) != 0;
	});

	if ( status.Success() )
		return _chunk.PerChannel();

	return -1;
}

void FPGAResonanceScanner::StopAcquisition() {
	status = NiFpga_WriteBool(session, (uint32_t)NiFpga_ResonanceScanner_ControlBool_Acquire, false);
}

void FPGAResonanceScanner::SetScannerdelay(const uint32_t& _scannerdelay) {
	status = NiFpga_WriteU32(session, (uint32_t)NiFpga_ResonanceScanner_ControlI16_Scannerdelay, _scannerdelay);
}

void FPGAResonanceScanner::SetChannelProps() {
	// We want to right-shift bits thus *-1
	//status = NiFpga_WriteI8(session, NiFpga_ResonanceScanner_ControlI8_BitshiftCh1, -1 * parameters->BitshiftCh1());
	//status = NiFpga_WriteI8(session, NiFpga_ResonanceScanner_ControlI8_BitshiftCh2, -1 * parameters->BitshiftCh2());
	
	status = NiFpga_WriteU16(session, (uint32_t)NiFpga_ResonanceScanner_ControlU16_Baselinex8Ch1, static_cast<uint16_t>(parameters->BaselineCh1()));
	status = NiFpga_WriteU16(session, (uint32_t)NiFpga_ResonanceScanner_ControlU16_Baselinex8Ch2, static_cast<uint16_t>(parameters->BaselineCh2()));

	DBOUT(L"FPGAResonanceScanner::SetChannelProps " << parameters->BitshiftCh1() << L" " << parameters->BaselineCh1());
}

void FPGAResonanceScanner::CheckFPGADiagnosis() {
	NiFpga_Bool b;
	status = NiFpga_ReadBool(session, (uint32_t)NiFpga_ResonanceScanner_IndicatorBool_ToHostCh1FIFOOverflow, &b);
	parameters->diagnosis.ToHostOverflowCh1 = (b!=0);
	status = NiFpga_ReadBool(session, (uint32_t)NiFpga_ResonanceScanner_IndicatorBool_ToHostCh2FIFOOverflow, &b);
	parameters->diagnosis.ToHostOverflowCh2 = (b!=0);
	status = NiFpga_ReadBool(session, (uint32_t)NiFpga_ResonanceScanner_IndicatorBool_InterloopFIFOOverflow, &b);
	parameters->diagnosis.InterloopOverflow = (b!=0);
	status = NiFpga_ReadBool(session, (uint32_t)NiFpga_ResonanceScanner_IndicatorBool_InterloopFIFOTimeout, &b);
	parameters->diagnosis.InterloopTimeout = (b!=0);
	status = NiFpga_ReadBool(session, (uint32_t)NiFpga_ResonanceScanner_IndicatorBool_Acquiring, &b);
	parameters->diagnosis.Acquiring = (b!=0);
	status = NiFpga_ReadBool(session, (uint32_t)NiFpga_ResonanceScanner_IndicatorBool_IOModuleAIOverRange, &b);
	parameters->diagnosis.AIOverRange = (b!=0);
}

void FPGAResonanceScanner::ClearFIFOs() {
	// Clear interloop fifo
	status = NiFpga_WriteBool(session, (uint32_t)NiFpga_ResonanceScanner_ControlBool_ClearInterloopFIFO, true);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	status = NiFpga_WriteBool(session, (uint32_t)NiFpga_ResonanceScanner_ControlBool_ClearInterloopFIFO, false);

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