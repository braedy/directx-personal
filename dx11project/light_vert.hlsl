//globals
cbuffer MatrixBuffer{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer{
	float3 cameraPosition;
	float padding; //padding
};

// typedefs
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};


// vertex shader
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

	//calc normal vector against the world matrix only
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	//normalize the normal vector
	output.normal = normalize(output.normal);

	//calc position of the vertex in the world
	worldPosition = mul(input.position, worldMatrix);

	//determine view direction based on position of camera and
	//position of vertex in the world
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	//normalize view direction vector
	output.viewDirection = normalize(output.viewDirection);

	return output;
}