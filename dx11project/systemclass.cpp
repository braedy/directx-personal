#include "systemclass.h"

// constructor
SystemClass::SystemClass(){
	m_Input = 0;
	m_Graphics = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_Timer = 0;
	m_Position = 0;
}

// keep copy empty instead of compiler making more space than needed
SystemClass::SystemClass(const SystemClass& other){
}

// ExitThread() doesnt always call deconstuctors so
// functionality will go in Shutdown function
SystemClass::~SystemClass(){
}

bool SystemClass::Initialize(){
	int screenWidth = 0;
	int screenHeight = 0;
	bool result = true;

	// initialize the Windows API
	InitializeWindows(screenWidth, screenHeight);

	// create input object to handle keyboard input
	m_Input = new InputClass;
	if (!m_Input) return false;
	// intialize input object
	m_Input->Initialize(m_hInstance, m_hwnd, screenWidth, screenHeight);
	if (!result){
		MessageBox(m_hwnd, "Could not initialize the input object.", "Error", MB_OK);
		return false;
	}

	// create graphics object to handle graphics
	m_Graphics = new GraphicsClass;
	if (!m_Graphics) return false;
	// initialize graphics object
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result) return false;

	// create fps object for frame rate checking
	m_Fps = new FpsClass;
	if (!m_Fps) return false;
	// initialize fps object
	m_Fps->Initialize();

	// create cpu object for process checking
	m_Cpu = new CpuClass;
	if (!m_Cpu) return false;
	// initialize cpu object
	m_Cpu->Initialize();

	// create timer object
	m_Timer = new TimerClass;
	if (!m_Timer) return false;
	// initialize timer object
	result = m_Timer->Initialize();
	if (!result){
		MessageBox(m_hwnd, "Could not initialize the Timer object.", "Error", MB_OK);
		return false;
	}

	//create position object
	m_Position = new PositionClass;
	if (!m_Position) return false;

	return true;
}

void SystemClass::Shutdown(){
	// release position object
	if (m_Position){
		delete m_Position;
		m_Position = 0;
	}

	// release timer object
	if (m_Timer){
		delete m_Timer;
		m_Timer = 0;
	}

	// release cpu object
	if (m_Cpu){
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// release fps object
	if (m_Fps){
		delete m_Fps;
		m_Fps = 0;
	}
	
	// release graphics object
	if (m_Graphics){
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// release the input object
	if (m_Input){
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
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
				MessageBox(m_hwnd, "Frame Processing Failed", "Error", MB_OK);
				done = true;
			}
		}

		//check if user pressed esc to quit
		if (m_Input->IsEscapePressed()) done = true;
	}
}

bool SystemClass::Frame(){
	bool result, keyDown;
	int mouseX, mouseY;
	float rotationY;

	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// frame processing for input object
	result = m_Input->Frame();
	if (!result) return false;

	// get loc of mouse
	m_Input->GetMouseLocation(mouseX, mouseY);
	
	//set frame time
	m_Position->SetFrameTime(m_Timer->GetTime());

	//check keys
	keyDown = m_Input->IsLeftArrowPressed();
	m_Position->TurnLeft(keyDown);
	keyDown = m_Input->IsRightArrowPressed();
	m_Position->TurnRight(keyDown);

	//get current view point rotation
	m_Position->GetRotation(rotationY);

	// frame processing for graphics object
	result = m_Graphics->Frame(rotationY);//mouseX, mouseY, m_Fps->GetFps(), m_Cpu->GetCpuPercentage(), m_Timer->GetTime());
	if (!result) return false;

	// render graphics scene
	result = m_Graphics->Render();
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
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

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
