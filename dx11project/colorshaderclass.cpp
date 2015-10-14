#include "colorshaderclass.h"

// intialize pointers
ColorShaderClass::ColorShaderClass(){
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass& other){
}

ColorShaderClass::~ColorShaderClass(){
}

bool ColorShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// init vertex and pixel shaders
	result = InitializeShader(device, hwnd, L"VertexShader.hlsl", L"PixelShader.hlsl");
	if (!result){ return false; }

	return true;
}

// shutdown vertex and pixel shaders and all related objects
void ColorShaderClass::Shutdown(){
	ShutdownShader();
}

bool ColorShaderClass::Render(
	ID3D11DeviceContext* deviceContext,
	int indexCount,
	XMMATRIX &worldMatrix,
	XMMATRIX &viewMatrix,
	XMMATRIX &projectionMatrix)
{
	bool result;

	// set shader parameters for rendering
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result){ return false; }

	// render prepared buffers with the shader
	RenderShader(deviceContext, indexCount);

	return true;
}

bool ColorShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;


	// init pointers this function will use to null
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// compile the vertex shader code.
	result = D3DCompileFromFile(
		vsFilename,
		NULL,
		NULL, 
		"ColorVertexShader", 
		"vs_5_0", 
		D3D10_SHADER_ENABLE_STRICTNESS, 
		0, 
		&vertexShaderBuffer,
		&errorMessage
		);
	if (FAILED(result))
	{
		// write error message if shader does not compile
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// if no error msg - file not found
		else
		{
			MessageBox(hwnd, (char*)vsFilename, "Missing Shader File", MB_OK);
		}

		return false;
	}

	// compile the pixel shader code.
	result = D3DCompileFromFile(
		psFilename,
		NULL,
		NULL,
		"ColorPixelShader",
		"ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&pixelShaderBuffer,
		&errorMessage);
	if (FAILED(result))
	{
		// write error message if shader does not compile
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// if no error msg - file not found
		else
		{
			MessageBox(hwnd, (char*)psFilename, "Missing Shader File", MB_OK);
		}

		return false;
	}

	// create vertex shader from buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result)){ return false; }

	// create pixel shader from buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result)){ return false; }

	// setup data layout going into the shader
	// NB: needs to match the VertexType stucture in the ModelClass and in the shader
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// get a count of the elements in the layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// create the vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result)){ return false; }

	// release vertex and pixel shader buffers since they are no longer needed
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// setup description of the dynamic matrix constant buffer in vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result)){ return false; }

	return true;
}

void ColorShaderClass::ShutdownShader()
{
	// release the matrix constant buffer
	if (m_matrixBuffer){
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// release the layout
	if (m_layout){
		m_layout->Release();
		m_layout = 0;
	}

	// release pixel shader
	if (m_pixelShader){
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// release vertex shader
	if (m_vertexShader){
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename){
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// get a pointer to the error message text buffer
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// get length of message.
	bufferSize = errorMessage->GetBufferSize();

	// open a file to write error message to
	fout.open("shader-error.txt");

	// write out error message
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// close file
	fout.close();

	// release error message.
	errorMessage->Release();
	errorMessage = 0;

	// popup msg notify the user to check the text file for compile errors
	MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", (char*)shaderFilename, MB_OK);
}

// for setting globals easier
bool ColorShaderClass::SetShaderParameters(
	ID3D11DeviceContext* deviceContext,
	XMMATRIX &worldMatrix,
	XMMATRIX &viewMatrix,
	XMMATRIX &projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// transpose matrices to prepare them for the shader
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// lock the constant buffer so it can be written to
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)){ return false; }

	// get a pointer to data in the constant buffer
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// copy the matrices into the constant buffer
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// unlock the constant buffer
	deviceContext->Unmap(m_matrixBuffer, 0);

	// set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	// finally, set the constant buffer in the vertex shader with updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;
}

// render
void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// set the vertex input layout
	deviceContext->IASetInputLayout(m_layout);

	// set the vertex and pixel shaders that will be used to render this triangle
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// render the triangle
	deviceContext->DrawIndexed(indexCount, 0, 0);
}