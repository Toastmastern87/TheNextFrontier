cbuffer PerFrameBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix positionMatrix;
	matrix scaleMatrix;
	matrix rotationMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent: NORMAL1;
	float3 bitangent : NORMAL2;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent: NORMAL1;
	float3 bitangent : NORMAL2;
};

PixelInputType BFSVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	output.normal = mul(input.normal, worldMatrix);

	output.tangent = mul(input.tangent, worldMatrix);

	output.bitangent = mul(input.bitangent, worldMatrix);
	
	return output;
}