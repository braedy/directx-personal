#include "orthowindowclass.h"

OrthoWindowClass::OrthoWindowClass(){
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

OrthoWindowClass::OrthoWindowClass(const OrthoWindowClass& other){}

OrthoWindowClass::~OrthoWindowClass(){}

bool OrthoWindowClass::Initialize(ID3D11Device* device, int windowWidth, int windowHeight){
	bool result;

	//init vertex and index buffers that hold geometry for otho window
	result = InitializeBuffers(device, windowWidth, windowHeight);
	if (!result) return false;

	return true;
}

void OrthoWindowClass::Shutdown(){
	//release vertex and index buffers
	ShutdownBuffers();
}

void OrthoWindowClass::Render(ID3D11DeviceContext* deviceContext){
	//put buffers on pipeline to prep for drawing
	RenderBuffers(deviceContext);
}

int OrthoWindowClass::GetIndexCount(){
	return m_indexCount;
}

bool OrthoWindowClass::InitializeBuffers(ID3D11Device* device, int windowWidth, int windowHeight){
	float left, right, top, bottom;
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Calculate screen coordinates 
	left = (float)((windowWidth / 2) * -1); //left side of the window.
	right = left + (float)windowWidth; //right
	top = (float)(windowHeight / 2); //top
	bottom = top - (float)windowHeight; //bottom

	// Set the number of vertices in the vertex array.
	m_vertexCount = 6;

	// Set the number of indices in the index array.
	m_indexCount = m_vertexCount;

	// Create vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices) return false;

	// Create index array.
	indices = new unsigned long[m_indexCount];
	if (!indices) return false;

	// Load the vertex array with data.
	// First triangle.
	vertices[0].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	// Load the index array with data.
	for (i = 0; i<m_indexCount; i++){
		indices[i] = i;
	}

	//setup vertex buffer desc
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//give subresource structure a vertex data pointer
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//create vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result)) return false;

	//setup index buffer desc
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//give subresource structure an index data pointer
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//create index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result)) return false;

	//release arrays now buffers are completed
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;

	return true;
}

void OrthoWindowClass::ShutdownBuffers(){
	// Release index buffer.
	if (m_indexBuffer){
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
	// Release vertex buffer.
	if (m_vertexBuffer){
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void OrthoWindowClass::RenderBuffers(ID3D11DeviceContext* deviceContext){
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}