// global
cbuffer MatrixBuffer{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// typedefs

struct VertexInputType{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

// vertex shader
PixelInputType MultiTexVertexShader(VertexInputType input){
	PixelInputType output;

	// adapt position vector
	input.position.w = 1.0f;

	// calc vertex
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// store tex coords for ps
	output.tex = input.tex;
	
	return output;
}