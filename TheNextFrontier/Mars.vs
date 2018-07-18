Texture2D heightMapTexture : register(t0);
Texture2D heightMapDetail2Texture : register(t1);
SamplerState sampleType;

#define PI 3.141592653589793

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

cbuffer AtmosphericScatteringCalculations
{
	float4 lightDirection;		// The direction vector to the light source
	float4 invWavelength;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
	float4 cameraHeight;	// The camera's current height
	float4 cameraHeight2;	// fCameraHeight^2
	float4 atmosphereRadius;		// The outer (atmosphere) radius
	float4 atmosphereRadius2;	// fOuterRadius^2
	float4 marsRadius2;	// fInnerRadius^2
	float4 krESun;			// Kr * ESun
	float4 kmESun;			// Km * ESun
	float4 kr4PI;			// Kr * 4 * PI
	float4 Km4PI;			// Km * 4 * PI
	float4 scale;			// 1 / (atmosphereRadius - marsRadius)
	float4 scaleDepth;		// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
	float4 scaleOverScaleDepth;	// fScale / fScaleDepth
}

float MorphFac(float distance, int level)
{
	float low, high, delta, a;

	low = distanceLUT[level - 1].x;
	high = distanceLUT[level].x;

	delta = high - low;

	a = (distance - low) / delta;

	return (1 - clamp((a/morphRange.x), 0, 1));
}

float GetHeight(float3 pos, float maxHeight, float minHeight)
{
	float2 uv;
	float heightColorValue, heightDetail2ColorValue;
	float3 normalizePos;

	normalizePos = normalize(pos);

	uv = float2((0.5f + (atan2(normalizePos.z, normalizePos.x) / (2 * PI))), (0.5f - (asin(normalizePos.y) / PI)));
	//uv = float2(saturate(((atan2(normalizePos.z, normalizePos.x) / 3.14159265f) + 1.0f) / 2), (0.5f - (asin(normalizePos.y) / 3.14159265f)));

	heightColorValue = heightMapTexture.SampleLevel(sampleType, uv, 0).r;

	return (heightColorValue * (maxHeight - minHeight));
}

// Returns the near intersection point of a line and a sphere
float GetNearIntersection(float3 pos, float3 ray, float distance2, float radius2)
{
	float B = 2.0 * dot(pos, ray);
	float C = distance2 - radius2;
	float det = max(0.0, B*B - 4.0 * C);

	return (0.5 * (-B - sqrt(det)));
}

PixelInputType MarsFromSpaceVertexShader(VertexInputType input)
{
	PixelInputType output;
	float3 finalPos;
	float3 mapCoords;
	float2 textureStretch;
	float distance;
	float morphPercentage;
	float height;

	float3 ray;
	float far, near;

	matrix normalMatrix;

	finalPos = input.a + input.r * input.localPosition.x + input.s * input.localPosition.y;

	distance = length(finalPos - cameraPos.xyz);
	morphPercentage = MorphFac(distance, input.level);

	finalPos += morphPercentage * (input.r * input.localMorph.x + input.s * input.localMorph.y);

	finalPos = normalize(finalPos) * (marsRadius + GetHeight(finalPos, marsMaxHeight, marsMinHeight) + marsMinHeight);

	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	ray = finalPos - cameraPos.xyz;
	far = length(ray);
	ray /= far;

	// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
	near = GetNearIntersection(cameraPos.xyz, ray, cameraHeight2, atmosphereRadius2);

	//Normal calculations
	output.normal = mul(finalPos, worldMatrix);
	output.viewVector = (mul(cameraPos.xyz, worldMatrix) - mul(finalPos, worldMatrix));

	mapCoords = normalize(finalPos);
	output.mapCoord = float2((0.5f + (atan2(mapCoords.z, mapCoords.x) / (2 * 3.14159265f))), (0.5f - (asin(mapCoords.y) / 3.14159265f)));

	output.position = mul(float4(finalPos, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//float heightColor = heightMapTexture.SampleLevel(sampleType, output.mapCoord, 0).r + (heightMapDetail2Texture.SampleLevel(sampleType, (output.mapCoord * textureStretch * 100), 0).r * 0.01f);

	//output.color = float4(heightColor, heightColor, heightColor, 1.0f);

	return output;
}

PixelInputType MarsFromAtmosphereVertexShader(VertexInputType input)
{
	PixelInputType output;
	float3 finalPos;
	float3 mapCoords;
	float2 textureStretch;
	float distance;
	float morphPercentage;
	float height;
	matrix normalMatrix;

	finalPos = input.a + input.r * input.localPosition.x + input.s * input.localPosition.y;

	distance = length(finalPos - cameraPos.xyz);
	morphPercentage = MorphFac(distance, input.level);

	finalPos += morphPercentage * (input.r * input.localMorph.x + input.s * input.localMorph.y);

	finalPos = normalize(finalPos) * (marsRadius + GetHeight(finalPos, marsMaxHeight, marsMinHeight) + marsMinHeight);

	//Normal calculations
	output.normal = mul(finalPos, worldMatrix);
	output.viewVector = (mul(cameraPos.xyz, worldMatrix) - mul(finalPos, worldMatrix));

	mapCoords = normalize(finalPos);
	output.mapCoord = float2((0.5f + (atan2(mapCoords.z, mapCoords.x) / (2 * 3.14159265f))), (0.5f - (asin(mapCoords.y) / 3.14159265f)));

	output.position = mul(float4(finalPos, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//float heightColor = heightMapTexture.SampleLevel(sampleType, output.mapCoord, 0).r + (heightMapDetail2Texture.SampleLevel(sampleType, (output.mapCoord * textureStretch * 100), 0).r * 0.01f);

	//output.color = float4(heightColor, heightColor, heightColor, 1.0f);

	return output;
}