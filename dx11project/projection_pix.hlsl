// textures
Texture2D shaderTexture : register(t0);
Texture2D projectionTexture : register(t1);

// samplers
SamplerState SampleType;

// constant buffers
cbuffer LightBuffer{
	float4 ambientColour;
	float4 diffuseColour;
	float3 lightDirection;
	float padding;
};

// typedefs
struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 viewPosition : TEXCOORD1;
};

// pixel shader
float4 ProjectionPixelShader(PixelInputType input) : SV_TARGET
{
	float4 colour;
	float3 lightDir;
	float lightIntensity;
	float4 textureColour;
	float2 projTexCoord;
	float4 projectionColour;

	//set default output to ambient
	colour = ambientColour;

	//invert light dir for calcs
	lightDir = -lightDirection;

	//calc amount of light for this pixel
	lightIntensity = saturate(dot(input.normal, lightDir));

	if (lightIntensity > 0.0f){
		//set light colour based on the diffuse and amount of intensity
		colour += (diffuseColour * lightIntensity);
	}

	//saturate light colour
	colour = saturate(colour);

	//sample pix colour from texture using sampler at this texcoord loc
	textureColour = shaderTexture.Sample(SampleType, input.tex);

	//combine light colour and texture colour
	colour = colour * textureColour;

	//calc projected texture coordinates (from projection viewpoint)
	projTexCoord.x = input.viewPosition.x / input.viewPosition.w / 2.0f + 0.5f;
	projTexCoord.y = input.viewPosition.y / input.viewPosition.w / 2.0f + 0.5f;

	//determine if corrds are within 0 to 1 (projected view port)
	if ((saturate(projTexCoord.x) == projTexCoord.x) && (saturate(projTexCoord.y) == projTexCoord.y)){
		//sample colour at projected tex coordinate loc
		projectionColour = projectionTexture.Sample(SampleType, projTexCoord);
		//set output of this pixel to the projection texture overriding the
		//regular colour value
		colour = projectionColour;
	}

	return colour;
}