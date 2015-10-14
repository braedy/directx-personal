// GLOBALS
Texture2D shaderTexture;
SamplerState SampleType;

// TYPEDEFS
struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXTCOORD0;
};

// SHADER

float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	//sample pixel color from texture using sampler at texcoord location
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	return textureColor;
}