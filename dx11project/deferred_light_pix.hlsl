//globals
Texture2D colourTexture : register(t0);
Texture2D normalTexture : register(t1);

//sample states
SamplerState SampleTypePoint : register(s0);

cbuffer LightBuffer{
	float3 lightDirection;
	float padding;
};

//typedefs
struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


//pixel shader
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 colours;
	float4 normals;
	float3 lightDir;
	float lightIntensity;
	float4 outputColour;

	//sample colours from colour render texture using sampler at texcoord
	colours = colourTexture.Sample(SampleTypePoint, input.tex);

	//sample normals from normal render texture using sampler at texcoord
	normals = normalTexture.Sample(SampleTypePoint, input.tex);

	//directional lighting equation
	//invert for calc
	lightDir = -lightDirection;

	//calc amount of light on pixel
	lightIntensity = saturate(dot(normals.xyz, lightDir));

	//calc amount of diffuse and specular
	outputColour = saturate(colours * lightIntensity);

	return outputColour;
}