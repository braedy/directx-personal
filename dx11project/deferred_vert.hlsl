//constant buffers
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//typedefs
struct VertexInputType{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

//vertex shader
PixelInputType DeferredVertexShader(VertexInputType input){
	PixelInputType output;

	//edit position vector to enable correct matrix calc
	input.position.w = 1.0f;

	//calc position of vertex against world, view, projection matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//store texture coordinates for the pixel shader
	output.tex = input.tex;

	//calc normal vector against the world matrix only
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	//normalise normal vector
	output.normal = normalize(output.normal);

	return output;
}