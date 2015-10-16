// globals
Texture2D shaderTextures[2];
SamplerState SampleType;

// typedefs
struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

// pixel shader
float4 MultiTexPixelShader(PixelInputType input) : SV_TARGET
{
	float4 colour1;
	float4 colour2;
	float4 blendColour;

	//float4 textureColor;
	//sample pixel color from texture using sampler at texcoord location
	//textureColor = shaderTextures[1].Sample(SampleType, input.tex);

	//return textureColor;

	// get pix colour from tex1
	colour1 = shaderTextures[0].Sample(SampleType, input.tex);
	// get pix colour from tex2
	colour2 = shaderTextures[1].Sample(SampleType, input.tex);
	// blend pixels and multiply by gamma
	blendColour = colour1 * colour2 * 2.0f; //

	// saturate colour
	blendColour = saturate(blendColour);

	return blendColour;
}