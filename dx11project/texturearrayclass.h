#ifndef _TEXTUREARRAYCLASS_H_
#define _TEXTUREARRAYCLASS_H_

#include <d3d11.h>
#include <stdio.h>

class TextureArrayClass{
	struct TargaHeader{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture[2];
	ID3D11Texture2D* m_singleTexture;
	ID3D11ShaderResourceView* m_textureView[2];
	ID3D11ShaderResourceView* m_singleTextureView;

	bool LoadTarga(char*, int&, int&);
public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass&);
	~TextureArrayClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	
	ID3D11ShaderResourceView** GetTextureArray();
	ID3D11ShaderResourceView* GetTexture();
};

#endif