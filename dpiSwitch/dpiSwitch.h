
#pragma once
#include <string>

int switchMouseAcceleration(bool turnOn, bool toggle = false);


// convert UTF-8 string to wstring
std::wstring utf8_to_wstring(const std::string& str);

// convert wstring to UTF-8 string
std::string wstring_to_utf8(const std::wstring& str);

void printError(std::string sFailingFunction);


// Format a readable error message, 
// and exit from the application.
void ErrorExit(std::string lpszFunction);