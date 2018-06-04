#include "InputClass.h"

InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
	int i;

	for (i = 0; i < 256; i++)
	{
		mKeys[i] = false;
	}

	return;
}

void InputClass::KeyDown(unsigned int input)
{
	mKeys[input] = true;
}

void InputClass::KeyUp(unsigned int input)
{
	mKeys[input] = false;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return mKeys[key];
}