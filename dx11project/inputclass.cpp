#include <assert.h>
#include "inputclass.h"

InputClass::InputClass(){
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
}


InputClass::InputClass(const InputClass& other){
}


InputClass::~InputClass()
{
}


bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	//store screen size for mouse cursor positioning
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//init mouse locs
	m_mouseX = 0;
	m_mouseY = 0;

	//init main directinput interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result)) return false;

	//init directinput for keyboard
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result)) return false;
	//set data format
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result)) return false;
	//set cooperative level (i.e. do not share outside of application)
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result)) return false;
	//acquire keyboard
	result = m_keyboard->Acquire();
	if (FAILED(result)) return false;
	
	//init directinput for mouse
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result)) return false;
	//set data format
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result)) return false;
	//set cooperative level (i.e. do not share outside of application)
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result)) return false;
	//acquire mouse
	result = m_mouse->Acquire();
	if (FAILED(result)) return false;
	
	return true;
}

void InputClass::Shutdown()
{
	//release mouse
	if (m_mouse){
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	//release keyboard
	if (m_keyboard){
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	//release directinput main interface
	if (m_directInput){
		m_directInput->Release();
		m_directInput = 0;
	}
}

bool InputClass::Frame()
{
	bool result;

	//read current state of keyboard
	result = ReadKeyboard();
	if (!result) return false;

	//read current state of mouse
	result = ReadMouse();
	if (!result) return false;

	//process changes in the mouse and keyboard.
	ProcessInput();

	return true;
}

bool InputClass::ReadKeyboard(){
	HRESULT result;

	//read the keyboard device
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	
	//if the keyboard lost focus or was not acquired, try to get control back
	if (FAILED(result)){
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)){
			m_keyboard->Acquire();
		}
		else{
			return false;
		}
	}

	return true;
}

bool InputClass::ReadMouse(){
	HRESULT result;

	//read the mouse device
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);

	//if the mouse lost focus or was not acquired, try to get control back
	if (FAILED(result)){
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)){
			m_mouse->Acquire();
		}
		else{
			return false;
		}
	}

	return true;
}

void InputClass::ProcessInput(){
	//update the location of the mouse cursor based on change
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	//clamp to screen
	if (m_mouseX < 0) m_mouseX = 0;
	if (m_mouseY < 0) m_mouseY = 0;
	if (m_mouseX > m_screenWidth) m_mouseX = m_screenWidth;
	if (m_mouseY > m_screenHeight) m_mouseY = m_screenHeight;
}

bool InputClass::IsEscapePressed(){
	// check if escape key is being pressed
	if (m_keyboardState[DIK_ESCAPE] & 0x80) return true;

	return false;
}

bool InputClass::IsLeftArrowPressed(){
	// check if left arrow key is being pressed
	if (m_keyboardState[DIK_LEFT] & 0x80) return true;

	return false;
}

bool InputClass::IsRightArrowPressed(){
	// check if left arrow key is being pressed
	if (m_keyboardState[DIK_RIGHT] & 0x80) return true;
	return false;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY){
	mouseX = m_mouseX;
	mouseY = m_mouseY;
}

bool InputClass::IsUpArrowPressed(){
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_UP] & 0x80) return true;

	return false;
}

bool InputClass::IsDownArrowPressed(){
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_DOWN] & 0x80) return true;

	return false;
}

bool InputClass::IsAPressed(){
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_A] & 0x80) return true;

	return false;
}

bool InputClass::IsDPressed(){
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_D] & 0x80) return true;

	return false;
}

bool InputClass::IsWPressed(){
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_W] & 0x80) return true;

	return false;
}

bool InputClass::IsSPressed(){
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_S] & 0x80) return true;

	return false;
}