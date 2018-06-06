#include "TextureClass.h"

TextureClass::TextureClass() 
{
	mTargaData = 0;
	mTexture = 0;
	mTextureView = 0;
}

TextureClass::TextureClass(const TextureClass& other)
{
}

TextureClass::~TextureClass() 
{
}

bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;
	int height, int width;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	result = LoadTarga(filename, height, width);
	if (!result) 
	{
		return false;
	}

	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	hResult = device->CreateTexture2D(&textureDesc, NULL, &mTexture);
	if (FAILED(hResult)) 
	{
		return false;
	}

	rowPitch = (width * 4) * sizeof(unsigned char);

	deviceContext->UpdateSubresource(mTexture, 0, NULL, mTargaData, rowPitch, 0);

	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hResult = device->CreateShaderResourceView(mTexture, &srvDesc, &mTextureView);
	if (FAILED(hResult))
	{
		return false;
	}

	deviceContext->GenerateMips(mTextureView);

	delete[] mTargaData;
	mTargaData = 0;

	return true;
}

void TextureClass::Shutdown()
{
	if (mTextureView)
	{
		mTextureView->Release();
		mTextureView = 0;
	}

	if (mTexture)
	{
		mTexture->Release();
		mTexture = 0;
	}

	if (mTargaData)
	{
		delete[] mTargaData;
		mTargaData = 0;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture() 
{
	return mTextureView;
}

bool TextureClass::LoadTarga(char* filename, int& height, int& width)
{
	int error, bpp, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	if (bpp != 32)
	{
		return false;
	}

	imageSize = width * height * 4;

	targaImage = new unsigned char[imageSize];
	if (!targaImage)
	{
		return false;
	}

	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if(count != imageSize)
	{
		return false;
	}

	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	mTargaData = new unsigned char[imageSize];
	if (!mTargaData) 
	{
		return false;
	}

	index = 0;

	k = (width * height * 4) - (width * 4);

	for (j = 0; j < height; j++)
	{
		for(i = 0; i < width; i++)
		{
			mTargaData[index + 0] = targaImage[k + 2];
			mTargaData[index + 1] = targaImage[k + 1];
			mTargaData[index + 2] = targaImage[k + 0];
			mTargaData[index + 3] = targaImage[k + 3];

			k += 4;
			index += 4;
		}

		k -= (width * 8);
	}

	delete[] targaImage;
	targaImage = 0;

	return true;
}