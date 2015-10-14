#include "textureclass.h"

TextureClass::TextureClass(){
	m_targaData = 0;
	m_texture = 0;
	m_textureView = 0;
}


TextureClass::TextureClass(const TextureClass& other){
}


TextureClass::~TextureClass(){
}

//the Initialize functions
//input: Direct3D device and targa image filename.
//loads the targa data into an array, creates a texture and loads the targa data into it
//in the correct format(targa images are upside by default and need to be reversed)
//once loaded, create a resource view of the texture for the shader to use for drawing
bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename){
	bool result;
	int height, width;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	//call the TextureClass::LoadTarga function to load targa file into the m_targaData array
	//this function will also pass us back the height and width of the texture.
	//load targa image data into memory
	result = LoadTarga(filename, height, width);
	if(!result)	return false;
	
	//setup texture description to load the targa data into
	//use height and width from targa image data, and set to 32 bit RGBA texture format.
	//set SampleDesc to default. set the Usage to D3D11_USAGE_DEFAULT (better performing memory).
	//set the MipLevels, BindFlags, and MiscFlags to the settings required for Mipmaped textures
	//afterwards call CreateTexture2D to create an empty texture to copy the targa data into.

	// Setup the description of the texture.
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hResult)) return false;

	// Set the row pitch of the targa image data.
	rowPitch = (width * 4) * sizeof(unsigned char);

	//use UpdateSubresource to actually copy the targa data array into the DirectX texture
	//NB: copying data into buffers
	//use Map/Unmap for data that is going to be reloaded each frame or on a very regular basis
	//use UpdateSubresource for something that will be loaded once / rarely during loading sequence
	//UpdateSubresource puts the data into higher speed memory that gets cache retention preference
	//since it knows you aren't going to remove or reload it anytime soon
	//D3D11_USAGE_DEFAULT indicates to DX we're gonna use UpdateSubresource
	//for Map/Unmap use D3D11_USAGE_DYNAMIC to notify DirectX that this type of data is transient

	// Copy the targa image data into the texture.
	deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);

	//after loading texture create shader resource view. we use a pointer to set the texture in shaders
	//set the two important Mipmap variables in desc. giving us the full range of Mipmap levels for
	//high quality texture rendering at any distance. Once the shader resource view is created call
	//GenerateMips to creates the Mipmaps, or load Mipmap levels in manually for better quality

	// Setup the shader resource view description.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	//Create the shader resource view for the texture.
	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	if (FAILED(hResult)) return false;

	//Generate mipmaps for this texture.
	deviceContext->GenerateMips(m_textureView);

	//Release the targa image data now that the image data has been loaded into the texture.
	delete[] m_targaData;
	m_targaData = 0;

	return true;
}

//release texture data set pointers to null
void TextureClass::Shutdown(){
	// Release the texture view resource
	if (m_textureView){
		m_textureView->Release();
		m_textureView = 0;
	}

	// Release the texture
	if (m_texture){
		m_texture->Release();
		m_texture = 0;
	}

	// Release the targa data
	if (m_targaData){
		delete[] m_targaData;
		m_targaData = 0;
	}
}

//get tex view
ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_textureView;
}

//targa image loading function
//targa images are stored upside down and need to be flipped.
//open the file, read it into an array then load correctly ordered data into m_targaData array.
//(32bit targa only, this function will reject targa's that are saved as 24 bit)
bool TextureClass::LoadTarga(char* filename, int& height, int& width)
{
	int error, bpp, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	// Open the targa file for reading in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0){
		return false;
	}

	// Read in the file header.
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1){
		return false;
	}

	// Get the important information from the header.
	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	// Check that it is 32 bit and not 24 bit.
	if (bpp != 32){
		//imageSize = width * height * 3;
		return false;
	}
	else{
		// Calculate the size of the 32 bit image data.
		imageSize = width * height * 4;
	}
	// Allocate memory for the targa image data.
	targaImage = new unsigned char[imageSize];
	if (!targaImage){
		return false;
	}

	// Read in the targa image data.
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize){
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0){
		return false;
	}

	// Allocate memory for the targa destination data.
	m_targaData = new unsigned char[imageSize];
	if (!m_targaData){
		return false;
	}

	// Initialize the index into the targa destination data array.
	index = 0;

	// Initialize the index into the targa image data.
	k = (width * height * 4) - (width * 4);

	// Now copy the targa image data into the targa destination array
	// in the correct order since the targa format is stored upside down.
	for (j = 0; j<height; j++){
		for (i = 0; i<width; i++){
			m_targaData[index + 0] = targaImage[k + 2];  // Red.
			m_targaData[index + 1] = targaImage[k + 1];  // Green.
			m_targaData[index + 2] = targaImage[k + 0];  // Blue
			m_targaData[index + 3] = targaImage[k + 3];  // Alpha

			// Increment the indexes into the targa data.
			k += 4;
			index += 4;
		}

		//set the targa image data index back to the preceding row at the
		//beginning of the column since it is read in upside down.
		k -= (width * 8);
	}

	//release targa image data now it has been copied into the destination array
	delete[] targaImage;
	targaImage = 0;

	return true;
}