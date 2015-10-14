//globals
cbuffer PerFrameBuffer{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//typedefs
struct VertexInputType{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

//vertex Shader
PixelInputType FontVertexShader(VertexInputType input)
{
	PixelInputType output;

	//change position vector to be 4 units for correct matrix calc.
	input.position.w = 1.0f;

	//calc position of vertex against world, view, and projection matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//store texcoords to be passed to pixel shader
	output.tex = input.tex;

	return output;
}