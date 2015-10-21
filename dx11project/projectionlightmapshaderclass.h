#ifndef _PROJECTIONLIGHTMAPSHADERCLASS_H_
#define _PROJECTIONLIGHTMAPSHADERCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>

using namespace DirectX;
using namespace std;

class ProjectionLightMapShaderClass{
	struct MatrixBufferType{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		//matrices from viewpoint of projection
		XMMATRIX projTexView;
		XMMATRIX projTexProjection;
	};

	struct LightBufferType{
		XMFLOAT4 ambientColour;
		XMFLOAT4 diffuseColour;
	};

	struct LightPositionBufferType{
		XMFLOAT3 lightPosition;
		float padding;
	};

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightPositionBuffer;
	ID3D11Buffer* m_lightBuffer;

	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX&, XMMATRIX&, XMMATRIX&, ID3D11ShaderResourceView*, XMFLOAT4&, XMFLOAT4&, XMFLOAT3&, XMMATRIX&, XMMATRIX&, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

public:
	ProjectionLightMapShaderClass();
	ProjectionLightMapShaderClass(const ProjectionLightMapShaderClass&);
	~ProjectionLightMapShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX&, XMMATRIX&, XMMATRIX&, ID3D11ShaderResourceView*, XMFLOAT4&, XMFLOAT4&, XMFLOAT3&, XMMATRIX&, XMMATRIX&, ID3D11ShaderResourceView*);
};
#endif