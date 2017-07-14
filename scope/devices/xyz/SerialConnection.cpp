#include "stdafx.h"
#include "SerialConnection.h"
#include "helpers\helpers.h"

namespace scope {

SerialConnection::SerialConnection(const std::wstring& _comstring, const DWORD& _baud, const BYTE& _bytesize, const BYTE& _parity, const BYTE& _stopbits)
	: status(0)
	, initialized(false) {
	hCom = CreateFile(_comstring.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,      //  must be opened with exclusive-access
		NULL,   //  default security attributes
		OPEN_EXISTING, //  must use OPEN_EXISTING
		0,      //  not overlapped I/O
		NULL ); //  hTemplate must be NULL for comm devices
	
	if (hCom == INVALID_HANDLE_VALUE)
		throw std::exception("Initializing serial connection failed");

	//  Initialize the DCB structure.
	DCB dcb;
	SecureZeroMemory(&dcb, sizeof(DCB));
	dcb.DCBlength = sizeof(DCB);

	//  Build on the current configuration by first retrieving all current
	//  settings.
   if ( !GetCommState(hCom, &dcb) )
	   throw std::exception("GetCommState of serial connection failed");

	//  Fill in some DCB values and set the com state: 
	//  9600 bps, 8 data bits, no parity, and 1 stop bit.
	dcb.BaudRate = _baud;     //  baud rate
	dcb.ByteSize = _bytesize;             //  data size, xmit and rcv
	dcb.Parity   = _parity;      //  parity bit
	dcb.StopBits = _stopbits;    //  stop bit

	if ( !SetCommState(hCom, &dcb) )
		throw std::exception("SetCommState of serial connection failed");

	// Get configuration again
	if ( !GetCommState(hCom, &dcb) )
	   throw std::exception("GetCommState of serial connection failed");

	DBOUT(L"SerialConnection::Initialize configured: " << dcb.BaudRate << L"baud " << dcb.ByteSize << L"byte size" << dcb.Parity << L"parity " << dcb.StopBits << L"stop bits");
}

SerialConnection::~SerialConnection(void) {
	CloseHandle(hCom);
}

void SerialConnection::Send(const std::string& _string) {
	//CW2A stra(_string);
	DWORD written = 0;
	if ( !WriteFile(hCom, _string.data(), static_cast<DWORD>(_string.length()), &written, NULL) )
		throw std::exception("SerialConnection::Send failed");
}

std::vector<BYTE> SerialConnection::Receive(const uint32_t& _toread, const uint32_t& _waitfor) {
	DWORD errors = 0;
	COMSTAT comstat;
	if ( !ClearCommError(hCom, &errors, &comstat) )
		throw std::exception("SerialConnection::Receive getting buffer info failed");

	if ( comstat.cbInQue < _toread ) {
		std::this_thread::sleep_for(std::chrono::milliseconds(_waitfor));
		if ( !ClearCommError(hCom, &errors, &comstat) )
			throw std::exception("SerialConnection::Receive getting buffer info failed");
	}

	if ( comstat.cbInQue < _toread )
		throw std::exception("SerialConnection::Receive not all requested bytes in buffer");

	DWORD readbytes = 0;
	std::vector<BYTE> read(_toread, 0);

	if ( !ReadFile(hCom, (void*)read.data(), comstat.cbInQue, &readbytes, NULL) )
		throw std::exception("SerialConnection::Receive read from buffer failed");

	return read;
}

}