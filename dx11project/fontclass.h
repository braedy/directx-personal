#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include "textureclass.h"

using namespace DirectX;
using namespace std;

class FontClass{
	struct FontType{
		float left, right;
		int size;
	};
	struct VertexType{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	FontType* m_Font;
	TextureClass* m_Texture;

	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();
public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
	void BuildVertexArray(void*, char*, float, float);
};

#endif