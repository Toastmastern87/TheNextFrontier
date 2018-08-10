Texture2D BFSTexture : register(t0);
Texture2D BFSNormalTexture : register(t1);
SamplerState sampleType;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent: NORMAL1;
	float3 bitangent : NORMAL2;
};

cbuffer LightCalculations
{
	float4 lightDirection;
	float4 diffuseColor;
	matrix rotationMatrix;
};

float4 BFSPixelShader(PixelInputType input) : SV_TARGET
{
	float4 finalColor;
	float lightIntensity, lightColor;
	float3x3 TBNMatrix;
	float3 normal;

	TBNMatrix = float3x3(input.tangent, input.bitangent, input.normal);

	normal = normalize(BFSNormalTexture.Sample(sampleType, input.tex));

	normal = normalize(mul(normal, TBNMatrix));

	lightIntensity = saturate(dot(normal, normalize(mul(lightDirection, rotationMatrix))));

	lightColor = saturate(diffuseColor * lightIntensity);

	finalColor = BFSTexture.Sample(sampleType, input.tex);

	return finalColor * lightColor;
}