//textures
Texture2D shaderTexture : register(t0);

//sample states
SamplerState SampleTypeWrap : register(s0);

//typedefs
struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelOutputType{
	float4 colour : SV_Target0;
	float4 normal : SV_Target1;
};

//pixel shader
PixelOutputType DeferredPixelShader(PixelInputType input) : SV_TARGET{
	PixelOutputType output;

	//sample color from the texture, store for output to the render target
	output.colour = shaderTexture.Sample(SampleTypeWrap, input.tex);

	//store normal for output to the render target
	output.normal = float4(input.normal, 1.0f);

	return output;
}