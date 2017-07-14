#pragma once

#include "devices/fpga/NiFpga.h"
#include "helpers.h"

namespace scope {

/** An exception for FPGA stuff */
class FPGAException
	: public std::exception {

public:
	/** @param[in] _status the FPGA status */
	FPGAException(const int32_t& _status)
		: std::exception()
		, fpga_status(_status) {
	}

	/** @param[in] _status the FPGA status
	* @param[in] _msg error message */
	FPGAException(const int32_t& _status, const char* const _msg)
		: std::exception(_msg)
		, fpga_status(_status) {
	}

	/** the FPGA status */
	const int32_t fpga_status;
};

/** Helper class to check the FPGA status value returned from NI FPGA C API */
struct FPGAStatusChecker {
	/** @return false if status is an error */
	static bool Check(const NiFpga_Status& _st) {
		if(NiFpga_IsError(_st))
			return false;
		return true;
	}
};

/** Wrapper class for the FPGA status, templatized for throwing or non-throwing */
template<bool ThrowException>
class FPGAStatusT {

protected:
	/** the FPGA status */
	NiFpga_Status status;

	/** status message */
	CString fpga_message;
	
	/** Set new status and check/throw if ThrowException */
	void Assign(const NiFpga_Status& _st) {
		status = _st;
		switch (status ) {
		case NiFpga_Status_FifoTimeout:
			fpga_message = L"FIFO timeout";
			break;
		case NiFpga_Status_InvalidParameter:
			fpga_message = L"Invalid parameter";
			break;
		case NiFpga_Status_ResourceNotFound:
			fpga_message = L"Resource not found";
			break;
		case NiFpga_Status_SignatureMismatch:
			fpga_message = L"Bitfile signature mismatch";
			break;
		case NiFpga_Status_IncompatibleBitfile:
			fpga_message = L"Bitfile incompatible";
			break;
		case NiFpga_Status_InvalidResourceName:
			fpga_message = L"Invalid resource name";
			break;
		case NiFpga_Status_BitfileReadError:
			fpga_message = L"Bitfile read error";
			break;
		case NiFpga_Status_CommunicationTimeout:
			fpga_message = L"Communication between FPGA and host timed out";
			break;
		case NiFpga_Status_BadReadWriteCount:
			fpga_message = L"The number of FIFO elements is invalid. Either the number is greater than the depth of the host memory DMA FIFO, or more elements were requested for";
			break;
		default:
			fpga_message = L"Unknown";
		}

		if( !FPGAStatusChecker::Check(status) ) {
			DBOUT(L"FPGAException " << status << L" " << fpga_message.GetString());
		}

		if( !FPGAStatusChecker::Check(status) && ThrowException )
			throw FPGAException(status);
	}

public:
	/** Standard constructor */
	FPGAStatusT(const NiFpga_Status& _st = NiFpga_Status_Success)
		: fpga_message(L"") {
		Assign(_st);
	}

	/** Assignment operator */
	FPGAStatusT& operator= (const NiFpga_Status& _st) {
		Assign(_st);
		return *this;
	}

	/** @return status */
	operator NiFpga_Status () const {
		return status;
	}

	/** @return true if not an error status */
	bool Success() const {
		return FPGAStatusChecker::Check(status);
	}
};

/** An FPGAStatus that throws */
typedef FPGAStatusT<true> FPGAStatus;

/** An FPGAStatus that does never throw */
typedef FPGAStatusT<false> FPGAStatusSafe;

}