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
	float4 marsRadius2;	// fInnerRadius^2
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
	float4 color : COLOR;
};

float Scale(float fCos)
{
	float x = 1.0 - fCos;
	return scaleDepth.x * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.80 + x * 5.25))));
}

PixelInputType MarsAtmosphereFromSpaceVertexShader(VertexInputType input)
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