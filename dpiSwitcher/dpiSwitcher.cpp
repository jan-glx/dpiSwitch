// dpiSwitcher.cpp : Defines the entry point for the application.
//
#include <Windows.h>
#include "stdafx.h"
#include "dpiSwitcher.h"
#include "dpiSwitch.h"
#include "DpiHelper.h"
#include <dbt.h>
#include <string>
#include <codecvt>
#include <iostream>
#include <vector>
#include <streambuf>
#include <string>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HDEVNOTIFY hDeviceNotify;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


// Custom stream buffer that redirects output to OutputDebugString
class DebugStreamBuffer : public std::wstreambuf
{
public:
	DebugStreamBuffer() = default;

protected:
	virtual int_type overflow(int_type ch)
	{
		if (ch != EOF)
		{
			buffer.push_back(static_cast<char>(ch));
			if (ch == '\n')
				FlushBuffer();
		}
		return ch;
	}

	virtual std::streamsize xsputn(const char_type* s, std::streamsize count)
	{
		buffer.append(s, s + count);
		size_t newlinePos = buffer.find('\n');
		if (newlinePos != std::wstring::npos)
			FlushBuffer();
		return count;
	}

	void FlushBuffer()
	{
		OutputDebugStringW(buffer.c_str());
		buffer.clear();
	}

private:
	std::wstring buffer;
};


int current_n_displays = 1;
const HKEY TOP_KEY = HKEY_CURRENT_USER;
const wchar_t  PATH[] = L"Software\\dpiSwitcher";
const wchar_t  REG_WITHOUT_NAME[] = L"last_dpi_scaling_without_external_monitor";
const wchar_t  REG_WITH_NAME[] = L"last_dpi_scaling_with_external_monitor";

bool writeIntToRegistry(int value, const wchar_t* valName) {
	HKEY hKey;
	DWORD dwDisposition;
	LONG lResult;

	// Create a new key or open it if it already exists
	lResult = RegCreateKeyEx(TOP_KEY, PATH, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition);
	if (lResult != ERROR_SUCCESS) {
		return false;
	}

	lResult = RegSetValueEx(hKey, valName, 0, REG_DWORD, (const BYTE*)&value, sizeof(value));
	RegCloseKey(hKey);

	return lResult == ERROR_SUCCESS;
}

bool readIntFromRegistry(int* value, const wchar_t* valName) {
	HKEY hKey;
	DWORD dwType = REG_DWORD;
	DWORD dwSize = sizeof(DWORD);
	LONG lResult;

	lResult = RegOpenKeyEx(TOP_KEY, PATH, 0, KEY_READ, &hKey);
	if (lResult != ERROR_SUCCESS) {
		return false;
	}

	lResult = RegGetValue(hKey, NULL, valName, RRF_RT_REG_DWORD, &dwType, value, &dwSize);
	RegCloseKey(hKey);

	return lResult == ERROR_SUCCESS;
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// Create the stream buffer and redirect std::wcout to it
	DebugStreamBuffer streamBuffer;
	std::wstreambuf* oldBuffer = std::wcout.rdbuf(&streamBuffer);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL haccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DPISWITCHER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	haccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(LoadString(hInstance, IDC_DPISWITCHER, szWindowClass, MAX_LOADSTRING)));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, haccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// Restore the old stream buffer
	std::wcout.rdbuf(oldBuffer);

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DPISWITCHER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_DPISWITCHER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}



//to store display info along with corresponding list item
struct DisplayData
{
	LUID m_adapterId;
	uint32_t m_sourceID;
	uint32_t m_targetID;
};

DisplayData GetInternalDisplayData()
{
	UINT32 numPathArrayElements = 0, numModeInfoArrayElements = 0;
	LONG status = GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &numPathArrayElements, &numModeInfoArrayElements);

	if (status != ERROR_SUCCESS)
	{
		std::wcout << "GetDisplayConfigBufferSizes() failed with error " << status << std::endl;
		throw std::runtime_error("Failed to get display config buffer sizes");
	}

	std::vector<DISPLAYCONFIG_PATH_INFO> pathInfos(numPathArrayElements);
	std::vector<DISPLAYCONFIG_MODE_INFO> modeInfos(numModeInfoArrayElements);

	status = QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &numPathArrayElements, pathInfos.data(), &numModeInfoArrayElements, modeInfos.data(), nullptr);
	if (status != ERROR_SUCCESS)
	{
		std::wcout << "QueryDisplayConfig() failed with error " << status << std::endl;
		throw std::runtime_error("Failed to query display config");
	}

	for (const auto& path : pathInfos)
	{
		DISPLAYCONFIG_TARGET_DEVICE_NAME deviceName = {};
		deviceName.header.size = sizeof(deviceName);
		deviceName.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_NAME;
		deviceName.header.adapterId = path.targetInfo.adapterId;
		deviceName.header.id = path.targetInfo.id;

		status = DisplayConfigGetDeviceInfo(&deviceName.header);
		if (status == ERROR_SUCCESS && deviceName.outputTechnology == DISPLAYCONFIG_OUTPUT_TECHNOLOGY_INTERNAL)
		{
			return { path.targetInfo.adapterId, path.sourceInfo.id, path.targetInfo.id };
		}
	}

	throw std::runtime_error("No internal display found");
}

int GetNumberOfDisplays()
{
	UINT32 numPathArrayElements = 0, numModeInfoArrayElements = 0;
	auto status = GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &numPathArrayElements, &numModeInfoArrayElements);

	if (status != ERROR_SUCCESS)
	{
		// Log or handle error
		std::wcout << "GetDisplayConfigBufferSizes() failed with error " << status << std::endl;
		return -1; // return an invalid value or handle the error as appropriate
	}

	return numPathArrayElements; // the number of active paths is the number of displays
}




//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   HANDLE hRecipient = hWnd;

   GUID InterfaceClassGuid = { 0xE6F07B5F, 0xEE97, 0x4a90, {0xB0, 0x76, 0x33, 0xF5, 0x7B, 0xF4, 0xEA, 0xA7} }; //GUID_DEVINTERFACE_MONITOR

   DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;

   ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
   NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
   NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
   NotificationFilter.dbcc_classguid = InterfaceClassGuid;

   hDeviceNotify = RegisterDeviceNotification(hRecipient, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);

   if (hDeviceNotify == NULL) {
	   ErrorExit("RegisterDeviceNotification Failed");
   }

   std::wcout << "Registered" << std::endl;

   return TRUE;
}

bool set_internal_display_dpi(const wchar_t* from, const wchar_t* to)
{
	DisplayData internalDisplay = GetInternalDisplayData();
	int current_scaling = DpiHelper::GetDPIScalingInfo(internalDisplay.m_adapterId, internalDisplay.m_sourceID).current;
	writeIntToRegistry(current_scaling, from);
	std::wcout << "Set scaling of internal monitor from " << current_scaling;
	readIntFromRegistry(&current_scaling, to);
	std::wcout << "% to " << current_scaling << " % " << std::endl;
	return DpiHelper::SetDPIScaling(internalDisplay.m_adapterId, internalDisplay.m_sourceID, current_scaling);
}
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		if (!UnregisterDeviceNotification(hDeviceNotify))
			ErrorExit("UnregisterDeviceNotification");
		PostQuitMessage(0);
		break;
	case WM_DEVICECHANGE:
		// Output some messages to the window.
		switch (wParam)
		{
		case DBT_DEVICEARRIVAL:
			std::wcout << "Monitor plugged in" << std::endl;
			current_n_displays = GetNumberOfDisplays();
			std::wcout << "Monitors present:" << current_n_displays << std::endl;

			if (current_n_displays == 2) {
				set_internal_display_dpi(REG_WITHOUT_NAME, REG_WITH_NAME);
			}
			break;
		case DBT_DEVICEREMOVECOMPLETE:
			std::wcout << "Monitor removed" << std::endl;
			current_n_displays = GetNumberOfDisplays();
			current_n_displays--; // GetNumberOfDisplays() seems to have some delay, so we infere that it is one less now
			std::wcout << "Monitors remaining:" << current_n_displays << std::endl;
			if(current_n_displays<=1) {
				set_internal_display_dpi(REG_WITH_NAME, REG_WITHOUT_NAME);
			}

			break;
		default:
			break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
