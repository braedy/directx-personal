// TYPEDEFS
struct PixelInputType{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// PIXEL SHADER
float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
	return 0.5 * input.color;
}