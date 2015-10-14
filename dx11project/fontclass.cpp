#include "fontclass.h"

FontClass::FontClass(){
	m_Font = 0;
	m_Texture = 0;
}

FontClass::FontClass(const FontClass& other){
}

FontClass::~FontClass(){
}

bool FontClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fontFilename, char* textureFileName){
	bool result;
	// load in font data
	result = LoadFontData(fontFilename);
	if (!result) return false;
	//load new texture with characters
	result = LoadTexture(device, deviceContext, textureFileName);
	if (!result) return false;

	return true;
}

void FontClass::Shutdown(){
	// release font texture.
	ReleaseTexture();
	// release font data.
	ReleaseFontData();
}

bool FontClass::LoadFontData(char* filename){
	ifstream fin;
	int i;
	char temp;
	int fontBufferSize = 95; //95 used ascii characters for text

	//create font spacing buffer.
	m_Font = new FontType[fontBufferSize];
	if (!m_Font) return false;

	//read in font size and spacing between chars
	fin.open(filename);
	if (fin.fail()) return false;

	// Read in the characters
	for (i = 0; i<fontBufferSize; i++){
		fin.get(temp);
		while (temp != ' '){
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' '){
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	//close file
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	//release font data array
	if (m_Font){
		delete[] m_Font;
		m_Font = 0;
	}
}

bool FontClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename){
	bool result;

	//create texture object
	m_Texture = new TextureClass;
	if (!m_Texture)	return false;

	//init texture object
	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result) return false;

	return true;
}

void FontClass::ReleaseTexture(){
	// release texture object
	if (m_Texture){
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}

//returns font texture interface
ID3D11ShaderResourceView* FontClass::GetTexture(){
	return m_Texture->GetTexture();
}

//build vertex buffers out of the text sentences
void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;

	//coerce input vertices into a VertexType structure.
	vertexPtr = (VertexType*)vertices;

	//get no. of letters in sentence.
	numLetters = (int)strlen(sentence);

	// init index to the vertex array
	index = 0;

	//build vertex and index arrays
	// Draw each letter onto a quad.
	for (i = 0; i<numLetters; i++){
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if (letter == 0){
			drawX = drawX + 3.0f;
		}
		else
		{
			//screen space coords required reordering texcoords
			// First triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
}