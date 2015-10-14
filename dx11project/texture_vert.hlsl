// GLOBALS
cbuffer MatrixBuffer{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// TYPEDEFS
struct VertexInputType{
	float4 position : POSITION;
	float2 tex : TEXCOORD0; //no. indicates texture coordinates set
};
struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

// SHADER

PixelInputType TextureVertexShader(VertexInputType input){
	PixelInputType output;

	//change position vector to be 4 units for correct matrix calculations
	input.position.w = 1.0f;

	//calc position of vertex against world, view, and projection matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//store texture coordinates for pixel shader
	output.tex = input.tex;

	return output;
}