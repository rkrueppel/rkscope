#pragma once

#include "helpers/FPGAException.h"

// Forward declaration
typedef uint32_t NiFpga_Session;

namespace scope {

/** Handels the NI FlexRIO adapter module IO-5771 */
class FPGAIO5771 {

protected:
	/** the User Command Idle indicator on the FPGA */
	const uint32_t user_command_idle_indicator;

	/** the PLL locked indicator on the FPGA */
	const uint32_t pll_locked_indicator;
	
	/** the Configure? indicator on the FPGA */
	const uint32_t configured_indicator;

	/** the User Error indicator on the FPGA */
	const uint32_t user_error_indicator;

	/** the User Command Status indicator on the FPGA */
	const uint32_t user_command_status_indicator;

	/** the User Command control on the FPGA */
	const uint32_t user_command_control;

	/** the User Data 0 control on the FPGA */
	const uint32_t user_data_0_control;

	/** the User Data 1 control on the FPGA */
	const uint32_t user_data_1_control;

	/** the User Command Commit control on the FPGA */
	const uint32_t user_command_commit_control;

	/** Current status of the module. We need the Safe (non-throwing) version here, because exceptions can crash (total computer freeze) the NI FPGA library
	* during debug runs (probably a problem with the stack unwinding?). */
	FPGAStatusSafe iostatus;

protected:
	/** Wait for user command idle */
	void WaitForIdle(NiFpga_Session _session);

public:
	/** Get the FPGA VI control/indicator ids */
	FPGAIO5771(const uint32_t& _user_command_idle
		, const uint32_t& _pll_locked
		, const uint32_t& _configured
		, const uint32_t& _user_error
		, const uint32_t& _user_command_status
		, const uint32_t& _user_command_control
		, const uint32_t& _user_data_0_control
		, const uint32_t& _user_data_1_control
		, const uint32_t& _user_command_commit);

	/** Possible clock sources (see adapter modules help and Configure Clock.vi from NI 5771 Clock Select example for code)
	* - 0: Internal clock, PLL off
	* - 1: Internal clock, PLL on (REF IN)
	* - 2: External clock, CLK IN
	* - 3: Internal clock, PLL on (IoModSyncClk)
	* Times out after 2 seconds and throws. Note: Wait at least 500ms after WriteOnBoardClockFrequency before calling SetClockSource!!!!!!!
	* @param[in] _session current handle of the connection to the FPGA
	* @param[in] _clock_source the new clock source to use 
	* @warning Attention, this function takes at least 6 seconds if using PLL on (NI seems to recommend to wait that long for the PLL to settle) */
	void SetClockSource(NiFpga_Session _session, const uint8_t& _clock_source = 0);

	/** Necessary calls at the moment not supported by NI FPGA API 12.0, see CheckIOModule.vi for what to do. */
	bool CheckIOModule(NiFpga_Session _session);
};

}
