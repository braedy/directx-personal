//rewritten to handle deferred shading
//globals
cbuffer MatrixBuffer{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// typedefs
// normals not required as will be in the render texture (2D post processing)
struct VertexInputType{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


// vertex shader
// pass forward position and tex coords of 2d image being processed
PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;
	float3 worldPosition;

	//change position vector to be 4 units for proper matrix calc
	input.position.w = 1.0f;

	//calc position of vertex against world, view, and projection matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//store tex coordinates for pixel shader
	output.tex = input.tex;

	return output;
}