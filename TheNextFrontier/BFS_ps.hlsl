Texture2D BFSTexture;
SamplerState sampleType;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 BFSPixelShader(PixelInputType input) : SV_TARGET
{
	float4 finalColor;

	finalColor = BFSTexture.Sample(sampleType, input.tex);

	return finalColor;
}