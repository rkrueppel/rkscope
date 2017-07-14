#include "StdAfx.h"
#include "FPGAIO6587.h"
#include "helpers/helpers.h"
#include "NiFpga.h"

namespace scope {

FPGAIO6587::FPGAIO6587(const uint32_t& _clock_ready
	, const uint32_t& _clock_write_data
	, const uint32_t& _clock_write
	, const uint32_t& _xpoint_ready
	, const uint32_t& _clock_source
	, const uint32_t& _xpoint_write
	, const uint32_t& _commit_control
	, const uint32_t& _acquisition_reset_control)
	: onboard_clock_ready_indicator(_clock_ready)
	, onboard_clock_write_data_control(_clock_write_data)
	, onboard_clock_write_control(_clock_write)
	, xpoint_switch_ready_indicator(_xpoint_ready)
	, clock_source_control(_clock_source)
	, xpoint_switch_write_control(_xpoint_write)
	, commit_control(_commit_control)
	, acquisition_reset_control(_acquisition_reset_control)
	, iostatus(NiFpga_Status_Success) {
}

bool FPGAIO6587::CheckIOModule(NiFpga_Session _session) {
	return true;
}

void FPGAIO6587::ClockFrequencySetup(const double& _clock_freq) {
	uint8_t i = 1;
	std::vector<uint8_t> n1(65, 1);
	std::generate(std::begin(n1)+1, std::end(n1), [&]() { return (uint8_t)(i++*2); } );
	std::array<uint8_t, 6> hsdiv = { 11, 9, 7, 6, 5, 4 };
	std::array<uint8_t, 9> regaddr = { 0x89, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0x89, 0x87 };
	std::array<uint8_t, 9> writedata = { 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x40 };
	
	const double xtalfreq = 114.285E6;
	double rfreqnew_dec = 0.0;
	uint64_t rfreqnew = 0;
	double dconew = 5.67E9;
	uint8_t hsdivnew = 0;
	uint8_t n1new = 0;
	std::for_each(std::begin(n1), std::end(n1), [&](const uint8_t& _n1) {
		for ( auto h : hsdiv ) {
			double tmp = _n1 * (h * _clock_freq);
			if ( ((tmp >= 4.85E9) && (tmp < 5.67E9)) && (tmp < dconew) ) {
				hsdivnew = h;
				n1new = _n1;
				dconew = tmp;
			}
		}
	} );
	rfreqnew_dec = dconew / xtalfreq;
	rfreqnew = static_cast<uint64_t>(round2ui64(rfreqnew_dec * 268435456));				// 2^28
	writedata[1] = ((hsdivnew-4) << 5) + ((n1new-1) >> 2);
	writedata[2] = ((n1new-1) << 6) + static_cast<uint8_t>(rfreqnew >> 32);				// use the upper 8 bits of rfreqnew
	writedata[3] = HIBYTE(HIWORD(LODWORD32(rfreqnew)));
	writedata[4] = LOBYTE(HIWORD(LODWORD32(rfreqnew)));
	writedata[5] = HIBYTE(LOWORD(LODWORD32(rfreqnew)));
	writedata[6] = LOBYTE(LOWORD(LODWORD32(rfreqnew)));
	std::transform(std::begin(writedata), std::end(writedata), std::begin(regaddr), std::begin(regndata)
		, [](const uint8_t& w, const uint8_t& r) { return MAKEWORD(w, r); } );
}

void FPGAIO6587::WriteOnboardClockFrequency(NiFpga_Session _session, const double& _clock_freq) {
	// Supported clock frequencies between 10 and 800 MHz (-> sampling frequencies 20-1600 MHz)
	assert( (_clock_freq >= 10E6) && (_clock_freq <= 800E6) );

	std::fill(std::begin(regndata), std::end(regndata), (uint8_t)0);
	ClockFrequencySetup(_clock_freq);

	// Write register data (calculated in ClockFrequencySetup to the IO module clock chip
	std::for_each(std::begin(regndata), std::end(regndata), [&](uint16_t rd) {
		NiFpga_Bool ready = false;
		uint32_t waitcounter = 0;
		do {
			iostatus = NiFpga_ReadBool(_session, onboard_clock_ready_indicator, &ready);
			std::this_thread::sleep_for(std::chrono::milliseconds(50));					// Wait 50 milliseconds
			waitcounter += 50;
		} while ( !ready && (waitcounter < 5000) );
		if ( waitcounter >= 5000 )
			throw FPGAException(-1, "WriteOnboardClockFrequency timeout");
		iostatus = NiFpga_WriteU16(_session, onboard_clock_write_data_control, rd);
		iostatus = NiFpga_WriteBool(_session, onboard_clock_write_control, false);
		iostatus = NiFpga_WriteBool(_session, onboard_clock_write_control, true);
		iostatus = NiFpga_WriteBool(_session, onboard_clock_write_control, false);
	} );
	DBOUT(L"FPGAIO6587::WriteOnboardClockFrequency finished");
}

void FPGAIO6587::SetClockSource(NiFpga_Session _session, const uint8_t& _clock_source) {
	NiFpga_Bool ready = false;
	uint32_t waitcounter = 0;

	// Set xpoint clock switch
	do {
		iostatus = NiFpga_ReadBool(_session, xpoint_switch_ready_indicator, &ready);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));						// Wait 50 milliseconds
		waitcounter += 50;
	} while ( !ready  && (waitcounter < 5000) );
	if ( waitcounter >= 5000 )
			throw FPGAException(-1, "SetClockSource timeout");


	// Set clock source
	iostatus = NiFpga_WriteU8(_session, clock_source_control, _clock_source);
	
	// Write xpoint switch
	iostatus = NiFpga_WriteBool(_session, xpoint_switch_write_control, false);
	iostatus = NiFpga_WriteBool(_session, xpoint_switch_write_control, true);
	iostatus = NiFpga_WriteBool(_session, xpoint_switch_write_control, false);
}

void FPGAIO6587::InitializeAcquisition(NiFpga_Session _session) {
	// Commit -> configuration loop on FPGA finishes
	iostatus = NiFpga_WriteBool(_session, commit_control, true);

	// Reset acquisition
	iostatus = NiFpga_WriteBool(_session, acquisition_reset_control, false);
	iostatus = NiFpga_WriteBool(_session, acquisition_reset_control, true);
	// The IO module needs a while for the reset (contrary to what the NI examples say...)
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	iostatus = NiFpga_WriteBool(_session, acquisition_reset_control, false);
}

}