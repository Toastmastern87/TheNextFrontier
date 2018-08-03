cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix rotationMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 uvCoord : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uvCoord : TEXCOORD0;
};

PixelInputType StarBoxVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, rotationMatrix);
	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.uvCoord = input.uvCoord;

	return output;
}