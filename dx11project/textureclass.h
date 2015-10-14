#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <d3d11.h>
#include <stdio.h>

class TextureClass{
	//define TARGA file header structure here to make reading in the data easier
	struct TargaHeader{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

	//m_targaData holds the raw targa data read straight in from the file
	//m_texture holds the structured texture data DirectX will use for rendering
	//m_textureView is the resource view the shader uses to access texture data when drawing
	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;

	//targa reading function
	//add reading functions here for more formats as required
	bool LoadTarga(char*, int&, int&);
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
};

#endif