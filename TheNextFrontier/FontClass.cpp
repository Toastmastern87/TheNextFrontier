#include "FontClass.h"

FontClass::FontClass() 
{
	mFont = 0;
	mTexture = 0;
}

FontClass::FontClass(const FontClass& other) 
{
}

FontClass::~FontClass() 
{
}

bool FontClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fontFilename, char* textureFilename, float fontHeight, int spaceSize)
{
	bool result;

	mFontHeight = fontHeight;

	mSpaceSize = spaceSize;

	result = LoadFontData(fontFilename);
	if (!result) 
	{
		return false;
	}

	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown() 
{
	ReleaseTexture();

	ReleaseFontData();

	return;
}

bool FontClass::LoadFontData(char* filename) 
{
	ifstream fin;
	int i;
	char temp;

	mFont = new FontType[95];
	if (!mFont) 
	{
		return false;
	}

	fin.open(filename);
	if (fin.fail()) 
	{
		return false;
	}

	for (i = 0; i < 95; i++) 
	{
		fin.get(temp);
		while (temp != ' ') 
		{
			fin.get(temp);
		}

		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> mFont[i].left;
		fin >> mFont[i].right;
		fin >> mFont[i].size;
	}

	fin.close();

	return true;
}

void FontClass::ReleaseFontData() 
{
	if (mFont) 
	{
		delete[] mFont;
		mFont = 0;
	}

	return;
}

bool FontClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;

	mTexture = new TextureClass();
	if (!mTexture)
	{
		return false;
	}

	result = mTexture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture() 
{
	if (mTexture)
	{
		mTexture->Shutdown();
		delete mTexture;
		mTexture = 0;
	}

	return;
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return mTexture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY) 
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;

	vertexPtr = (VertexType*)vertices;

	numLetters = (int)strlen(sentence);

	for (i = 0; i < numLetters; i++) 
	{
		letter = ((int)sentence[i]) - 32;

		if (letter == 0)
		{
			drawX = drawX + (float)mSpaceSize;
		}
		else
		{
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(mFont[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + mFont[letter].size), (drawY - mFontHeight), 0.0f);
			vertexPtr[index].texture = XMFLOAT2(mFont[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - mFontHeight), 0.0f);
			vertexPtr[index].texture = XMFLOAT2(mFont[letter].left, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(mFont[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + mFont[letter].size), drawY, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(mFont[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + mFont[letter].size), (drawY - mFontHeight), 0.0f);
			vertexPtr[index].texture = XMFLOAT2(mFont[letter].right, 1.0f);
			index++;

			drawX = drawX + mFont[letter].size + 1.0f;
		}
	}

	return;
}

int FontClass::GetSentencePixelLength(char* sentence) 
{
	int pixelLength, numLetters, i, letter;

	pixelLength = 0;
	numLetters = (int)strlen(sentence);

	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		if (letter == 0) 
		{
			pixelLength += mSpaceSize;
		}
		else
		{
			pixelLength += (mFont[letter].size + 1);
		}
	}

	return pixelLength;
}

int FontClass::GetFontHeight() 
{
	return (int)mFontHeight;
}