//globals
cbuffer MatrixBuffer{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;

	// projected matrices
	matrix projViewMatrix;
	matrix projProjectionMatrix;
};

cbuffer LightPositionBuffer{
	float3 lightPosition;
	float padding;
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
	//viewpos of vertices from projection location
	float4 viewPosition : TEXCOORD1;
	// light position is added for point light calc
	float3 lightPos : TEXCOORD2;
};

//vertex shader
PixelInputType ProjectionLightMapVertexShader(VertexInputType input){
	PixelInputType output;
	float4 worldPosition;

	//change position vector for matrix calc
	input.position.w = 1.0f;

	//calc position of vertex against the world, view, projection matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//store position of vertice as viewed by projection view point
	output.viewPosition = mul(input.position, worldMatrix);
	output.viewPosition = mul(output.viewPosition, projViewMatrix);
	output.viewPosition = mul(output.viewPosition, projProjectionMatrix);

	//store tex coords for ps
	output.tex = input.tex;

	//calc normal vector against world matrix
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	//normalise normal vector
	output.normal = normalize(output.normal);

	//calc position of vertex in world
	worldPosition = mul(input.position, worldMatrix);
	
	//determine light position based on light position and world vertex position
	output.lightPos = lightPosition.xyz - worldPosition.xyz;

	//normalize light position vector
	output.lightPos = normalize(output.lightPos);

	return output;
}