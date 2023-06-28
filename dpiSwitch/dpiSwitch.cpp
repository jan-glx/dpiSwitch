// dpiSwitch.cpp : Defines the entry point for the console application.
// Credits to Cody Gray @ http://stackoverflow.com/a/9052357/1870254
//

#include "stdafx.h"
#include "dpiSwitch.h"
#include "windows.h"
#include <string>
#include <iostream>
#include <codecvt>

// convert UTF-8 string to wstring
std::wstring utf8_to_wstring(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.from_bytes(str);
}

// convert wstring to UTF-8 string
std::string wstring_to_utf8(const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}

void printError(std::string sFailingFunction)
{
	LPWSTR lpMsgBuf;
	DWORD dwErrorCode = ::GetLastError();

	size_t size = FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf,
		0, NULL);
	std::string message = wstring_to_utf8(std::wstring(lpMsgBuf));
	HeapFree(GetProcessHeap(), 0, lpMsgBuf); //	LocalFree(lpMsgBuf);

	std::cerr << sFailingFunction << "failed with error " << dwErrorCode << ": " << message << std::endl;

}

void ErrorExit(std::string lpszFunction)
// Format a readable error message, 
// and exit from the application.
{
	printError(lpszFunction);
	ExitProcess(1);
}

int switchMouseAcceleration(bool turnOn, bool toggle)
{
	int pvParam[3];
	// Get the current values.
	if (!SystemParametersInfoW(SPI_GETMOUSE, 0, pvParam, 0))
		return EXIT_FAILURE;

	// Modify the dpieration value as directed.
	pvParam[2] = toggle ? !pvParam[2] : turnOn;

	// Update the system setting.
	if (!SystemParametersInfoW(SPI_SETMOUSE, 0, pvParam, SPIF_SENDCHANGE))
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

#ifndef LIB
int main(int argc, char* argv[])
{
	bool toggle=false, turnOn=false;

	if (argc == 1 || (argc == 2 && !strcmp(argv[1], "toggle")))
		toggle = true;
	else if (argc == 2 && !strcmp(argv[1], "on"))
		turnOn = true;
	else if (argc == 2 && !strcmp(argv[1], "off"))
		;
	else {
		std::cerr << "Usage: " << argv[0] << " [on|off|toggle]\nNo argument will toggle mouse dpieration / enhance pointer precision." << std::endl;
		return EXIT_FAILURE;
	}
	if (EXIT_FAILURE==switchMouseAcceleration(turnOn, toggle))
		ErrorExit("SPI_SETMOUSE or SPI_GETMOUSE");
	return EXIT_SUCCESS;
}
#endif

