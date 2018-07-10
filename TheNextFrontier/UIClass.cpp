#include "UIClass.h"

UIClass::UIClass() 
{
	mFont1 = 0;
	mFPSString = 0;
	mVideoStrings = 0;
	mPositionStrings = 0;
	mVerticesString = 0;
	mAltitudeString = 0;
	mDistFromOrigoString = 0;
	mMarsHeightString = 0;
	mGameTimeString = 0;
}

UIClass::UIClass(const UIClass& other) 
{
}

UIClass::~UIClass() 
{
}

bool UIClass::Initialize(HWND hwnd, D3DClass* direct3D, int screenHeight, int screenWidth)
{
	bool result;
	char videoCard[128];
	int videoMemory;
	char videoString[144];
	char memoryString[32];
	char altitudeString[32];
	char distFromOrigoString[60];
	char marsHeightString[32];
	char tempString[16];
	int i;

	mFont1 = new FontClass;
	if (!mFont1)
	{
		return false;
	}

	result = mFont1->Initialize(direct3D->GetDevice(), direct3D->GetDeviceContext(), (char*)"../TheNextFrontier/font01.txt", (char*)"../TheNextFrontier/font01.tga", 32.0f, 3);
	if (!result)
	{
		return false;
	}

	mFPSString = new TextClass;
	if (!mFPSString)
	{
		return false;
	}

	result = mFPSString->Initialize(hwnd, direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, mFont1, (char*)"FPS: 0", 10, 50, 0.0f, 1.0f, 0.0f);
	if (!result)
	{
		return false;
	}

	mPreviousFPS = -1;

	direct3D->GetVideoCardInfo(videoCard, videoMemory);
	strcpy_s(videoString, "Video Card: ");
	strcat_s(videoString, videoCard);

	_itoa_s(videoMemory, tempString, 10);

	strcpy_s(memoryString, "Video Memory: ");
	strcat_s(memoryString, tempString);
	strcat_s(memoryString, " MB");

	mVideoStrings = new TextClass[2];
	if (!mVideoStrings)
	{
		return false;
	}

	result = mVideoStrings[0].Initialize(hwnd, direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, 256, false, mFont1, videoString, 10, 10, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = mVideoStrings[1].Initialize(hwnd, direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, false, mFont1, memoryString, 10, 30, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	mPositionStrings = new TextClass[6];
	if (!mVideoStrings)
	{
		return false;
	}

	result = mPositionStrings[0].Initialize(hwnd, direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, mFont1, (char*)"X: 0", 10, 310, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = mPositionStrings[1].Initialize(hwnd, direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, mFont1, (char*)"Y: 0", 10, 330, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = mPositionStrings[2].Initialize(hwnd, direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, mFont1, (char*)"Z: 0", 10, 350, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = mPositionStrings[3].Initialize(hwnd, direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, mFont1, (char*)"rX: 0", 10, 370, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = mPositionStrings[4].Initialize(hwnd, direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, mFont1, (char*)"rY: 0", 10, 390, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = mPositionStrings[5].Initialize(hwnd, direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, mFont1, (char*)"rZ: 0", 10, 410, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	for (i = 0; i < 6; i++)
	{
		mPreviousPosition[i] = -1;
	}

	mVerticesString = new TextClass;

	result = mVerticesString->Initialize(hwnd, direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, 40, false, mFont1, (char*)"Vertices rendered: 0", 10, 440, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	mAltitudeString = new TextClass;

	strcpy_s(altitudeString, "Altitude: ");
	strcat_s(altitudeString, tempString);
	strcat_s(altitudeString, " km");

	result = mAltitudeString->Initialize(hwnd, direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, 40, false, mFont1, altitudeString, 10, 460, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	mDistFromOrigoString = new TextClass;

	strcpy_s(distFromOrigoString, "Distance from origo: ");
	strcat_s(distFromOrigoString, tempString);
	strcat_s(distFromOrigoString, " km");

	result = mDistFromOrigoString->Initialize(hwnd, direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, 60, false, mFont1, distFromOrigoString, 10, 480, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	mMarsHeightString = new TextClass;

	strcpy_s(marsHeightString, "Mars height: ");
	strcat_s(marsHeightString, tempString);
	strcat_s(marsHeightString, " km");

	result = mMarsHeightString->Initialize(hwnd, direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, 40, false, mFont1, marsHeightString, 10, 500, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	mGameTimeString = new TextClass;

	result = mGameTimeString->Initialize(hwnd, direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, 40, false, mFont1, (char*)"M: 0 Sol: 0 Time: 00:00:00", 10, 520, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	return true;
}

void UIClass::Shutdown() 
{
	if (mVerticesString)
	{
		mVerticesString->Shutdown();
		delete mVerticesString;
		mVerticesString = 0;
	}

	if (mPositionStrings) 
	{
		mPositionStrings[0].Shutdown();
		mPositionStrings[1].Shutdown();
		mPositionStrings[2].Shutdown();
		mPositionStrings[3].Shutdown();
		mPositionStrings[4].Shutdown();
		mPositionStrings[5].Shutdown();

		delete[] mPositionStrings;
		mPositionStrings = 0;
	}

	if (mVideoStrings) 
	{
		mVideoStrings[0].Shutdown();
		mVideoStrings[1].Shutdown();
		delete[] mVideoStrings;
		mVideoStrings = 0;
	}

	if (mFPSString) 
	{
		mFPSString->Shutdown();
		delete mFPSString;
		mFPSString = 0;
	}

	if (mGameTimeString)
	{
		mGameTimeString->Shutdown();
		delete mGameTimeString;
		mGameTimeString = 0;
	}

	if (mFont1)
	{
		mFont1->Shutdown();
		delete mFont1;
		mFont1 = 0;
	}

	return;
}

bool UIClass::Frame(HWND hwnd, ID3D11DeviceContext* deviceContext, int fps, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, int verticesNr, float altitude, float distFromOrigo, float marsHeight, int gameTimeSecs, int gameTimeMins, int gameTimeHours, int gameTimeDays, int gameTimeMarsYears)
{
	bool result;

	result = UpdateFPSString(hwnd, deviceContext, fps);
	if (!result)
	{
		return false;
	}

	result = UpdatePositionStrings(hwnd, deviceContext, posX, posY, posZ, rotX, rotY, rotZ);
	if (!result)
	{
		return false;
	}

	result = UpdateVerticesString(hwnd, deviceContext, verticesNr);
	if (!result)
	{
		return false;
	}

	result = UpdateAltitudeString(hwnd, deviceContext, altitude);
	if (!result)
	{
		return false;
	}

	result = UpdateDistFromOrigoString(hwnd, deviceContext, distFromOrigo);
	if (!result)
	{
		return false;
	}

	result = UpdateMarsHeightString(hwnd, deviceContext, marsHeight);
	if (!result)
	{
		return false;
	}

	result = UpdateGameTimeString(hwnd, deviceContext, gameTimeSecs, gameTimeMins, gameTimeHours, gameTimeDays, gameTimeMarsYears);
	if (!result)
	{
		return false;
	}

	return true;
}

bool UIClass::Render(D3DClass* direct3D, ShaderManagerClass* shaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX orthoMatrix) 
{
	mFPSString->Render(direct3D->GetDeviceContext(), shaderManager, worldMatrix, viewMatrix, orthoMatrix, mFont1->GetTexture());

	mVideoStrings[0].Render(direct3D->GetDeviceContext(), shaderManager, worldMatrix, viewMatrix, orthoMatrix, mFont1->GetTexture());
	mVideoStrings[1].Render(direct3D->GetDeviceContext(), shaderManager, worldMatrix, viewMatrix, orthoMatrix, mFont1->GetTexture());

	for (int i = 0; i < 6; i++)
	{
		mPositionStrings[i].Render(direct3D->GetDeviceContext(), shaderManager, worldMatrix, viewMatrix, orthoMatrix, mFont1->GetTexture());
	}

	mVerticesString->Render(direct3D->GetDeviceContext(), shaderManager, worldMatrix, viewMatrix, orthoMatrix, mFont1->GetTexture());
	mAltitudeString->Render(direct3D->GetDeviceContext(), shaderManager, worldMatrix, viewMatrix, orthoMatrix, mFont1->GetTexture());
	mDistFromOrigoString->Render(direct3D->GetDeviceContext(), shaderManager, worldMatrix, viewMatrix, orthoMatrix, mFont1->GetTexture());
	mMarsHeightString->Render(direct3D->GetDeviceContext(), shaderManager, worldMatrix, viewMatrix, orthoMatrix, mFont1->GetTexture());

	mGameTimeString->Render(direct3D->GetDeviceContext(), shaderManager, worldMatrix, viewMatrix, orthoMatrix, mFont1->GetTexture());

	return true;
}

bool UIClass::UpdateFPSString(HWND hwnd, ID3D11DeviceContext* deviceContext, int fps) 
{
	char tempString[16];
	char finalString[16];
	float red, green, blue;
	bool result;

	if (mPreviousFPS == fps)
	{
		return true;
	}

	mPreviousFPS = fps;

	if (fps > 99999) 
	{
		fps = 99999;
	}

	_itoa_s(fps, tempString, 10);

	strcpy_s(finalString, "FPS: ");
	strcat_s(finalString, tempString);

	if (fps >= 60) 
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	result = mFPSString->UpdateSentence(hwnd, deviceContext, mFont1, finalString, 10, 50, red, green, blue);
	if (!result)
	{
		return false;
	}

	return true;
}

bool UIClass::UpdatePositionStrings(HWND hwnd, ID3D11DeviceContext* deviceContext, float posX, float posY, float posZ, float rotX, float rotY, float rotZ) 
{
	int positionX, positionY, positionZ, rotationX, rotationY, rotationZ;
	char tempString[16];
	char finalString[16];
	bool result;

	positionX = (int)posX;
	positionY = (int)posY;
	positionZ = (int)posZ;
	rotationX = (int)rotX;
	rotationY = (int)rotY;
	rotationZ = (int)rotZ;

	if (positionX != mPreviousPosition[0])
	{
		mPreviousPosition[0] = positionX;
		_itoa_s(positionX, tempString, 10);
		strcpy_s(finalString, "X: ");
		strcat_s(finalString, tempString);

		result = mPositionStrings[0].UpdateSentence(hwnd, deviceContext, mFont1, finalString, 10, 100, 1.0f, 1.0f, 1.0f);
		if (!result) 
		{
			return false;
		}
	}

	if (positionY != mPreviousPosition[1])
	{
		mPreviousPosition[1] = positionY;
		_itoa_s(positionY, tempString, 10);
		strcpy_s(finalString, "Y: ");
		strcat_s(finalString, tempString);

		result = mPositionStrings[1].UpdateSentence(hwnd, deviceContext, mFont1, finalString, 10, 120, 1.0f, 1.0f, 1.0f);
		if (!result)
		{
			return false;
		}
	}

	if (positionZ != mPreviousPosition[2])
	{
		mPreviousPosition[2] = positionZ;
		_itoa_s(positionZ, tempString, 10);
		strcpy_s(finalString, "Z: ");
		strcat_s(finalString, tempString);

		result = mPositionStrings[2].UpdateSentence(hwnd, deviceContext, mFont1, finalString, 10, 140, 1.0f, 1.0f, 1.0f);
		if (!result)
		{
			return false;
		}
	}

	if (rotationX != mPreviousPosition[3])
	{
		mPreviousPosition[3] = rotationX;
		_itoa_s(rotationX, tempString, 10);
		strcpy_s(finalString, "rX: ");
		strcat_s(finalString, tempString);

		result = mPositionStrings[3].UpdateSentence(hwnd, deviceContext, mFont1, finalString, 10, 180, 1.0f, 1.0f, 1.0f);
		if (!result)
		{
			return false;
		}
	}

	if (rotationY!= mPreviousPosition[4])
	{
		mPreviousPosition[4] = rotationY;
		_itoa_s(rotationY, tempString, 10);
		strcpy_s(finalString, "rY: ");
		strcat_s(finalString, tempString);

		result = mPositionStrings[4].UpdateSentence(hwnd, deviceContext, mFont1, finalString, 10, 200, 1.0f, 1.0f, 1.0f);
		if (!result)
		{
			return false;
		}
	}

	if (rotationZ != mPreviousPosition[5])
	{
		mPreviousPosition[5] = rotationZ;
		_itoa_s(rotationZ, tempString, 10);
		strcpy_s(finalString, "rZ: ");
		strcat_s(finalString, tempString);

		result = mPositionStrings[5].UpdateSentence(hwnd, deviceContext, mFont1, finalString, 10, 220, 1.0f, 1.0f, 1.0f);
		if (!result)
		{
			return false;
		}
	}

	return true;
}

bool UIClass::UpdateVerticesString(HWND hwnd, ID3D11DeviceContext* deviceContext, int verticesNr)
{
	bool result;
	char tempString[40];
	char finalString[40];

	_itoa_s(verticesNr, tempString, 10);

	strcpy_s(finalString, "Vertices rendered: ");
	strcat_s(finalString, tempString);

	result = mVerticesString->UpdateSentence(hwnd, deviceContext, mFont1, finalString, 10, 260, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	return true;
}

bool UIClass::UpdateAltitudeString(HWND hwnd, ID3D11DeviceContext* deviceContext, float altitude)
{
	bool result;
	char tempString[40];
	char finalString[40];

	_itoa_s(altitude, tempString, 10);

	strcpy_s(finalString, "Altitude: ");
	strcat_s(finalString, tempString);
	strcat_s(finalString, " km");

	result = mAltitudeString->UpdateSentence(hwnd, deviceContext, mFont1, finalString, 10, 280, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	return true;
}

bool UIClass::UpdateDistFromOrigoString(HWND hwnd, ID3D11DeviceContext* deviceContext, float distFromOrigo)
{
	bool result;
	char tempString[60];
	char finalString[60];

	_itoa_s(distFromOrigo, tempString, 10);

	strcpy_s(finalString, "Distance from origo: ");
	strcat_s(finalString, tempString);
	strcat_s(finalString, " km");

	result = mDistFromOrigoString->UpdateSentence(hwnd, deviceContext, mFont1, finalString, 10, 300, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	return true;
}

bool UIClass::UpdateMarsHeightString(HWND hwnd, ID3D11DeviceContext* deviceContext, float marsHeight)
{
	bool result;
	char tempString[60];
	char finalString[60];

	_itoa_s(marsHeight, tempString, 10);

	strcpy_s(finalString, "Mars height: ");
	strcat_s(finalString, tempString);
	strcat_s(finalString, " km");

	result = mMarsHeightString->UpdateSentence(hwnd, deviceContext, mFont1, finalString, 10, 320, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	return true;
}

bool UIClass::UpdateGameTimeString(HWND hwnd, ID3D11DeviceContext *deviceContext, int gameTimeSecs, int gameTimeMins, int gameTimeHours, int gameTimeDays, int gameTimeYears)
{
	bool result;
	char tempString[16];
	char finalString[40];

	if (gameTimeSecs != mPreviousGameTimeSec)
	{
		_itoa_s(gameTimeYears, tempString, 10);
		strcpy_s(finalString, "M: ");
		strcat_s(finalString, tempString);

		_itoa_s(gameTimeDays, tempString, 10);
		strcat_s(finalString, " Sol: ");
		strcat_s(finalString, tempString);

		strcat_s(finalString, " Time: ");

		if (gameTimeHours < 10)
		{
			_itoa_s(gameTimeHours, tempString, 10);
			strcat_s(finalString, " 0");
			strcat_s(finalString, tempString);
		}
		else
		{
			_itoa_s(gameTimeHours, tempString, 10);
			strcat_s(finalString, " ");
			strcat_s(finalString, tempString);
		}

		if (gameTimeMins < 10)
		{
			_itoa_s(gameTimeMins, tempString, 10);
			strcat_s(finalString, ":0");
			strcat_s(finalString, tempString);
		}
		else
		{
			_itoa_s(gameTimeMins, tempString, 10);
			strcat_s(finalString, ":");
			strcat_s(finalString, tempString);
		}

		if (gameTimeSecs < 10)
		{
			_itoa_s(gameTimeSecs, tempString, 10);
			strcat_s(finalString, ":0");
			strcat_s(finalString, tempString);
		}
		else
		{
			_itoa_s(gameTimeSecs, tempString, 10);
			strcat_s(finalString, ":");
			strcat_s(finalString, tempString);
		}

		mPreviousGameTimeSec = gameTimeSecs;

		result = mGameTimeString->UpdateSentence(hwnd, deviceContext, mFont1, finalString, 2300, 17, 1.0f, 1.0f, 1.0f);
		if (!result)
		{
			return false;
		}
	}

	return true;
}