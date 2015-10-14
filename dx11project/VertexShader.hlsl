// GLOBALS
cbuffer MatrixBuffer{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// TYPEDEFS
struct VertexInputType{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// VERTEX SHADER
PixelInputType ColorVertexShader(VertexInputType input){
	PixelInputType output;

	// change position vector to be 4 units for correct matrix calc.
	input.position.w = 1.0f;

	// calculate position of vertex against world, view, projection matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// store input color for the pixel shader to use.
	output.color = input.color;

	return output;
}