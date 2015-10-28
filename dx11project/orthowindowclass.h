#ifndef _ORTHOWINDOWCLASS_H_
#define _ORTHOWINDOWCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class OrthoWindowClass{
	struct VertexType{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	bool InitializeBuffers(ID3D11Device*, int, int);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

public:
	OrthoWindowClass();
	OrthoWindowClass(const OrthoWindowClass&);
	~OrthoWindowClass();

	bool Initialize(ID3D11Device*, int, int);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
};
#endif