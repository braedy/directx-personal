#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_CubeModel = 0;
	m_GroundModel = 0;
	//m_TextureShader = 0;
	//m_LightShader = 0;
	//m_Light = 0;
	//m_ColorShader = 0;
	//m_Bitmap = 0;
	//m_Text = 0;
	//m_ModelList = 0;
	//m_Frustum = 0;
	//m_MultiTexShader = 0;
	m_ProjectionLightMapShader = 0;
	m_ProjectionTexture = 0;
	m_ViewPoint = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	//NEW VARIABLE
	XMMATRIX baseViewMatrix;
	

	// create D3D object
	m_D3D = new D3DClass;
	if (!m_D3D){ return false; }

	// initialize D3D object
	result = m_D3D->Initialize(
		screenWidth,
		screenHeight,
		VSYNC_ENABLED,
		hwnd,
		FULL_SCREEN,
		SCREEN_DEPTH,
		SCREEN_NEAR);
	if (!result){
		MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}

	// create camera object.
	m_Camera = new CameraClass;
	if (!m_Camera){	return false; }

	// set initial camera position
	m_Camera->SetPosition(0.0f, 7.0f, -10.0f);
	m_Camera->SetRotation(35.0f, 0.0f, 0.0f);

	//m_Camera->Render();
	//m_Camera->GetViewMatrix(baseViewMatrix);

	//create text
	//m_Text = new TextClass;
	//if (!m_Text) return false;

	////init text obj
	//result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	//if (!result){
	//	MessageBox(hwnd, "Could not initialize the text object.", "Error", MB_OK);
	//	return false;
	//}

	//create ground
	m_GroundModel = new ModelClass;
	if (!m_GroundModel) return false;
	//init ground
	result = m_GroundModel->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), "floor.txt", "concrete.tga");
	if (!result){
		MessageBox(hwnd, "Could not initialize the ground model object.", "Error", MB_OK);
		return false;
	}

	//create cube
	m_CubeModel = new ModelClass;
	if (!m_CubeModel) return false;
	//init cube
	result = m_CubeModel->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), "head.txt", "sand.tga");
	if (!result){
		MessageBox(hwnd, "Could not initialize the cube model object.", "Error", MB_OK);
		return false;
	}
	
	// create the model object
	//m_Model = new ModelClass;
	//if (!m_Model) return false;

	//// initialize model object.
	//result = m_Model->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), "square.txt", "blackwater.tga", "pebbles.tga");
	//if (!result){
	//	MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
	//	return false;
	//}

	// create color shader object
	//m_ColorShader = new ColorShaderClass;
	//if (!m_ColorShader){ return false; }

	// initialize color shader object
	//result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	//if (!result){
	//	MessageBox(hwnd, "Could not initialize the color shader object.", "Error", MB_OK);
	//	return false;
	//}

	// Create the texture shader object.
	//m_TextureShader = new TextureShaderClass;
	//if (!m_TextureShader){
	//	return false;
	//}

	//// Initialize the texture shader object.
	//result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	//if (!result){
	//	MessageBox(hwnd, "Could not initialize the texture shader object.", "Error", MB_OK);
	//	return false;
	//}
	/*
	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap) return false;

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), screenWidth, screenHeight, "boattex.tga", 512, 256);
	if (!result){
		MessageBox(hwnd, "Could not initialize the bitmap object.", "Error", MB_OK);
		return false;
	}*/

	//create multi tex shader object
	//m_MultiTexShader = new MultiTexShaderClass;
	//if (!m_MultiTexShader) return false;

	////init multi tex shader object
	//result = m_MultiTexShader->Initialize(m_D3D->GetDevice(), hwnd);
	//if (!result){
	//	MessageBox(hwnd, "Could not initialize the multi tex shader object.", "Error", MB_OK);
	//	return false;
	//}
	
	////create light shader object
	//m_LightShader = new LightShaderClass;
	//if (!m_LightShader) return false;

	////init light shader object
	//result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	//if (!result){
	//	MessageBox(hwnd, "Could not initialize the light shader object.", "Error", MB_OK);
	//	return false;
	//}

	//create light  object
	m_Light = new LightClass;
	if (!m_Light) return false;

	//init light object
	m_Light->SetAmbientColour(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetPosition(2.0f, 5.0f, -2.0f);
	//m_Light->SetDirection(0.0f, -0.75f, 0.5f);
	//m_Light->SetSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	//m_Light->SetSpecularPower(32.0f);

	//create model list object
	//m_ModelList = new ModelListClass;
	//if (!m_ModelList) return false;
	//
	////init model list object
	//result = m_ModelList->Initialize(30);
	//if (!result){
	//	MessageBox(hwnd, "Could not initialize the model list object.", "Error", MB_OK);
	//	return false;
	//}

	////create frustum object
	//m_Frustum = new FrustumClass;
	//if (!m_Frustum) return false;

	//create projection shader object
	//m_ProjectionShader = new ProjectionShaderClass;
	//if (!m_ProjectionShader) return false;

	////init projection shader object
	//result = m_ProjectionShader->Initialize(m_D3D->GetDevice(), hwnd);
	//if (!result){
	//	MessageBox(hwnd, "Could not initialize the projection shader object.", "Error", MB_OK);
	//	return false;
	//}

	//create projection light map shader object
	m_ProjectionLightMapShader = new ProjectionLightMapShaderClass;
	if (!m_ProjectionLightMapShader) return false;

	//init projection shader object
	result = m_ProjectionLightMapShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result){
		MessageBox(hwnd, "Could not initialize the projection shader object.", "Error", MB_OK);
		return false;
	}

	//create projection texture object
	m_ProjectionTexture = new TextureClass;
	if (!m_ProjectionTexture) return false;

	//init projection texture object
	result = m_ProjectionTexture->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), "grate.tga");
	if (!result){
		MessageBox(hwnd, "Could not initialize the projection texture object.", "Error", MB_OK);
		return false;
	}

	//create view point object
	m_ViewPoint = new ViewPointClass;
	if (!m_ViewPoint) return false;
	
	// Initialize the view point object.
	m_ViewPoint->SetPosition(-0.5f, 6.0f, -3.0f);
	m_ViewPoint->SetLookAt(0.0f, 0.0f, 0.0f);
	m_ViewPoint->SetProjectionParameters((float)(XM_PI / 2.0f), 1.0f, 0.1f, 100.0f);
	m_ViewPoint->GenerateViewMatrix();
	m_ViewPoint->GenerateProjectionMatrix();

	return true;
}


void GraphicsClass::Shutdown(){
	//release view point object
	if (m_ViewPoint){
		delete m_ViewPoint;
		m_ViewPoint = 0;
	}

	//release projection texture object
	if (m_ProjectionTexture){
		m_ProjectionTexture->Shutdown();
		delete m_ProjectionTexture;
		m_ProjectionTexture = 0;
	}

	//release projection shader object
	if (m_ProjectionLightMapShader){
		m_ProjectionLightMapShader->Shutdown();
		delete m_ProjectionLightMapShader;
		m_ProjectionLightMapShader = 0;
	}

	//release cube model
	if (m_CubeModel){
		m_CubeModel->Shutdown();
		delete m_CubeModel;
		m_CubeModel = 0;
	}

	//release the ground model object.
	if (m_GroundModel){
		m_GroundModel->Shutdown();
		delete m_GroundModel;
		m_GroundModel = 0;
	}

	//release the multitexture shader
	/*if (m_MultiTexShader){
		m_MultiTexShader->Shutdown();
		delete m_MultiTexShader;
		m_MultiTexShader = 0;
	}*/

	//release frustum object
	/*if (m_Frustum){
		delete m_Frustum;
		m_Frustum = 0;
	}*/

	//release model list object
	/*if (m_ModelList){
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = 0;
	}*/


	// Release the bitmap object.
	/*if (m_Bitmap){
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}*/

	// release color shader object
	/*if (m_ColorShader){
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}*/
	//release the light object
	if (m_Light){
		delete m_Light;
		m_Light = 0;
	}

	//release the light shader object
	/*if (m_LightShader){
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}*/

	//release the texture shader object
	/*if (m_TextureShader){
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}*/

	// release the model object
	/*if (m_Model){
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}*/

	//release text object
	/*if (m_Text){
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}*/

	// release the camera object
	if (m_Camera){
		delete m_Camera;
		m_Camera = 0;
	}

	// clear up D3D object
	if (!m_D3D){
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
}


bool GraphicsClass::Frame(float rotationY)//int mouseX, int mouseY, int fps, int cpu, float frameTime)
{
	//bool result;
	//static float rotation = 0.0f;

	// set fps
	//result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	//if (!result) return false;

	// set cpu
	//result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	//if (!result) return false;

	// set location of mouse
	//result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	//if (!result) return false;

	//update rotation variable
	//rotation += (float)XM_PI * 0.005f;
	//if (rotation > 360.0f) rotation -= 360.0f;

	// Set the position of the camera.
	m_Camera->SetPosition(2.0f, 7.0f, -12.5f);

	// Set the rotation of the camera.
	m_Camera->SetRotation(-10.0f, rotationY, 0.0f);

	//m_Light->SetDirection(0.0f, 0.0f, 1.0f);

	return true;
}

// important calls
bool GraphicsClass::Render()
{
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix,
		projTexViewMatrix, projTexProjectionMatrix;
	int modelCount, renderCount, index;
	float positionX, positionY, positionZ, radius;
	XMFLOAT4 colour;
	bool renderModel, result;

	// clear buffers to begin the scene
	// CLEAR COLOUR
	m_D3D->BeginScene(0.05f, 0.0f, 0.1f, 1.0f);

	// generate view matrix based on camera position
	m_Camera->Render();

	// get the world, view, projection, ortho matrices from the camera and d3d objects
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// get viewpoint matrices
	m_ViewPoint->GetViewMatrix(projTexViewMatrix);
	m_ViewPoint->GetProjectionMatrix(projTexProjectionMatrix);

	// set up translation for the ground model
	worldMatrix = XMMatrixTranslation(0.0f, 1.0f, 0.0f);

	// render ground model with projection shader
	m_GroundModel->Render(m_D3D->GetDeviceContext());

	result = m_ProjectionLightMapShader->Render(
		m_D3D->GetDeviceContext(),
		m_GroundModel->GetIndexCount(), 
		worldMatrix, 
		viewMatrix, 
		projectionMatrix,
		m_GroundModel->GetTexture(), 
		m_Light->GetAmbientColour(),
		m_Light->GetDiffuseColour(),
		m_Light->GetPosition(),
		projTexViewMatrix,
		projTexProjectionMatrix,
		m_ProjectionTexture->GetTexture());
	if (!result) return false;

	//reset world matrix and setup translation for cube model
	m_D3D->GetWorldMatrix(worldMatrix);
	//worldMatrix = XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	worldMatrix = XMMatrixScaling(0.011, 0.011, 0.011) * XMMatrixTranslation(0.0f, 2.0f, -1.25f);

	//render cube model with projection shader
	m_CubeModel->Render(m_D3D->GetDeviceContext());
	result = m_ProjectionLightMapShader->Render(
		m_D3D->GetDeviceContext(),
		m_CubeModel->GetIndexCount(),
		worldMatrix,
		viewMatrix,
		projectionMatrix,
		m_CubeModel->GetTexture(),
		m_Light->GetAmbientColour(),
		m_Light->GetDiffuseColour(),
		m_Light->GetPosition(),
		projTexViewMatrix,
		projTexProjectionMatrix,
		m_ProjectionTexture->GetTexture());
	if (!result) return false;


	/*//construct frustum;
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	//get no of models
	modelCount = m_ModelList->GetModelCount();
	//init the count of models that have been rendered
	renderCount = 0;
	
	//render models only if seen by camera view
	for (index = 0; index < modelCount; index++){
		//get position & colour of sphere model at index
		m_ModelList->GetData(index, positionX, positionY, positionZ, colour);
		//set radius of sphere 
		radius = 1.0f;
		//check is sphere is in view
		renderModel = m_Frustum->CheckSphere(positionX, positionY, positionZ, radius);
		//if can be seen
		if (renderModel){
			//move model to render location
			worldMatrix = XMMatrixTranslation(positionX, positionY, positionZ);

			//put model vertex and index buffers on the  graphics pipeline
			m_Model->Render(m_D3D->GetDeviceContext());

			//render model using light shader
			m_Light->SetDiffuseColour(colour.x, colour.y, colour.z, colour.w);
			m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColour(), m_Light->GetDiffuseColour(), m_Camera->GetPosition(), m_Light->GetSpecularColour(), m_Light->GetSpecularPower());

			//reset world matrix
			m_D3D->GetWorldMatrix(worldMatrix);

			//increase rendered object count
			renderCount++;
		}
	}

	//set the number of models that were actually rendered this frame.
	result = m_Text->SetRenderCount(renderCount, m_D3D->GetDeviceContext());
	if (!result) return false;*/

	// put model vertex and index buffers on the graphics pipeline to prepare them for drawing
	//m_Model->Render(m_D3D->GetDeviceContext());

	//rotate world
	//worldMatrix = XMMatrixRotationY(rotation);

	// Render the model using the color shader
	/*result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result){
		return false;
	}*/

	//render the model using the texture shader
	/*result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());
	if (!result){
		return false;
	}*/

	//render the model using the light shader
	/*result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColour(), m_Light->GetDiffuseColour(), m_Camera->GetPosition(), m_Light->GetSpecularColour(), m_Light->GetSpecularPower());
	if (!result) return false;*/


	//turn off z buffer (2d)
	/*m_D3D->TurnZBufferOff();
	//turn on alpha
	m_D3D->TurnAlphaBlendingOn();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result) return false;*/

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 100, 100);
	//if (!result) return false;

	// Render the bitmap with the texture shader.
	//result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	//if (!result) return false;

	//Turn off alpha after render
	/*m_D3D->TurnAlphaBlendingOff();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();*/

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//m_Model->Render(m_D3D->GetDeviceContext());

	//// Render the model using the multitexture shader.
	//m_MultiTexShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	//	m_Model->GetTextureArray());

	// present the rendered scene to the screen
	m_D3D->EndScene();

	return true;
}
