#pragma once

namespace scope {

/** Class around a Win32 API serial connection. Not thread-safe. */
class SerialConnection {

protected:
	/** the connection handle */
	HANDLE hCom;

	/** current status */
	BOOL status;

	/** is connection already initialized? */
	bool initialized;

public:
	/** Initializes connection */
	SerialConnection(const std::wstring& _comstring, const DWORD& _baud = CBR_9600, const BYTE& _bytesize = 8, const BYTE& _parity = NOPARITY, const BYTE& _stopbits = ONESTOPBIT);

	/** Closes connection */
	~SerialConnection(void);

	/** send command string over connection */
	void Send(const std::string& _string);

	/** Receive bytes from connection, does only wait 100ms for the bytes to be ready, otherwise throws.
	* @param[in] _toread no of bytes to read
	* @param[in] _waitfor milliseconds to wait (throws if not received all _toread bytes in time) */
	std::vector<BYTE> Receive(const uint32_t& _toread, const uint32_t& _waitfor = 100);
};

}
