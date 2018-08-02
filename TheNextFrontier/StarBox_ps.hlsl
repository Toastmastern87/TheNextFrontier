Texture2D starBoxTexture;
SamplerState sampleType;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uvCoord : TEXCOORD0;
};

float4 StarBoxPixelShader(PixelInputType input) : SV_TARGET
{
	float4 finalColor;

	finalColor = starBoxTexture.SampleLevel(sampleType, input.uvCoord, 0).rgba;

	return finalColor;
}