#include "stdafx.h"
#include "FPGAIO5771.h"

namespace scope {

FPGAIO5771::FPGAIO5771(const uint32_t& _user_command_idle
	, const uint32_t& _pll_locked
	, const uint32_t& _configured
	, const uint32_t& _user_error
	, const uint32_t& _user_command_status
	, const uint32_t& _user_command_control
	, const uint32_t& _user_data_0_control
	, const uint32_t& _user_data_1_control
	, const uint32_t& _user_command_commit)
	: user_command_idle_indicator(_user_command_idle)
	, pll_locked_indicator(_pll_locked)
	, configured_indicator(_configured)
	, user_error_indicator(_user_error)
	, user_command_status_indicator(_user_command_status)
	, user_command_control(_user_command_control)
	, user_data_0_control(_user_data_0_control)
	, user_data_1_control(_user_data_1_control)
	, user_command_commit_control(_user_command_commit) {

}

void FPGAIO5771::WaitForIdle(NiFpga_Session _session) {
	NiFpga_Bool idle = false;
	uint32_t waitcounter = 0;

	// Wait until idle indicator is true
	do {
		iostatus = NiFpga_ReadBool(_session, user_command_idle_indicator, &idle);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));					// Wait 20 milliseconds
		waitcounter += 20;
	} while ( !idle && (waitcounter < 2000) );
	if ( waitcounter >= 2000 )
		throw FPGAException(-1, "SetClockSource timeout");
}

void FPGAIO5771::SetClockSource(NiFpga_Session _session, const uint8_t& _clock_source) {
	NiFpga_Bool configured = false;
	uint32_t waitcounter = 0;

	// Wait until moduled has configured itself
	do {
		iostatus = NiFpga_ReadBool(_session, configured_indicator, &configured);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));					// Wait 20 milliseconds
		waitcounter += 20;
	} while ( !configured && (waitcounter < 2000) );
	if ( waitcounter >= 5000 )
		throw FPGAException(-1, "SetClockSource adapter module not configured");

	// Write user command
	iostatus = NiFpga_WriteU8(_session, user_command_control, 0);
	iostatus = NiFpga_WriteU16(_session, user_data_0_control, static_cast<uint16_t>(_clock_source));
	iostatus = NiFpga_WriteU8(_session, user_data_1_control, 0);

	WaitForIdle(_session);

	// Commit user command
	iostatus = NiFpga_WriteBool(_session, user_command_commit_control, 0);
	iostatus = NiFpga_WriteBool(_session, user_command_commit_control, 1);
	iostatus = NiFpga_WriteBool(_session, user_command_commit_control, 0);

	WaitForIdle(_session);

	// Check for error
	uint8_t err = 0;
	iostatus = NiFpga_ReadU8(_session, user_error_indicator, &err);
	if ( err != 0 )
		throw FPGAException(-1, "SetClockSource user error");

	// If PLL on, wait for lock
	if ( (_clock_source == 1) || (_clock_source == 3) ) {
		NiFpga_Bool locked = false;
		waitcounter = 0;

		do {
			iostatus = NiFpga_ReadBool(_session, pll_locked_indicator, &locked);
			std::this_thread::sleep_for(std::chrono::milliseconds(20));					// Wait 20 milliseconds
			waitcounter += 20;
		} while ( !locked && (waitcounter < 5000) );
		if ( waitcounter >= 5000 )
			throw FPGAException(-1, "SetClockSource PLL not locked");

		// Wait 6 seconds for the lock to settle (! this is copied from NI's example Labview code...)
		std::this_thread::sleep_for(std::chrono::seconds(6));
	}
}

bool FPGAIO5771::CheckIOModule(NiFpga_Session _session) {
	return true;
}

}