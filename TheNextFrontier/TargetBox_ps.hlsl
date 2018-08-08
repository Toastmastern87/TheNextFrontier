Texture2D TargetBoxTexture;
SamplerState sampleType;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 TargetBoxPixelShader(PixelInputType input) : SV_TARGET
{
	float4 finalColor;

	finalColor = TargetBoxTexture.Sample(sampleType, input.tex);

	return finalColor;
}