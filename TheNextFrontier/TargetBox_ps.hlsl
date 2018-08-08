Texture2D shaderTexture;
SamplerState sampleType;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 TargetBoxPixelShader(PixelInputType input) : SV_TARGET
{
	float4 color;

	color = float4(1.0f, 0.0f, 0.0f, 0.0f);

	return color;
}