#include "deferredshaderclass.h"

DeferredShaderClass::DeferredShaderClass(){
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_sampleStateWrap = 0;
	m_matrixBuffer = 0;
}

DeferredShaderClass::DeferredShaderClass(const DeferredShaderClass& other){
}

DeferredShaderClass::~DeferredShaderClass(){
}

bool DeferredShaderClass::Initialize(ID3D11Device* device, HWND hwnd){
	bool result;

	//initialise vs and ps
	result = InitializeShader(device, hwnd, L"deferred_vert.hlsl", L"deferred_pix.hlsl");
	if (!result) return false;

	return true;
}

void DeferredShaderClass::Shutdown(){
	//shut down vs ps et al.
	ShutdownShader();
}

bool DeferredShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX &worldMatrix, XMMATRIX &viewMatrix,
	XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture){
	bool result;

	//set shader params for render
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
	if (!result) return false;

	RenderShader(deviceContext, indexCount);

	return true;
}

bool DeferredShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename){
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;

	//init pointers
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//compile vs
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "DeferredVertexShader", "vs_5_0",
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
	result = D3DCompileFromFile(psFilename, NULL, NULL, "DeferredPixelShader", "ps_5_0",
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

	//create vertex input layout description
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

	//get no of elements in layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//create vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_layout);
	if (FAILED(result)) return false;

	//release vs buffer and ps buffer (no longer needed)
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	//create a wrap texture sampler state description
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
	result = device->CreateSamplerState(&samplerDesc, &m_sampleStateWrap);
	if (FAILED(result)) return false;

	//setup dynamic matrix constant buffer description (vertex shader)
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//create constant buffer pointer to access the constant buffer in vs
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result)) return false;

	return true;
}

void DeferredShaderClass::ShutdownShader(){
	// Release the matrix constant buffer.
	if (m_matrixBuffer){
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}
	// Release the sampler state.
	if (m_sampleStateWrap){
		m_sampleStateWrap->Release();
		m_sampleStateWrap = 0;
	}
	// Release the layout.
	if (m_layout){
		m_layout->Release();
		m_layout = 0;
	}
	// Release the pixel shader.
	if (m_pixelShader){
		m_pixelShader->Release();
		m_pixelShader = 0;
	}
	// Release the vertex shader.
	if (m_vertexShader){
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}

void DeferredShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename){
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++){
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

bool DeferredShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX &worldMatrix, XMMATRIX &viewMatrix,
	XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;

	// Transpose the matrices to prepare them for the shader.
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))	return false;
	
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

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	//reset matrices to original transposition
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	return true;
}

void DeferredShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount){
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Set the sampler states in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleStateWrap);

	// Render the geometry.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}
