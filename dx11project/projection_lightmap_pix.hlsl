// textures
Texture2D shaderTexture : register(t0);
Texture2D projectionTexture : register(t1); //this is now the lightmap

// samplers
SamplerState SampleType;

// constant buffers
cbuffer PointLightBuffer{
	float4 ambientColour;
	float4 diffuseColour;
};

// typedefs
struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 viewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
};

// pixel shader
float4 ProjectionLightMapPixelShader(PixelInputType input) : SV_TARGET
{
	float brightness;
	float4 colour;
	float lightIntensity;
	float4 textureColour;
	float2 projTexCoord;
	float4 projectionColour;

	//set brightness of light
	brightness = 1.5f;

	//set default output to ambient
	colour = ambientColour;

	//calc amount of light for this pixel
	lightIntensity = saturate(dot(input.normal, input.lightPos));

	if (lightIntensity > 0.0f){
		//set light colour based on the diffuse and amount of intensity
		colour += (diffuseColour * lightIntensity) * brightness;
	}

	//saturate light colour
	colour = saturate(colour);

	//sample pix colour from texture using sampler at this texcoord loc
	textureColour = shaderTexture.Sample(SampleType, input.tex);

	//combine light colour and texture colour
	colour = colour * textureColour;

	//calc projected lightmap texture coordinates (from projection viewpoint)
	projTexCoord.x = input.viewPosition.x / input.viewPosition.w / 2.0f + 0.5f;
	projTexCoord.y = input.viewPosition.y / input.viewPosition.w / 2.0f + 0.5f;

	//determine if corrds are within 0 to 1 (projected view port)
	if ((saturate(projTexCoord.x) == projTexCoord.x) && (saturate(projTexCoord.y) == projTexCoord.y)){
		//sample colour at projected tex coordinate loc
		projectionColour = projectionTexture.Sample(SampleType, projTexCoord);
		//set output of this pixel to the projection texture overriding the
		//regular colour value
		colour = saturate((colour * projectionColour * textureColour) + (ambientColour * textureColour));
	}
	else{
		colour = ambientColour * textureColour;
	}

	return colour;
}