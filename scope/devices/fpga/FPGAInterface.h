#pragma once

#include "config\config_choices.h"
#include "helpers\DaqChunks.h"
#include "helpers\SupportedAreas.h"
#include "helpers\FPGAException.h"
#include "NiFpga.h"

// Forward declaration
namespace scope {
	namespace parameters {
		class InputsFPGA;
	}
}

namespace scope {

	/** Abstract base class for FPGA classes
	* @ingroup ScopeComponentsHardware */
	class FPGAInterface 
		: virtual public SupportedAreas<0, 0> {

		protected:
			/** current FPGA status. We need the Safe (non-throwing) version here, because exceptions can crash (total computer freeze) the NI FPGA library
			* during debug runs (probably a problem with the stack unwindinw?). */
			FPGAStatusSafe status;
	
			/** NI FPGA session handle */
			NiFpga_Session session;

			/** true if already initialized */
			bool initialized;

		public:
			FPGAInterface();
	
			/** Ends the FPGA program */
			virtual ~FPGAInterface();

			/** Set initial parameters. Derived classes override this and cast _parameters to the appropriate derived type. Derived functions should check if already initialized. */
			virtual void Initialize(parameters::InputsFPGA* _parameters) { initialized = true; }

			/** Sets the time per pixel/dwell time (in seconds)
			* @param[in] _area for which area
			* @param[in] _pixeltime the pixel dwell time in seconds
			* @returns the actual pixel dwell time, can be different than requested due to finite clock rate */
			virtual double SetPixeltime(const uint32_t& _area, const double& _pixeltime) { return _pixeltime; }

			/** Sets the time per line (in seconds) for the generation of the line clock (if implemented)
			* @param[in] _area for which area
			* @param[in] _linetime the line time in seconds
			* @returns the actual line time, can be different than requested due to finite clock rate */
			virtual double SetLinetime(const uint32_t& _area, const double& _linetime) { return _linetime; }

			/** Sets if the FPGA should wait for a trigger before starting acquisition
			* @param[in] _waitfortrigger true if wait for trigger */
			virtual void SetTriggering(const bool& _waitfortrigger) { }

			/** Sets if the FPGA should acquire data continuously or acquire the number of pixels per channel set with SetRequestedPixels
			* @param[in] _cont yes or no */
			virtual void SetContinuousAcquisition(const bool& _cont) { }

			/** Sets the number of pixels per channel the FPGA should acquire, set to -1 for live scanning
			* @param[in] _area for which area
			* @param[in] _reqpixels number of pixels the FPGA should acquire before automatically stopping acquisition */
			virtual void SetRequestedPixels(const uint32_t& _area, const uint32_t& _reqpixels) { }

			/** Starts the acquisition on the FPGA */
			virtual void StartAcquisition() { }

			/** Read only pixels from the FPGA FIFO (dummy function, returns -1 (error)).
			* @param[in] _area which area to read from
			* @param[in, out] _chunk the (single area) daq chunk to read into, inside _chunk is information about 
			* the number of pixels per channel to read
			* @param[in] _timeout time out for reading in seconds
			* @param[out] _timedout set to true if reading timed out
			* @return number of read pixels per channel or -1 on error */
			virtual int32_t ReadPixels(const uint32_t& _area, DaqChunk<uint16_t>& _chunk, const double& _timeout, bool& _timedout);

			/** Read only pixels from the FPGA FIFO (one specified area, two channels). Not implemented in base class FPGAInterface, returns -1 (error).
			* @param[in] _area which area to read from
			* @param[in, out] _chunk the (single area, two channel) daq chunk to read into, inside _chunk is information about
			* the number of pixels per channel to read
			* @param[in] _timeout time out for reading in seconds
			* @param[out] _timedout set to true if reading timed out
			* @return number of read pixels per channel or -1 on error */
			virtual int32_t ReadPixels(const uint32_t& _area, DaqMultiChunk<2, 1, uint16_t>& _chunk, const double& _timeout, bool& _timedout);
			
			/** Read only pixels from the FPGA FIFO (two areas, starting with the specified area, two channels). Not implemented in base class FPGAInterface, returns -1 (error).
			* @param[in] _area first area of the two consecutive ones to read from
			* @param[in, out] _chunk the (two area, two channel) daq chunk to read into, inside _chunk is information about
			* the number of pixels per channel to read
			* @param[in] _timeout time out for reading in seconds
			* @param[out] _timedout set to true if reading timed out
			* @return number of read pixels per channel or -1 on error */
			virtual int32_t ReadPixels(const uint32_t& _area, DaqMultiChunk<2, 2, uint16_t>& _chunk, const double& _timeout, bool& _timedout);

			/** Read only pixels from the FPGA FIFO (one area, two channels, plus sync signal). Not implemented in base class FPGAInterface, returns -1 (error).
			* @param[in] _area area to read from
			* @param[in, out] _chunk the (one area, two channel, plus sync signale) daq chunk to read into, inside _chunk is information about
			* the number of pixels per channel to read
			* @param[in] _timeout time out for reading in seconds
			* @param[out] _timedout set to true if reading timed out
			* @return number of read pixels per channel or -1 on error */
			virtual int32_t ReadPixels(const uint32_t& _area, DaqMultiChunkResonance<2, 1, uint16_t>& _chunk, const double& _timeout, bool& _timedout);

			/** Read only pixels from the FPGA FIFO (two areas starting with the specified area, two channels, plus sync signal). Not implemented in base class FPGAInterface, returns -1 (error).
			* @param[in] _area first area of the two consecutive ones to read from
			* @param[in, out] _chunk the (two areas, two channel, plus sync signale) daq chunk to read into, inside _chunk is information about
			* the number of pixels per channel to read
			* @param[in] _timeout time out for reading in seconds
			* @param[out] _timedout set to true if reading timed out
			* @return number of read pixels per channel or -1 on error */
			virtual int32_t ReadPixels(const uint32_t& _area, DaqMultiChunkResonance<2, 2, uint16_t>& _chunk, const double& _timeout, bool& _timedout);

			/** Stops the acquisition on the FPGA */
			virtual void StopAcquisition() { }

			/** Sets the scanner delay on the FPGA (used currently for resonance scanners only) */
			virtual void SetScannerdelay(const uint32_t& _scannerdelay) {}

			/** @return the current FPGA status */
			FPGAStatusSafe CurrentStatus() const { return status; }
	};

}