#ifndef _TEXTUREARRAYCLASS_H_
#define _TEXTUREARRAYCLASS_H_

#include <d3d11.h>
#include "textureclass.h"


class TextureArrayClass{
	ID3D11ShaderResourceView* m_textures[2];
	TextureClass* m_Texture;

public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass&);
	~TextureArrayClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	
	ID3D11ShaderResourceView** GetTextureArray();
};

#endif