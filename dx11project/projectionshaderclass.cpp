#include "projectionshaderclass.h"

ProjectionShaderClass::ProjectionShaderClass(){
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_sampleState = 0;
	m_matrixBuffer = 0;
	m_lightBuffer = 0;
}

ProjectionShaderClass::ProjectionShaderClass(const ProjectionShaderClass& other){
}

ProjectionShaderClass::~ProjectionShaderClass(){
}

//Load the projection HLSL files.

bool ProjectionShaderClass::Initialize(ID3D11Device* device, HWND hwnd){
	bool result;

	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"projection_vert.hlsl", L"projection_pix.hlsl");
	if (!result) return false;
	
	return true;
}

void ProjectionShaderClass::Shutdown(){
	//shutdown vs, ps and related
	ShutdownShader();
}

bool ProjectionShaderClass::Render(
	ID3D11DeviceContext* deviceContext,
	int indexCount,
	XMMATRIX &worldMatrix,
	XMMATRIX &viewMatrix,
	XMMATRIX &projectionMatrix,
	ID3D11ShaderResourceView* texture,
	XMFLOAT4 &ambientColour,
	XMFLOAT4 &diffuseColour,
	XMFLOAT3 &lightDirection,
	XMMATRIX &projTexViewMatrix,
	XMMATRIX &projTexProjectionMatrix,
	ID3D11ShaderResourceView* projectionTexture){
	bool result;

	//set shader params
	result = SetShaderParameters(
		deviceContext,
		worldMatrix,
		viewMatrix,
		projectionMatrix,
		texture,
		ambientColour,
		diffuseColour,
		lightDirection,
		projTexViewMatrix,
		projTexProjectionMatrix,
		projectionTexture);
	if (!result) return false;

	//render prepped buffers with shader
	RenderShader(deviceContext, indexCount);

	return true;
}

bool ProjectionShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename){
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	//init pointers
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//compile vs
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "ProjectionVertexShader", "vs_5_0",
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
	//compile ps
	result = D3DCompileFromFile(psFilename, NULL, NULL, "ProjectionPixelShader", "ps_5_0",
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

	//create vs from buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result)) return false;
	//create ps from buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result)) return false;

	//create the vertex input layout description.
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

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	//number of elements in layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//create vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_layout);
	if (FAILED(result)) return false;

	//release vs, ps buffers
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	//create texture sampler state desc
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

	//create the texture sampler state
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result)) return false;

	//setup the dynamic matrix constant buffer desc in vs
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//create constant buffer pointer to access vs constant buffer
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result)) return false;

	//setup light dynamic constant buffer desc in ps
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	//create constant buffer pointer to access ps constant buffer
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result)) return false;

	return true;
}

void ProjectionShaderClass::ShutdownShader(){
	//release light constant buffer
	if (m_lightBuffer){
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}
	//release matrix constant buffer
	if (m_matrixBuffer){
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}
	//release sampler state
	if (m_sampleState){
		m_sampleState->Release();
		m_sampleState = 0;
	}
	//release layout
	if (m_layout){
		m_layout->Release();
		m_layout = 0;
	}
	//release pixel shader
	if (m_pixelShader){
		m_pixelShader->Release();
		m_pixelShader = 0;
	}
	//release vertex shader
	if (m_vertexShader){
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}

void ProjectionShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename){
	char* compileErrors;
	unsigned long long bufferSize, i;
	ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++){
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", (char*)shaderFilename, MB_OK);
}

bool ProjectionShaderClass::SetShaderParameters(
	ID3D11DeviceContext* deviceContext,
	XMMATRIX &worldMatrix,
	XMMATRIX &viewMatrix,
	XMMATRIX &projectionMatrix,
	ID3D11ShaderResourceView* texture,
	XMFLOAT4 &ambientColour,
	XMFLOAT4 &diffuseColour,
	XMFLOAT3 &lightDirection,
	XMMATRIX &projTexViewMatrix,
	XMMATRIX &projTexProjectionMatrix,
	ID3D11ShaderResourceView* projectionTexture){

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;

	//transpose matrices to prep them for shader
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);
	//transpose projected texture matrices
	projTexViewMatrix = XMMatrixTranspose(projTexViewMatrix);
	projTexProjectionMatrix = XMMatrixTranspose(projTexProjectionMatrix);

	//lock constant buffer for writing
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	//get a pointer to the data in the constant buffer
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	//copy the matrices into the constant buffer
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	dataPtr->projTexView = projTexViewMatrix;
	dataPtr->projTexProjection = projTexProjectionMatrix;

	//unlock constant buffer
	deviceContext->Unmap(m_matrixBuffer, 0);

	//set pos of constant buffer in vs
	bufferNumber = 0;

	//set constant buffer in vs with updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	//lock light constant buffer for writing
	result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	//get pointer to data in constant buffer
	dataPtr2 = (LightBufferType*)mappedResource.pData;

	//copy lighting variables into the constant buffer
	dataPtr2->ambientColour = ambientColour;
	dataPtr2->diffuseColour = diffuseColour;
	dataPtr2->lightDirection = lightDirection;
	dataPtr2->padding = 0.0f;

	//unlock constant buffer
	deviceContext->Unmap(m_lightBuffer, 0);

	//set position of light constant buffer in ps
	bufferNumber = 0;

	//set light constant buffer in ps with updated values
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	//set shader texture resource in pixel shader
	deviceContext->PSSetShaderResources(0, 1, &texture);
	//set projection texture resource in pixelshader
	deviceContext->PSSetShaderResources(1, 1, &projectionTexture);

	//reset matrices because 2x render ---- IMPORTANT
	//transpose matrices to prep them for shader
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);
	//transpose projected texture matrices
	projTexViewMatrix = XMMatrixTranspose(projTexViewMatrix);
	projTexProjectionMatrix = XMMatrixTranspose(projTexProjectionMatrix);

	return true;
}

void ProjectionShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount){
	//set vertex input layout
	deviceContext->IASetInputLayout(m_layout);
	//set vertex and pixel shaders for rendering
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	//set the sampler state in the pixel shader
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	//render triangles
	deviceContext->DrawIndexed(indexCount, 0, 0);
}