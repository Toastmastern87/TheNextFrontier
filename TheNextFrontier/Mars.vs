Texture2D heightMapTexture : register(t0);
Texture2D heightMapDetail2Texture : register(t1);
SamplerState sampleType;

#define PI 3.141592653589793

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix rotationMatrix;
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

cbuffer AtmosphericScatteringCalculations
{
	float4 lightDirection;	
	float4 invWavelength;	
	float4 cameraHeight;	
	float4 atmosphereRadius;	
	float4 kr;			
	float4 km;			
	float4 eSun;		
	float4 scale;			
	float4 scaleDepth;		
	float4 scaleOverScaleDepth;	
}

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
	float4 color : COLOR0;
	float4 secondColor : COLOR1;
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
float GetNearIntersection(float3 pos, float3 ray, float distance, float radius)
{
	float B = 2.0f * dot(pos, ray);
	float C = (distance * distance) - (radius * radius);
	float det = max(0.0f, B*B - 4.0f * C);

	return (0.5f * (-B - sqrt(det)));
}

float Scale(float cos)
{
	float x = 1.0f - cos;
	return (scaleDepth.x * exp(-0.00287f + x * (0.459f + x * (3.83f + x * (-6.80f + x * 5.25f)))));
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

	float3 ray, start, sampleRay, samplePoint, frontColor, attenuate;
	float far, near, depth, cameraAngle, lightAngle, cameraScale, lightScale, cameraOffset, temp, sampleLength, scaledLength;
	int samples;

	matrix normalMatrix;

	finalPos = input.a + input.r * input.localPosition.x + input.s * input.localPosition.y;

	distance = length(finalPos - cameraPos.xyz);
	morphPercentage = MorphFac(distance, input.level);

	finalPos += morphPercentage * (input.r * input.localMorph.x + input.s * input.localMorph.y);

	finalPos = normalize(finalPos) * (marsRadius + GetHeight(finalPos, marsMaxHeight, marsMinHeight) + marsMinHeight);

	samples = 2;

	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	ray = finalPos - cameraPos.xyz;
	far = length(ray);
	ray /= far;

	// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
	near = GetNearIntersection(cameraPos.xyz, ray, cameraHeight, atmosphereRadius);

	// Calculate the ray's starting position, then calculate its scattering offset
	start = cameraPos.xyz + ray * near;
	far -= near;
	depth = exp((marsRadius.x - atmosphereRadius.x) / scaleDepth);
	cameraAngle = dot(-ray, finalPos) / length(finalPos);
	lightAngle = dot(mul(lightDirection.xyz, rotationMatrix), finalPos) / length(finalPos);
	cameraScale = Scale(cameraAngle);
	lightScale = Scale(lightAngle);
	cameraOffset = depth * cameraScale;
	temp = lightScale + cameraScale;

	// Initialize the scattering loop variables
	sampleLength = far / samples;
	scaledLength = sampleLength * scale;
	sampleRay = ray * sampleLength;
	samplePoint = start + sampleRay * 0.5f;

	// Now loop through the sample rays
	frontColor = float3(0.0f, 0.0f, 0.0f);
	[unroll(samples)]
	for(int i = 0; i < samples; i++)
	{
		float height = length(samplePoint);
		float depth = exp(scaleOverScaleDepth.x * (marsRadius.x - height));
		float scatter = depth * temp - cameraOffset;
		attenuate = exp(-scatter * (invWavelength.xyz * (kr.x * 4 * PI) + (km.x * 4 * PI)));
		frontColor += attenuate * (depth * scaledLength);
		samplePoint += sampleRay;
	}

	//Normal calculations
	output.normal = mul(finalPos, worldMatrix);
	output.viewVector = (mul(cameraPos.xyz, worldMatrix) - mul(finalPos, worldMatrix));

	mapCoords = normalize(finalPos);
	output.mapCoord = float2((0.5f + (atan2(mapCoords.z, mapCoords.x) / (2 * PI))), (0.5f - (asin(mapCoords.y) / PI)));

	output.position = mul(float4(finalPos, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//float heightColor = heightMapTexture.SampleLevel(sampleType, output.mapCoord, 0).r + (heightMapDetail2Texture.SampleLevel(sampleType, (output.mapCoord * textureStretch * 100), 0).r * 0.01f);

	output.color.rgb = frontColor * (invWavelength.xyz * (kr.x * eSun.x) + (km.x * eSun.x));
	output.secondColor.rgb = attenuate;

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