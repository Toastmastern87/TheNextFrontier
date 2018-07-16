cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	float4 marsAtmosphereRadius;
};

struct VertexInputType
{
	float2 localPosition : TEXCOORD0;
	int level : TEXTUREID;
	float3 a : POSITION0;
	float3 r : POSITION1;
	float3 s : POSITION2;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PixelInputType MarsAtmosphereVertexShader(VertexInputType input)
{
	PixelInputType output;
	float3 finalPos;

	finalPos = input.a + input.r * input.localPosition.x + input.s * input.localPosition.y;

	finalPos = normalize(finalPos) * (marsAtmosphereRadius.r);

	output.position = mul(float4(finalPos, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.color = float4(0.788f, 0.584f, 0.255f, 1.0f);

	return output;
}