#include "systemclass.h"

// constructor
SystemClass::SystemClass(){
	m_Application = 0;
}

// keep copy empty instead of compiler making more space than needed
SystemClass::SystemClass(const SystemClass& other){}

// ExitThread() doesnt always call deconstuctors so
// functionality will go in Shutdown function
SystemClass::~SystemClass(){}

bool SystemClass::Initialize(){
	int screenWidth = 0;
	int screenHeight = 0;
	bool result = true;

	// initialize the Windows API
	InitializeWindows(screenWidth, screenHeight);

	//create application wrapper
	m_Application = new ApplicationClass;
	if (!m_Application) return false;

	// init application wrapper
	result = m_Application->Initialize(m_hInstance, m_hwnd, screenWidth, screenHeight);
	if (!result) return false;
}

void SystemClass::Shutdown(){
	// release the application wrapper object
	if (m_Application){
		m_Application->Shutdown();
		delete m_Application;
		m_Application = 0;
	}

	// shutdown the windows
	ShutdownWindows();
}

void SystemClass::Run(){
	MSG msg;
	bool done, result;

	// initialize the message structure
	ZeroMemory(&msg, sizeof(MSG));

	// loop until quit
	done = false;
	while (!done){
		// handle window messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// if windows wants to quit
		if (msg.message == WM_QUIT){
			done = true;
		}
		else {
			// process frame
			result = Frame();
			if (!result){ 
				//MessageBox(m_hwnd, "Frame Processing Failed", "Error", MB_OK);
				done = true;
			}
		}
	}
}

bool SystemClass::Frame(){
	bool result;

	// frame processing for graphics object
	result = m_Application->Frame();
	if (!result) return false;

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
		return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight){
	WNDCLASSEX wcex;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// get an external pointed to this object
	ApplicationHandle = this;

	// get the instance of this application
	m_hInstance = GetModuleHandle(NULL);

	// give the application a name
	m_applicationName = "Engine";

	// setup the windows class with default settings
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = LoadIcon(m_hInstance, (LPCTSTR)107);
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)107);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_applicationName;
	wcex.cbSize = sizeof(WNDCLASSEX);

	// register window class
	RegisterClassEx(&wcex);

	// get resolution of screen
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// setup screen settings
	if (FULL_SCREEN){
		// set to max size of desktop
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 64; // 64-bit
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// change display settings to fullscreen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// position window top left
		posX = posY = 0;
	}
	else{
		// windowed 800x600
		screenWidth = 800;
		screenHeight = 600;

		// centre window
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// create window with settings
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hInstance, NULL);

	// show and focus window
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// hide cursor
	ShowCursor(false);
}

void SystemClass::ShutdownWindows()
{
	// show cursor
	ShowCursor(true);

	// reset display settings
	if (FULL_SCREEN) ChangeDisplaySettings(NULL, 0);

	// destroy window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// remove application instance
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;

	// release the pointer to this class
	ApplicationHandle = NULL;
}

// windows sends messages here
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// check if window is being destroyed
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// check if the window is being closed
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// pass other messages to message handler in the system class
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}
