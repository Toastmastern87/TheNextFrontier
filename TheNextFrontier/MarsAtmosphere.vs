cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix rotationMatrix;
	float4 marsAtmosphereRadius;
};

cbuffer AtmosphericScatteringCalculations
{
	float4 cameraPos;		// The camera's current position
	float4 lightDirection;		// The direction vector to the light source
	float4 invWavelength;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
	float4 cameraHeight;	// The camera's current height
	float4 cameraHeight2;	// fCameraHeight^2
	float4 atmosphereRadius;		// The outer (atmosphere) radius
	float4 atmosphereRadius2;	// fOuterRadius^2
	float4 marsRadius;	// fInnerRadius^2
	float4 krESun;			// Kr * ESun
	float4 kmESun;			// Km * ESun
	float4 kr4PI;			// Kr * 4 * PI
	float4 km4PI;			// Km * 4 * PI
	float4 scale;			// 1 / (atmosphereRadius - marsRadius)
	float4 scaleDepth;		// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
	float4 scaleOverScaleDepth;	// fScale / fScaleDepth
}

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
	float4 color : COLOR0;
	float4 secondColor : COLOR1;
	float3 direction : NORMAL0;
};

// Returns the near intersection point of a line and a sphere
float GetNearIntersection(float3 pos, float3 ray, float distance, float radius)
{
	float B = 2.0f * dot(pos, ray);
	float C = (distance * distance) - (radius * radius);
	float det = max(0.0f, B*B - 4.0f * C);

	return (0.5f * (-B - sqrt(det)));
}

float Scale(float fCos)
{
	float x = 1.0 - fCos;
	return scaleDepth.x * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.80 + x * 5.25))));
}

PixelInputType MarsAtmosphereFromSpaceVertexShader(VertexInputType input)
{
	PixelInputType output;
	float3 finalPos;

	float3 ray, start, sampleRay, samplePoint, frontColor, attenuate;
	float far, near, startAngle, startDepth, startOffset, sampleLength, scaledLength;
	int samples;

	finalPos = input.a + input.r * input.localPosition.x + input.s * input.localPosition.y;

	finalPos = normalize(finalPos) * (marsAtmosphereRadius.r);

	samples = 4;

	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	ray = finalPos - cameraPos.xyz;
	far = length(ray);
	ray /= far;

	// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
	near = GetNearIntersection(cameraPos.xyz, ray, cameraHeight, atmosphereRadius);

	// Calculate the ray's starting position, then calculate its scattering offset
	start = cameraPos.xyz + ray * near;
	far -= near;
	startAngle = dot(ray, start) / atmosphereRadius;
	startDepth = exp(-1.0f / scaleDepth);
	startOffset = startDepth * Scale(startAngle);

	// Initialize the scattering loop variables
	sampleLength = far / samples;
	scaledLength = sampleLength * scale;
	sampleRay = ray * sampleLength;
	samplePoint = start + sampleRay * 0.5f;

	frontColor = float3(0.0, 0.0, 0.0);
	[unroll(samples)]
	for(int i = 0; i < samples; i++)
	{
		float heightForSunlight = length(samplePoint);
		float depth = exp(scaleOverScaleDepth * (marsRadius - heightForSunlight));
		float lightAngle = dot(-mul(lightDirection, rotationMatrix), samplePoint) / heightForSunlight;
		float cameraAngle = dot(ray, samplePoint) / heightForSunlight;
		float scatter = (startOffset + depth * (Scale(lightAngle) - Scale(cameraAngle)));
		attenuate = exp(-scatter * (invWavelength * kr4PI + km4PI));
		frontColor += attenuate * (depth * scaledLength);
		samplePoint += sampleRay;
	}

	output.position = mul(float4(finalPos, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.secondColor.rgb = frontColor * kmESun;
	output.color.rgb = frontColor * (invWavelength * krESun);

	output.direction = cameraPos - finalPos;

	return output;
}

PixelInputType MarsAtmosphereFromAtmosphereVertexShader(VertexInputType input)
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