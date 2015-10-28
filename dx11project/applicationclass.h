#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

//globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100.f;
const float SCREEN_NEAR = 1.0f;

//includes
#include "inputclass.h"
#include "d3dclass.h"
#include "lightclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "orthowindowclass.h"
#include "deferredbuffersclass.h"
#include "deferredshaderclass.h"
#include "deferredlightshaderclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "timerclass.h"
#include "positionclass.h"

class ApplicationClass{
	InputClass* m_Input;
	D3DClass* m_D3D;

	CameraClass* m_Camera;
	LightClass* m_Light;
	ModelClass* m_Model;

	OrthoWindowClass* m_FullScreenWindow;
	DeferredBuffersClass* m_DeferredBuffers;
	DeferredShaderClass* m_DeferredShader;
	DeferredLightShaderClass* m_DeferredLightShader;

	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	TimerClass* m_Timer;
	PositionClass* m_Position;

	bool Render();
	bool RenderSceneToTexture();

public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();
};


#endif