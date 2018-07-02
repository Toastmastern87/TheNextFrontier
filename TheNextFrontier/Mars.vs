Texture2D shaderTexture;
SamplerState sampleType;

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix inverseWorldMatrix;
	float marsRadius;
};

cbuffer MorphCalculation
{
	float4 distanceLUT[32];
	float4 cameraPos;
	float4 morphRange;
};

cbuffer HeightCalculations
{
	float4 marsMaxHeight;
	float4 marsMinHeight;
};

struct VertexInputType
{
	float2 localPosition : TEXCOORD0;
	float2 localMorph : TEXCOORD1;
	int level : TEXTUREID;
	float3 a : POSITION0;
	float3 r : POSITION1;
	float3 s : POSITION2;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 mapCoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 viewVector : NORMAL1;
};

float MorphFac(float distance, int level)
{
	float low, high, delta, a;

	low = distanceLUT[level - 1].x;
	high = distanceLUT[level].x;

	delta = high - low;

	a = (distance - low) / delta;

	return (1 - clamp((a/morphRange.x), 0, 1));
}

float GetHeight(float3 pos)
{
	float2 uv;
	float heightColorValue;
	float3 normalizePos;

	normalizePos = normalize(pos);

	uv = float2((0.5f + (atan2(normalizePos.z, normalizePos.x) / (2 * 3.14159265f))), (0.5f - (asin(normalizePos.y) / 3.14159265f)));

	heightColorValue = shaderTexture.SampleLevel(sampleType, uv, 0).r;

	return heightColorValue;
}

PixelInputType MarsVertexShader(VertexInputType input)
{
	PixelInputType output;
	float3 finalPos;
	float3 mapCoords;
	float3 normPos;
	float distance;
	float morphPercentage;
	float height;
	matrix normalMatrix;

	finalPos = input.a + input.r * input.localPosition.x + input.s * input.localPosition.y;

	distance = length(finalPos - cameraPos.xyz);
	morphPercentage = MorphFac(distance, input.level);

	finalPos += morphPercentage * (input.r * input.localMorph.x + input.s * input.localMorph.y);

	height = GetHeight(finalPos);

	normPos = normalize(finalPos) * (marsRadius + (height * (marsMaxHeight - marsMinHeight)) + marsMinHeight);
	finalPos = normPos;

	//Normal calculations
	output.normal = mul(finalPos, worldMatrix);
	output.viewVector = (cameraPos.xyz - mul(finalPos, worldMatrix));

	mapCoords = normalize(finalPos);
	output.mapCoord = float2((0.5f + (atan2(mapCoords.z, mapCoords.x) / (2 * 3.14159265f))), (0.5f - (asin(mapCoords.y) / 3.14159265f)));

	output.position = mul(float4(finalPos, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	return output;
}