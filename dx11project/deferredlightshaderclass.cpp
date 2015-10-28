#include "deferredlightshaderclass.h"

DeferredLightShaderClass::DeferredLightShaderClass(){
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_sampleState = 0;
	m_matrixBuffer = 0;
	m_lightBuffer = 0;
}


DeferredLightShaderClass::DeferredLightShaderClass(const DeferredLightShaderClass& other){
}


DeferredLightShaderClass::~DeferredLightShaderClass(){
}


bool DeferredLightShaderClass::Initialize(ID3D11Device* device, HWND hwnd){
	bool result;

	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"deferred_light_vert.hlsl", L"deferred_light_pix.hlsl");
	if (!result) return false;

	return true;
}

void DeferredLightShaderClass::Shutdown(){
	//shutdown vs, ps and related
	ShutdownShader();
}

bool DeferredLightShaderClass::Render(ID3D11DeviceContext* deviceContext,
	int indexCount,
	XMMATRIX &worldMatrix,
	XMMATRIX &viewMatrix,
	XMMATRIX &projectionMatrix,
	ID3D11ShaderResourceView* colourTexture,
	ID3D11ShaderResourceView* normalTexture,
	XMFLOAT3 &lightDirection)
{
	bool result;

	//set shader params for rendering
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, colourTexture, normalTexture, lightDirection);
	if (!result) return false;

	//render prepped buffers with shader
	RenderShader(deviceContext, indexCount);

	return true;
}

bool DeferredLightShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename){
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	//init pointers
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;


	//compile vertex shader
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result)){
		if (errorMessage){
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		else{
			MessageBox(hwnd, (char*)vsFilename, "Missing Shader File", MB_OK);
		}
		return false;
	}

	//compile pixel shader
	result = D3DCompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result)){
		if (errorMessage){
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		else{
			MessageBox(hwnd, (char*)psFilename, "Missing Shader File", MB_OK);
		}
		return false;
	}

	//create vertex shader
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result)) return false;

	//create pixel shader
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result)) return false;

	//create vertex input layout desc
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	//num elements in layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//create vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result)) return false;

	//release vertex and pixel shader buffers
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	//create tex sampler state desc
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//create texture sampler state
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result)) return false;

	//setup desc of synamic matrix constant buffer in vs
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//create constant buffer pointer to access vs constant buffer
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result)) return false;

	//setup desc of light dynamic constant 
	//NB: ByteWidth NEEDS TO BE multiple of 16 iff D3D11_BIND_CONSTANT_BUFFER else CreateBuffer will fail
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	//create constant buffer pointer to access vs constant buffer
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result)) return false;

	return true;
}

void DeferredLightShaderClass::ShutdownShader(){
	// Release the light constant buffer.
	if (m_lightBuffer)	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	// Release the matrix constant buffer.
	if (m_matrixBuffer)	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the sampler state.
	if (m_sampleState){
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the layout.
	if (m_layout)	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if (m_pixelShader)	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if (m_vertexShader)	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}

void DeferredLightShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename){
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	//pointer to error message text buffer
	compileErrors = (char*)(errorMessage->GetBufferPointer());
	//get length of message
	bufferSize = errorMessage->GetBufferSize();
	//open file to write to
	fout.open("shader-error.txt");
	//write out error
	for (i = 0; i < bufferSize; i++){
		fout << compileErrors[i];
	}
	//close file
	fout.close();

	//release
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", (char*)shaderFilename, MB_OK);
}

bool DeferredLightShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX &worldMatrix, XMMATRIX &viewMatrix,
	XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* colourTexture, ID3D11ShaderResourceView* normalTexture, XMFLOAT3 &lightDirection){

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;

	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	

	// Set shader texture resources in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &colourTexture);
	deviceContext->PSSetShaderResources(1, 1, &normalTexture);

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	dataPtr2->lightDirection = lightDirection;
	dataPtr2->padding = 0.0f;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	return true;
}

void DeferredLightShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}