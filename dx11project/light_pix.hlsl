//globals
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer{
	float4 ambientColour;
	float4 diffuseColour;
	float3 lightDirection;
	float specularPower;
	float4 specularColour;
};

//typedefs
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};


//pixel shader
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColour;
	float3 lightDir;
	float lightIntensity;
	float4 colour;
	float3 reflection;
	float4 specular;

	//sample pixel colour from texture using sampler at texcoord location
	textureColour = shaderTexture.Sample(SampleType, input.tex);

	//set default output colour to ambient light value for all pixels
	colour = ambientColour;

	//init specular color
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//invert for calc
	lightDir = -lightDirection;

	//calc amount of light on pixel
	lightIntensity = saturate(dot(input.normal, lightDir));

	if (lightIntensity > 0.0f){
		//determine final diffuse color from diffuse color and amount of light intensity
		colour += (diffuseColour * lightIntensity);

		//saturate ambient and diffuse colour
		colour = saturate(colour);

		//calc the reflection vector based on light intensity, normal and light direction
		reflection = normalize(2 * lightIntensity * input.normal - lightDir);

		//determine amount of specular light based on
		//reflection vector, view direction, specular power
		specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
	}

	//multiply by tex colour value to get final colour
	colour = colour * textureColour;

	//calc amount of diffuse and specular
	colour = saturate(colour + specular);

	return colour;
}