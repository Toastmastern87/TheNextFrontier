Texture2D heightMapTexture : register(t0);
Texture2D heightMapDetail2Texture : register(t1);
Texture2D colorMapTexture : register(t2);
Texture2D detailAreaMapTexture : register(t3);
Texture2D craterHeightMapTexture : register(t4);
SamplerState sampleType;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
	float4 secondColor : COLOR1;
	float2 mapCoord : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewVector : NORMAL1;
};

cbuffer LightCalculations
{
	float4 lightDirection;
	float4 diffuseColor;
	float4 patchDelta;
	matrix rotationMatrix;
};

float3x3 GetTBNMatrix(float3 normalVector, float3 posVector, float2 uv)
{
	float3 dp1, dp2, dp2perp, dp1perp, T, B;
	float2 duv1, duv2;
	float invMax;

	dp1 = ddx(posVector);
	dp2 = ddy(posVector);
	duv1 = ddx(uv);
	duv2 = ddx(uv);

	dp2perp = cross(dp2, normalVector);
	dp1perp = cross(normalVector, dp1);

	T = (dp2perp * duv1.x + dp1perp * duv2.x);
	B = (dp2perp * duv1.y + dp1perp * duv2.y);

	invMax = rsqrt(max(dot(T, T), dot(B, B)));

	return float3x3(T * invMax, B * invMax, normalVector);
}

float GetHeight(float2 uv, bool insideAtmosphere)
{
	float2 textureStretch;
	float finalHeight;

	textureStretch = float2(2.0f, 1.0f);

	float4 detailArea = detailAreaMapTexture.SampleLevel(sampleType, uv, 0).rgba;

	if (insideAtmosphere)
	{
		finalHeight = (heightMapTexture.SampleLevel(sampleType, uv, 0).r * (21.229f + 8.2f));// +(heightMapDetail2Texture.SampleLevel(sampleType, (uv * textureStretch * 700), 1).r * 1.0f);
	}
	else
	{
		finalHeight = (heightMapTexture.SampleLevel(sampleType, uv, 0).r * (21.229f + 8.2f));
	}

	if (detailArea.r == 1.0f && detailArea.g != 1.0f)
	{
		float2 craterMapping = uv - float2((4669.0f / 8192.0f), (1704.0f / 4096.0f));
		craterMapping = float2((craterMapping.x * 8192.0f), (craterMapping.y * 4096.0f));

		finalHeight += craterHeightMapTexture.SampleLevel(sampleType, (craterMapping / 25.0f), 0).rgb * 5.0f;
	}

	return finalHeight;
}

float3 CalculateNormal(float3 normalVector, float3 viewVector, float2 uv, bool insideAtmosphere)
{
	float textureWidth, textureHeight, hL, hR, hD, hU;
	float3 texOffset, N;
	float3x3 TBN;

	heightMapTexture.GetDimensions(textureWidth, textureHeight);

	texOffset = float3((1.0f / textureWidth), (1.0f / textureHeight), 0.0f);

	hL = GetHeight((uv - texOffset.xz), insideAtmosphere);
	hR = GetHeight((uv + texOffset.xz), insideAtmosphere);
	hD = GetHeight((uv + texOffset.zy), insideAtmosphere);
	hU = GetHeight((uv - texOffset.zy), insideAtmosphere);

	N = normalize(float3((hL - hR), (hU - hD), 2.0f));

	TBN = GetTBNMatrix(normalVector, -viewVector, uv);

	return mul(N, TBN);
}

float4 MarsFromAtmospherePixelShader(PixelInputType input) : SV_TARGET
{
		float3 normal;
	float lightIntensity, lightColor, increasedLigthningFactor;
	float4 finalColor, color;

	normal = normalize(CalculateNormal(normalize(input.normal), normalize(input.viewVector), input.mapCoord, true));

	lightIntensity = saturate(dot(normal, normalize(mul(lightDirection, rotationMatrix))));

	lightColor = saturate(diffuseColor * lightIntensity);

	color = colorMapTexture.SampleLevel(sampleType, input.mapCoord, 0).rgba; //float4(0.666f, 0.435f, 0.313f, 1.0f); //

	// To use when the color is decided by the GPU
	//color.r *= 7.0f;
	//color.g *= 4.0f;
	//color.b *= 3.0f;

	// To be used when the color is decided by a texture
	color.r *= 7.0f;
	color.g *= 6.0f;
	color.b *= 3.0f;

	increasedLigthningFactor = 1.0f;

	return input.color * color * input.secondColor * lightIntensity * 5.0f; //(color * increasedLigthningFactor) * lightColor;
}