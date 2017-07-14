#include "stdafx.h"
#include "FPGAIO5751.h"

namespace scope {

FPGAIO5751::FPGAIO5751(const uint32_t& _configured)
	: configured_indicator(_configured) {

}

void FPGAIO5751::SetClockSource(NiFpga_Session _session, const uint8_t& _clock_source) {
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

	//// If PLL on, wait for lock
	//if ( (_clock_source == 1) || (_clock_source == 3) ) {
	//	NiFpga_Bool locked = false;
	//	uint32_t waitcounter = 0;

	//	do {
	//		iostatus = NiFpga_ReadBool(_session, pll_locked_indicator, &locked);
	//		std::this_thread::sleep_for(std::chrono::milliseconds(20));					// Wait 20 milliseconds
	//		waitcounter += 20;
	//	} while ( !locked && (waitcounter < 5000) );
	//	if ( waitcounter >= 5000 )
	//		throw FPGAException(-1, "SetClockSource PLL not locked");

	//	// Wait 6 seconds for the lock to settle (! this is copied from NI's example Labview code...)
	//	std::this_thread::sleep_for(std::chrono::seconds(6));
	//}
}

bool FPGAIO5751::CheckIOModule(NiFpga_Session _session) {
	return true;
}

}