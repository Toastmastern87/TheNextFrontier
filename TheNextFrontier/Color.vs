cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
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

PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 finalPos;
	float3 posxr;
	float3 posys;

	posxr = float3(input.r.x * input.localPosition.x, input.r.y * input.localPosition.x, input.r.z * input.localPosition.x);
	posys = float3(input.s.x * input.localPosition.y, input.s.y * input.localPosition.y, input.s.z * input.localPosition.y);

	finalPos.x = input.a.x + posxr.x + posys.x;
	finalPos.y = input.a.y + posxr.y + posys.y;
	finalPos.z = input.a.z + posxr.z + posys.z;
	finalPos.w = 1.0f;

	output.position = mul(finalPos, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	return output;
}