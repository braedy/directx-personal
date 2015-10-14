//globals
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer PixelBuffer{
	float4 pixelColour;
};

//typedefs
struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

//pixel shader
float4 FontPixelShader(PixelInputType input) : SV_TARGET
{
	float4 colour;

	//sample texture pixel at this location.
	colour = shaderTexture.Sample(SampleType, input.tex);

	// If the color is black on the texture then treat this pixel as transparent.
	if (colour.r == 0.0f){
		colour.a = 0.0f;
	}
	// If the color is other than black on the texture then this is a pixel in the font so draw it using the font pixel color.
	else{
		colour.a = 1.0f;
		colour = colour * pixelColour;
	}

	return colour;
}
