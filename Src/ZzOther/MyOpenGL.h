#pragma once

#include"MyLog.h"
#include"Camera.h"
#include"MyTGA_Loader.h"
#include"Noise.h"

#include"RayCaster.h"
#include"ZzRenderer.h"

#include"imgui/imgui.h"
#include"imgui/imgui_impl_win32.h"
#include"imgui/imgui_impl_opengl3.h"

#include <Windows.h>

#define TEMPORAL_UPSCALE_8K

class MyOpenGL
{
public:

	MyOpenGL();
	~MyOpenGL() ;

	// Functions
	bool InitializeOpenGL(HWND hwndFromWin32);
	bool InitializeAfterGLEW(void);
	void RenderScene(void);
	//functions
	void CleanUp(void);
	//Members
	static GLfloat fDeltaTime;
	static GLfloat fTime;
	static GLfloat fExposure;
	static bool bShaderInitStatus;
	static bool bObject3DInitStatus;

	HDC getHDC(void)
	{
		return hdc;
	}

private:
	//variables
	HWND hwnd;
	HDC hdc;
	HGLRC hrc;

	//Renderer
	ZzRenderer *Renderer;

};
