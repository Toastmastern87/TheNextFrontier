#ifndef _GRAPHICSCLASS_H_

#define _GRAPHICSCLASS_H_

#include <Windows.h>

const bool FULLSCREEN = false;
const bool VSYNCENABLED = true;
const float SCREENDEPTH = 1000.0f;
const float SCREENEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:

};

#endif