#include "stdafx.h"
#include "FPGANoiseOutput.h"
#include "parameters/Inputs.h"

namespace scope {

	FPGANoiseOutput::FPGANoiseOutput() {	
		status = NiFpga_Initialize();

		char* const Bitfile = "devices\\fpga\\" NiFpga_NoiseOutput_PXIe7962R_Bitfile;

		// Load but do not run yet
		status = NiFpga_Open(Bitfile, NiFpga_NoiseOutput_PXIe7962R_Signature, "RIO0", 0, &session);
		DBOUT(L"FPGANoiseOutput: FPGA Session opened");

		// Reset the non-running FPGA to clear any bad stuff (e.g. after a program crash with open FPGA before)
		status = NiFpga_Reset(session);

		// YOU HAVE TO WAIT THAT LONG!!!
		// Otherwise you will occasionally (!!!) into strange problems (communication with fpga error -61046)
		// This took me very very long to figure out...
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		// Run after reset
		status = NiFpga_Run(session, 0);

		// Set the fifo constants
		fifos[0] = NiFpga_NoiseOutput_PXIe7962R_TargetToHostFifoU16_ToHostA1Ch1FIFO;
		fifos[1] = NiFpga_NoiseOutput_PXIe7962R_TargetToHostFifoU16_ToHostA1Ch2FIFO;
		fifos[2] = NiFpga_NoiseOutput_PXIe7962R_TargetToHostFifoU16_ToHostA2Ch1FIFO;
		fifos[3] = NiFpga_NoiseOutput_PXIe7962R_TargetToHostFifoU16_ToHostA2Ch2FIFO;
		smplsperpixel[0] = NiFpga_NoiseOutput_PXIe7962R_ControlU32_SamplesperlineA1;
		smplsperpixel[1] = NiFpga_NoiseOutput_PXIe7962R_ControlU32_SamplesperlineA2;
		smplsperline[0] = NiFpga_NoiseOutput_PXIe7962R_ControlU32_SamplesperlineA1;
		smplsperline[1] = NiFpga_NoiseOutput_PXIe7962R_ControlU32_SamplesperlineA2;
		reqpixels[0] = NiFpga_NoiseOutput_PXIe7962R_ControlU32_RequestedpixelsA1;
		reqpixels[1] = NiFpga_NoiseOutput_PXIe7962R_ControlU32_RequestedpixelsA2;
	}

	FPGANoiseOutput::~FPGANoiseOutput() {
		status = NiFpga_Close(session, 0);
		status = NiFpga_Finalize();
		DBOUT(L"FPGANoiseOutput::~FPGANoiseOutput session closed");
	}

	void FPGANoiseOutput::Initialize(parameters::InputsFPGA* _parameters) {
		if ( !initialized ) {
			parameters = dynamic_cast<parameters::InputsFPGANoiseOutput*>(_parameters);
			FPGAInterface::Initialize(_parameters);
		}
	}

	double FPGANoiseOutput::SetPixeltime(const uint32_t& _area, const double& _pixeltime) {
		// "sampling rate" is 40 MHz
		uint16_t samplesperpixel = round2ui16(_pixeltime * 1E-6 * 40E6);
		status = NiFpga_WriteU16(session, smplsperpixel[_area], samplesperpixel);

		DBOUT(L"FPGANoiseOutput::SetPixeltime samples per pixel" << samplesperpixel);

		return static_cast<double>(samplesperpixel*1E6/40E6);
	}

	double FPGANoiseOutput::SetLinetime(const uint32_t& _area, const double& _linetime) {
		// "sampling rate" is 40 MHz
		uint16_t samplesperline = round2ui16(_linetime * 1E-6 * 40E6);
		status = NiFpga_WriteU16(session, smplsperline[_area], samplesperline);

		DBOUT(L"FPGANoiseOutput::SetPixeltime samples per line" << samplesperline);

		return static_cast<double>(samplesperline*1E6/40E6);
	}

	void FPGANoiseOutput::SetTriggering(const bool& _waitfortrigger) {
		status = NiFpga_WriteBool(session, NiFpga_NoiseOutput_PXIe7962R_ControlBool_Waitfortrigger, _waitfortrigger);
	}

	void FPGANoiseOutput::SetContinuousAcquisition(const bool& _cont) {
		status = NiFpga_WriteBool(session, NiFpga_NoiseOutput_PXIe7962R_ControlBool_Acquirecontinuously, _cont);
	}

	void FPGANoiseOutput::SetRequestedPixels(const uint32_t& _area, const uint32_t& _reqpixels) {
		status = NiFpga_WriteU32(session, reqpixels[_area], _reqpixels);
	}

	void FPGANoiseOutput::StartAcquisition() {
		ClearFIFOs();
		status = NiFpga_WriteBool(session, NiFpga_NoiseOutput_PXIe7962R_ControlBool_Acquire, true);
	}

	int32_t FPGANoiseOutput::ReadPixels(const uint32_t& _area, DaqMultiChunk<2, 1, uint16_t>& _chunk, const double& _timeout, bool& _timedout) {
		size_t remaining = 0;

		NiFpga_Status stat = NiFpga_Status_Success;

		// Read each channels fifo
		for ( uint32_t c = 0 ; c < 2 ; c++ ) {
			stat = NiFpga_ReadFifoU16(session
					, fifos[_area * 2 + c]							// select correct fifo
					, &_chunk.data[c * _chunk.PerChannel()]			// offset start in vector for second channel pixels
					, _chunk.PerChannel()
					, static_cast<uint32_t>(_timeout * 1000)		// FPGA C API takes timeout in milliseconds, to be consistent with DAQmx we have _timeout in seconds
					, &remaining);
	
			_timedout = (stat == NiFpga_Status_FifoTimeout);

			// avoid throwing exception on time out (since FpgaStatus status could throw on all errors)
			// return immediately, makes no sense to also wait for the other channels, since anyway something went seriously wrong
			if ( _timedout )
				return -1;

			// this could throw on error (if we would use FPGAStatus instead of FPGAStatusSafe)
			status = stat;
		}

		if ( status.Success() )
			return _chunk.PerChannel();

		return -1;
	}

	int32_t FPGANoiseOutput::ReadPixels(DaqMultiChunk<2, 2, uint16_t>& _chunk, const double& _timeout, bool& _timedout) {
		size_t remaining = 0;

		NiFpga_Status stat = NiFpga_Status_Success;

		// Read each areas and each channels fifo
		for (uint32_t a = 0; a < 2; a++) {
			for (uint32_t c = 0; c < 2; c++) {
				stat = NiFpga_ReadFifoU16(session
					, fifos[a * 2 + c]									// select correct fifo
					, &_chunk.data[a * 2 * _chunk.PerChannel +  c * 2]	// offset start in vector 
					, _chunk.PerChannel()
					, static_cast<uint32_t>(_timeout * 1000)			// FPGA C API takes timeout in milliseconds, to be consistent with DAQmx we have _timeout in seconds
					, &remaining);

				_timedout = (stat == NiFpga_Status_FifoTimeout);

				// avoid throwing exception on time out (since FpgaStatus status could throw on all errors)
				// return immediately, makes no sense to also wait for the other channels, since anyway something went seriously wrong
				if (_timedout)
					return -1;

				// this could throw on error (if we would use FPGAStatus instead of FPGAStatusSafe)
				status = stat;
			}
		}

		if (status.Success())
			return _chunk.PerChannel();

		return -1;
	}

	void FPGANoiseOutput::StopAcquisition() {
		status = NiFpga_WriteBool(session, NiFpga_NoiseOutput_PXIe7962R_ControlBool_Acquire, false);
	}

	void FPGANoiseOutput::CheckFPGADiagnosis() {
		NiFpga_Bool b;
		status = NiFpga_ReadBool(session, NiFpga_NoiseOutput_PXIe7962R_IndicatorBool_ToHostA1Ch1Overflow, &b);
		parameters->diagnosis.ToHostOverflowA1Ch1 = (b!=0);
		status = NiFpga_ReadBool(session, NiFpga_NoiseOutput_PXIe7962R_IndicatorBool_ToHostA1Ch2Overflow, &b);
		parameters->diagnosis.ToHostOverflowA1Ch2 = (b!=0);
		status = NiFpga_ReadBool(session, NiFpga_NoiseOutput_PXIe7962R_IndicatorBool_ToHostA2Ch1Overflow, &b);
		parameters->diagnosis.ToHostOverflowA2Ch1 = (b!=0);
		status = NiFpga_ReadBool(session, NiFpga_NoiseOutput_PXIe7962R_IndicatorBool_ToHostA2Ch2Overflow, &b);
		parameters->diagnosis.ToHostOverflowA2Ch2 = (b!=0);
		status = NiFpga_ReadBool(session, NiFpga_NoiseOutput_PXIe7962R_IndicatorBool_Acquiring, &b);
		parameters->diagnosis.Acquiring = (b!=0);
	}

	void FPGANoiseOutput::ClearFIFOs() {
		// Stop FIFOs (clears them)
		for ( auto f : fifos )
			status = NiFpga_StopFifo(session, f);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		// Restart FIFOs
		for ( auto f : fifos )
			status = NiFpga_StartFifo(session, f);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}


}