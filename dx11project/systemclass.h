#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

// preprocessing directive (removes less used win32 apis)
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "applicationclass.h"

class SystemClass{
	LPCSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hwnd;

	ApplicationClass* m_Application;

	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
};

// redirect messaging into MessageHandler
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global
static SystemClass* ApplicationHandle = 0;

#endif