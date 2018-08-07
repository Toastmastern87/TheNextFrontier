Texture2D heightMapTexture : register(t0);
Texture2D heightMapDetail2Texture : register(t1);
Texture2D colorMapTexture : register(t2);
Texture2D detailAreaMapTextureX : register(t3);
Texture2D craterHeightMapTexture : register(t4);
Texture2D detailAreaMapTextureY : register(t5);
Texture2D detailAreaMapTextureWH : register(t6);
Texture2D crater2HeightMapTexture : register(t7);
SamplerState sampleType;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
	float4 secondColor : COLOR1;
	float4 originalPosition : COLOR2;
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

float GetHeight(float2 uv, bool insideAtmosphere, float4 pos)
{
	float finalHeight;

	finalHeight = (heightMapTexture.SampleLevel(sampleType, uv, 0).r * (21.229f + 8.2f));// +(heightMapDetail2Texture.SampleLevel(sampleType, (uv * textureStretch * 700), 1).r * 1.0f);

	return finalHeight;
}

float GetCraterDetailHeight(float2 uv, bool insideAtmosphere, float4 pos)
{
	float finalHeight;

	finalHeight = (craterHeightMapTexture.SampleLevel(sampleType, uv, 0).r * (21.229f + 8.2f)); 

	return finalHeight;
}

float GetCrater2DetailHeight(float2 uv, bool insideAtmosphere, float4 pos)
{
	float finalHeight;

	finalHeight = (crater2HeightMapTexture.SampleLevel(sampleType, uv, 0).r * (21.229f + 8.2f));

	return finalHeight;
}

float3 CalculateNormal(float3 normalVector, float3 viewVector, float2 uv, bool insideAtmosphere, float4 pos)
{
	float textureWidth, textureHeight, hL, hR, hD, hU;
	float craterDetailTextureWidth, craterDetailTextureHeight, hL2, hR2, hD2, hU2;
	float2 craterDetailUV;
	float3 texOffset, N, texOffsetCraterDetail;
	float3x3 TBN;

	heightMapTexture.GetDimensions(textureWidth, textureHeight);

	texOffset = float3((1.0f / (textureWidth)), (1.0f / (textureHeight)), 0.0f);

	hL = GetHeight((uv - texOffset.xz), insideAtmosphere, pos);
	hR = GetHeight((uv + texOffset.xz), insideAtmosphere, pos);
	hD = GetHeight((uv + texOffset.zy), insideAtmosphere, pos);
	hU = GetHeight((uv - texOffset.zy), insideAtmosphere, pos);

	float4 detailAreaX = detailAreaMapTextureX.SampleLevel(sampleType, uv, 0).rgba;

	if (round(detailAreaX.a * 255.0f) == 255.0f)
	{
		craterHeightMapTexture.GetDimensions(craterDetailTextureWidth, craterDetailTextureHeight);

		texOffsetCraterDetail = float3((1.0f / (craterDetailTextureWidth)), (1.0f / (craterDetailTextureHeight)), 0.0f);

		float4 detailAreaY = detailAreaMapTextureY.SampleLevel(sampleType, uv, 0).rgba;
		float4 detailAreaWH = detailAreaMapTextureWH.SampleLevel(sampleType, uv, 0).rgba;

		craterDetailUV = uv - float2((((round(detailAreaX.r * 255.0f) * round(detailAreaX.g * 255.0f)) + round(detailAreaX.b * 255.0f)) / 8192.0f), (((round(detailAreaY.r * 255.0f) * round(detailAreaY.g * 255.0f)) + round(detailAreaY.b * 255.0f)) / 4096.0f));
		craterDetailUV = float2((craterDetailUV.x * 8192.0f), (craterDetailUV.y * 4096.0f)) / (round(detailAreaWH.r * 255.0f));

		hL2 = GetCraterDetailHeight((craterDetailUV - texOffsetCraterDetail.xz), insideAtmosphere, pos);
		hR2 = GetCraterDetailHeight((craterDetailUV + texOffsetCraterDetail.xz), insideAtmosphere, pos);
		hD2 = GetCraterDetailHeight((craterDetailUV + texOffsetCraterDetail.zy), insideAtmosphere, pos);
		hU2 = GetCraterDetailHeight((craterDetailUV - texOffsetCraterDetail.zy), insideAtmosphere, pos);

		N = normalize(float3((hL2 - hR2), (hU2 - hD2), 2.0f));
	}
	else if (round(detailAreaX.a * 255.0f) == 254.0f)
	{
		crater2HeightMapTexture.GetDimensions(craterDetailTextureWidth, craterDetailTextureHeight);

		texOffsetCraterDetail = float3((1.0f / (craterDetailTextureWidth)), (1.0f / (craterDetailTextureHeight)), 0.0f);

		float4 detailAreaY = detailAreaMapTextureY.SampleLevel(sampleType, uv, 0).rgba;
		float4 detailAreaWH = detailAreaMapTextureWH.SampleLevel(sampleType, uv, 0).rgba;

		craterDetailUV = uv - float2((((round(detailAreaX.r * 255.0f) * round(detailAreaX.g * 255.0f)) + round(detailAreaX.b * 255.0f)) / 8192.0f), (((round(detailAreaY.r * 255.0f) * round(detailAreaY.g * 255.0f)) + round(detailAreaY.b * 255.0f)) / 4096.0f));
		craterDetailUV = float2((craterDetailUV.x * 8192.0f), (craterDetailUV.y * 4096.0f)) / (round(detailAreaWH.r * 255.0f));

		hL2 = GetCrater2DetailHeight((craterDetailUV - texOffsetCraterDetail.xz), insideAtmosphere, pos);
		hR2 = GetCrater2DetailHeight((craterDetailUV + texOffsetCraterDetail.xz), insideAtmosphere, pos);
		hD2 = GetCrater2DetailHeight((craterDetailUV + texOffsetCraterDetail.zy), insideAtmosphere, pos);
		hU2 = GetCrater2DetailHeight((craterDetailUV - texOffsetCraterDetail.zy), insideAtmosphere, pos);

		N = normalize(float3((hL2 - hR2), (hU2 - hD2), 2.0f));
	}
	else
	{
		N = normalize(float3((hL - hR), (hU - hD), 2.0f));
	}

	TBN = GetTBNMatrix(normalVector, -viewVector, uv);

	return mul(N, TBN);	
}

float4 MarsFromSpacePixelShader(PixelInputType input) : SV_TARGET
{
	float3 normal;
	float lightIntensity, lightColor, increasedLigthningFactor;
	float4 finalColor, color;

	normal = normalize(CalculateNormal(normalize(input.normal), normalize(input.viewVector), input.mapCoord, false, input.originalPosition));

	lightIntensity = saturate(dot(normal, normalize(mul(lightDirection, rotationMatrix))));

	lightColor = saturate(diffuseColor * lightIntensity);

	// DETAIL AREA TEST
	float4 detailArea;

	detailArea = detailAreaMapTextureX.SampleLevel(sampleType, input.mapCoord, 0).rgba;

	color = colorMapTexture.SampleLevel(sampleType, input.mapCoord, 0).rgba; //float4(0.86f, 0.55f, 0.25f, 1.0f);// 

	// To use when the color is decided by the GPU
	//color.r *= 7.0f;
	//color.g *= 4.0f;
	//color.b *= 3.0f;

	// To be used when the color is decided by a texture
	color.r *= 7.0f;
	color.g *= 6.0f;
	color.b *= 3.0f;

	// Debugging code to get the UV coordinates colored patches
	//float4 detailAreaX = detailAreaMapTextureX.SampleLevel(sampleType, input.mapCoord, 0).rgba;
	//if (round(detailAreaX.a * 255.0f) == 255.0f)
	//{
	//	float4 detailAreaY = detailAreaMapTextureY.SampleLevel(sampleType, input.mapCoord, 0).rgba;
	//	float4 detailAreaWH = detailAreaMapTextureWH.SampleLevel(sampleType, input.mapCoord, 0).rgba;

	//	float2 craterMapping = input.mapCoord - float2((((round(detailAreaX.r * 255.0f) * round(detailAreaX.g * 255.0f)) + round(detailAreaX.b * 255.0f)) / 8192.0f), (((round(detailAreaY.r * 255.0f) * round(detailAreaY.g * 255.0f)) + round(detailAreaY.b * 255.0f)) / 4096.0f));
	//	craterMapping = (float2((craterMapping.x * 8192.0f), (craterMapping.y * 4096.0f)) / (round(detailAreaWH.r * 255.0f)));

	//	finalColor = float4(craterMapping.x, craterMapping.y, 0.0f, 1.0f);
	//}
	//else if (round(detailAreaX.a * 255.0f) == 254.0f) 
	//{
	//	float4 detailAreaY = detailAreaMapTextureY.SampleLevel(sampleType, input.mapCoord, 0).rgba;
	//	float4 detailAreaWH = detailAreaMapTextureWH.SampleLevel(sampleType, input.mapCoord, 0).rgba;

	//	float2 craterMapping = input.mapCoord - float2((((round(detailAreaX.r * 255.0f) * round(detailAreaX.g * 255.0f)) + round(detailAreaX.b * 255.0f)) / 8192.0f), (((round(detailAreaY.r * 255.0f) * round(detailAreaY.g * 255.0f)) + round(detailAreaY.b * 255.0f)) / 4096.0f));
	//	craterMapping = (float2((craterMapping.x * 8192.0f), (craterMapping.y * 4096.0f)) / (round(detailAreaWH.r * 255.0f)));

	//	finalColor = float4(craterMapping.x, craterMapping.y, 0.0f, 1.0f);
	//}
	//else 
	//{
	//	finalColor = input.color * color * input.secondColor * lightIntensity * 5.0f;
	//}

	finalColor = input.color * color * input.secondColor * lightIntensity * 5.0f;

	return finalColor; //(color * increasedLigthningFactor) * lightColor; //
}