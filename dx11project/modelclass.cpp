#include "modelclass.h"

// init vertex and index buffers
ModelClass::ModelClass(){
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	//tex
	m_Texture = 0;
	m_model = 0;
	m_TextureArray = 0;
}

ModelClass::ModelClass(const ModelClass& other){
}

ModelClass::~ModelClass(){
}

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFileName, char* textureFilename){
	bool result;

	//load model data
	result = LoadModel(modelFileName);
	if (!result) return false;
	// initialize vertex and index buffers holding geometry
	result = InitializeBuffers(device);
	if (!result){ return false; }

	// Load the texture for this model.
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result) return false;

	return true;
}

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFileName, char* textureFilename1, char* textureFilename2){
	bool result;

	//load model data
	result = LoadModel(modelFileName);
	if (!result) return false;
	// initialize vertex and index buffers holding geometry
	result = InitializeBuffers(device);
	if (!result){ return false; }

	// Load the texture for this model.
	result = LoadTextureArray(device, deviceContext, textureFilename1, textureFilename2);
	if (!result) return false;

	return true;
}

void ModelClass::Shutdown(){
	// Release the model texture.
	ReleaseTextures();
	// relesase vertex and index buffers
	ShutdownBuffers();
	// release model
	ReleaseModel();
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext){
	// buffers into pipeline
	RenderBuffers(deviceContext);
}

// return no of indexes in model
int ModelClass::GetIndexCount(){
	return m_indexCount;
}

ID3D11ShaderResourceView** ModelClass::GetTextureArray(){
	return m_TextureArray->GetTextureArray();
}

ID3D11ShaderResourceView* ModelClass::GetTexture(){
	return m_Texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device){
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// create vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices){ return false; }

	// create index array
	indices = new unsigned long[m_indexCount];
	if (!indices){ return false; }

	//load vertex and index arrays
	for ( i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	/*// load vertex array with data
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f); // bottom left
	//vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);


	//vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f); // top centre
	vertices[1].position = XMFLOAT3(-1.0f, 1.0f, 0.0f); // top left
	//vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[1].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f); // bottom right
	vertices[2].position = XMFLOAT3(1.0f, 1.0f, 0.0f); // top right
	//vertices[2].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	vertices[2].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[3].position = XMFLOAT3(1.0f, 1.0f, 0.0f); // top right
	//vertices[3].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	vertices[3].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[4].position = XMFLOAT3(1.0f, -1.0f, 0.0f); // bottom right
	//vertices[4].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	vertices[4].texture = XMFLOAT2(1.0f, 1.0f);
	vertices[4].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[5].position = XMFLOAT3(-1.0f, -1.0f, 0.0f); // bottom left
	//vertices[5].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[5].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[5].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);


	// load index array with data
	indices[0] = 0; // bottom left
	indices[1] = 1; // top centre // top left
	indices[2] = 2; // bottom right // top right
	indices[3] = 3; // bottom right
	indices[4] = 4; // bottom left
	indices[5] = 5; // top centre // top left*/


	// set up static vertex buffer description
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// give subresource structre a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result)){ return false; }

	// set up static index buffer description
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// give subresource structre a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result)){ return false; }

	// release arrays as buffers have been created and loaded
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;

	return true;
}

void ModelClass::ShutdownBuffers(){
	// release index buffer
	if (m_indexBuffer){
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
	// release vertex buffer
	if (m_vertexBuffer){
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext){
	unsigned int stride;
	unsigned int offset;

	// set vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	// set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// set the index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// set primitive type that should be rendered from this vertex buffer
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename){
	bool result;

	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)	return false;

	// Initialize the texture object.
	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result) return false;

	return true;
}

bool ModelClass::LoadTextureArray(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename1, char* filename2){
	bool result;

	// Create the texture object.
	m_TextureArray = new TextureArrayClass;
	if (!m_TextureArray) return false;

	// Initialize the texture object.
	result = m_TextureArray->Initialize(device, deviceContext, filename1, filename2);
	if (!result) return false;

	return true;
}

void ModelClass::ReleaseTextures(){
	// Release the texture object.
	if (m_Texture){
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	if (m_TextureArray){
		m_TextureArray->Shutdown();
		delete m_TextureArray;
		m_TextureArray = 0;
	}
}

bool ModelClass::LoadModel(char* filename){
	ifstream fin;
	char input;
	int i;

	//open the model file
	fin.open(filename);
	if (fin.fail())	return false;

	//read up to the value of vertex count
	fin.get(input);
	while (input != ':'){ fin.get(input); }

	//read in the vertex count
	fin >> m_vertexCount;

	//set the number of indices to be the same as the vertex count
	m_indexCount = m_vertexCount;

	//create model using the vertex count read in
	m_model = new ModelType[m_vertexCount];
	if (!m_model) return false;

	//read upto the beginning of the data
	fin.get(input);
	while (input != ':'){ fin.get(input); }
	fin.get(input);
	fin.get(input);

	//read in vertex data
	for (i = 0; i < m_vertexCount; i++){
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	//close model file
	fin.close();

	return true;
}

void ModelClass::ReleaseModel(){
	if (m_model){
		delete[] m_model;
		m_model = 0;
	}
}