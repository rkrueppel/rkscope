#include "stdafx.h"
#include "helpers.h"

std::wstring GetCurrentDateString() {
	SYSTEMTIME st;
	GetLocalTime(&st);
	std::wstringstream stream;
	stream << std::setfill(L'0') << st.wYear << L"-" <<  std::setw(2) << st.wMonth << L"-" << std::setw(2) << st.wDay;
	return stream.str();
}

std::wstring GetCurrentTimeString(const bool& _filenamecompatible) {
	SYSTEMTIME st;
	GetLocalTime(&st);
	std::wstringstream stream;
	if ( _filenamecompatible )			// ISO standard says HH:MM:SS, but no : allowed in filenames
		stream << std::setfill(L'0') << std::setw(2) << st.wHour << L"-" << std::setw(2) << st.wMinute << L"-" << std::setw(2) << st.wSecond;
	else
		stream << std::setfill(L'0') << std::setw(2) << st.wHour << L":" << std::setw(2) << st.wMinute << L":" << std::setw(2) << st.wSecond;
	return stream.str();
}
