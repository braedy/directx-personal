//globals
cbuffer MatrixBuffer{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;

	// texture viewpoint projected matrices
	matrix projTexViewMatrix;
	matrix projTexProjectionMatrix;
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
	//viewpos of vertices from projTex location
	float4 viewPosition : TEXCOORD1;
};

//vertex shader
PixelInputType ProjectionVertexShader(VertexInputType input){
	PixelInputType output;

	//change position vector for matrix calc
	input.position.w = 1.0f;

	//calc position of vertex against the world, view, projection matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//store position of vertice as viewed by projection view point
	output.viewPosition = mul(input.position, worldMatrix);
	output.viewPosition = mul(output.viewPosition, projTexViewMatrix);
	output.viewPosition = mul(output.viewPosition, projTexProjectionMatrix);

	//store tex coords for ps
	output.tex = input.tex;

	//calc normal vector against world matrix
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	//normalise normal vector
	output.normal = normalize(output.normal);

	return output;
}