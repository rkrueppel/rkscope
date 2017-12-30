#pragma once

#include "helpers/FPGAException.h"

// Forward declaration
typedef uint32_t NiFpga_Session;

namespace scope {

	/** Handels the NI FlexRIO adapter module IO-6587 */
	class FPGAIO6587 {

	protected:
		/** the clock ready indicator on the FPGA vi */
		const uint32_t onboard_clock_ready_indicator;

		/** the clock write data control on the FPGA vi */
		const uint32_t onboard_clock_write_data_control;

		/** the clock write control on the FPGA vi */
		const uint32_t onboard_clock_write_control;

		/** the xpoint switch ready indicator on the FPGA vi */
		const uint32_t xpoint_switch_ready_indicator;

		/** the clocksource control on the FPGA vi */
		const uint32_t clock_source_control;

		/** the xpoint switch write control on the FPGA vi */
		const uint32_t xpoint_switch_write_control;

		/** the commit control on the FPGA vi */
		const uint32_t commit_control;

		/** the acquisition reset control on the FPGA vi */
		const uint32_t acquisition_reset_control;

		/** register data for writing to the clock chip */
		std::array<uint16_t, 9> regndata;

		/** Current status of the module. We need the Safe (non-throwing) version here, because exceptions can crash (total computer freeze) the NI FPGA library
		* during debug runs (probably a problem with the stack unwindinw?). */
		FPGAStatusSafe iostatus;

	protected:
		/** Generates value for programming the Si570 clock chip.
		* Code translated from NI's 'Clock Frequency Setup.vi' from e.g. examples/FlexRIO/IO Modules/NI 6587/SerialGenExportClk.
		* See also here: http://digital.ni.com/public.nsf/allkb/D8F9352F85E7FFE18625780A007A3345.
		* See also the Si570 datasheet.
		* @param[in] _clock_freq the IO module adapter clock frequency (100-800 MHz), since samples are acquired with double data rate, the subsequent sampling rate is (200-1600 MHz). */
		void ClockFrequencySetup(const double& _clock_freq);

	public:
		/** @param[in] _clock_ready id of the clock_ready indicator
		* @param[in] _clock_write_data id of the clock_write_data control
		* @param[in] _clock_write id of the clock_write control
		* @param[in] _xpoint_ready id of the xpoint_ready indicator
		* @param[in] _clock_source id of the clock_source control
		* @param[in] _xpoint_write id of the xpoint_write control
		* @param[in] _commit_control id of the commit control
		* @param[in] _acquisition_reset_control id of the acquisition_reset control */
		FPGAIO6587(const uint32_t& _clock_ready
			, const uint32_t& _clock_write_data
			, const uint32_t& _clock_write
			, const uint32_t& _xpoint_ready
			, const uint32_t& _clock_source
			, const uint32_t& _xpoint_write
			, const uint32_t& _commit_control
			, const uint32_t& _acquisition_reset_control);

		/** Necessary calls at the moment not supported by NI FPGA API 12.0, see CheckIOModule.vi for what to do. */
		bool CheckIOModule(NiFpga_Session _session);

		/** Writes the program for the Si570 clock chip to the device.
		* Times out after 2 seconds and throws
		* @param[in] _session current handle of the connection to the FPGA
		* @param[in] _clock_freq the desired clock frequency (100-800E6 Hz) */
		void WriteOnboardClockFrequency(NiFpga_Session _session, const double& _clock_freq);

		/** Possible clock sources (see SetClockSource.vi from NI examples)\n
		* - 0: tristate\n
		* - 2: Sync clock (PXIe only)\n
		* - 3: Adapter module onboard clock\n
		* - 4: Strobe from crosspoint switch\n
		* - 5: Strobe bypass\n
		* Times out after 2 seconds and throws. Note: Wait at least 500ms after WriteOnBoardClockFrequency before calling SetClockSource!!!!!!!
		* @param[in] _session current handle of the connection to the FPGA
		* @param[in] _clock_source the new clock source to use */
		void SetClockSource(NiFpga_Session _session, const uint8_t& _clock_source = 3);

		/** Commits the clock frequency and clock source writings and resets the IO module acquisition circuit.
		* After this, the IO module is good to go
		* @param[in] _session current handle of the connection to the FPGA */
		void InitializeAcquisition(NiFpga_Session _session);
	};

}
