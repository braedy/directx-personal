#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

/*
ENCAPSULATE GEOMETRY FOR MODELS
*/

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

#include "textureclass.h"
#include "texturearrayclass.h"

using namespace DirectX;
using namespace std;

class ModelClass{
	struct VertexType{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		//XMFLOAT3 normal;
		//XMFLOAT4 color;
	};

	struct ModelType{
		float x, y, z; //position
		float tu, tv; //texture 
		float nx, ny, nz; //normal
	};

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	ModelType* m_model;
	TextureClass* m_Texture;
	TextureArrayClass* m_TextureArray;

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	bool LoadTextureArray(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void ReleaseTextures();

	bool LoadModel(char*);
	void ReleaseModel();

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView** GetTextureArray();

	int GetIndexCount();
};

#endif