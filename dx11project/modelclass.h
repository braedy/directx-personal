#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

/*
ENCAPSULATE GEOMETRY FOR MODELS
*/

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

#include "textureclass.h"

using namespace DirectX;
using namespace std;

class ModelClass{
	struct VertexType{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		//XMFLOAT4 color;
	};

	struct ModelType{
		float x, y, z; //position
		float tu, tv; //texture 
		float nx, ny, nz; //normal
	};

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_Texture;
	ModelType* m_model;

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	ID3D11ShaderResourceView* GetTexture();

	int GetIndexCount();
};

#endif