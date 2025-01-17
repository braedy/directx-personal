#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <Windows.h>
#include "d3dclass.h"
#include "cameraclass.h"
//#include "colorshaderclass.h"
//#include "textureshaderclass.h"
//#include "lightshaderclass.h"
#include "lightclass.h"
#include "modelclass.h"
//#include "bitmapclass.h"
//#include "textclass.h"
//#include "modellistclass.h"
//#include "frustumclass.h"
//#include "multitexshaderclass.h"
//#include "projectionshaderclass.h"
#include "projectionlightmapshaderclass.h"
#include "textureclass.h"
#include "viewpointclass.h"

// globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 1.0f;

class GraphicsClass
{
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass *m_CubeModel, *m_GroundModel;
	//TextureShaderClass* m_TextureShader;
	//LightShaderClass* m_LightShader;
	LightClass* m_Light;
	//ColorShaderClass* m_ColorShader;
	//BitmapClass* m_Bitmap;
	//TextClass* m_Text;
	//ModelListClass* m_ModelList;
	//FrustumClass* m_Frustum;
	//MultiTexShaderClass* m_MultiTexShader;
	//ProjectionShaderClass* m_ProjectionShader;
	ProjectionLightMapShaderClass* m_ProjectionLightMapShader;
	TextureClass* m_ProjectionTexture;
	ViewPointClass* m_ViewPoint;

public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(float);// int, int, int, int, float);

	bool Render();
};

#endif