#include "texturearrayclass.h"
#include <assert.h>

TextureArrayClass::TextureArrayClass(){
	m_textures[0] = 0;
	m_textures[1] = 0;

	m_Texture = 0;
}

TextureArrayClass::TextureArrayClass(const TextureArrayClass& other){
}

TextureArrayClass::~TextureArrayClass(){
}

bool TextureArrayClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename1, char* filename2){
	bool result;	
	m_Texture = new TextureClass;
	if (!m_Texture) return false;
	// load first texture
	result = m_Texture->Initialize(device, deviceContext, filename1);
	assert(result);
	if (!result) return false;
	//set texture to array
	m_textures[0] = m_Texture->GetTexture();
	m_Texture->Shutdown(); // clear temp class instance
	// load second texture
	result = m_Texture->Initialize(device, deviceContext, filename2);
	if (!result) return false;
	//set texture to array
	m_textures[1] = m_Texture->GetTexture();
	m_Texture->Shutdown();

	return true;
}

void TextureArrayClass::Shutdown(){
	// Release the texture resources.
	if (m_textures[0]){
		m_textures[0]->Release();
		m_textures[0] = 0;
	}

	if (m_textures[1]){
		m_textures[1]->Release();
		m_textures[1] = 0;
	}

	if (m_Texture){
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}

ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray(){
	return m_textures;
}