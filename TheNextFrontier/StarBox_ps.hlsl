struct PixelInputType
{
	float4 position : SV_POSITION;
};

float4 StarBoxPixelShader(PixelInputType input) : SV_TARGET
{
	return float4((255.0f/255.0f), (6.0f/255.0f), (174.0f/255.0f), 1.0f);
}