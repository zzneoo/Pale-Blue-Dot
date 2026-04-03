#pragma once
#include"MyOpenGL.h"
//#include"gl/wglext.h" //V-Sync
#include"MyLog.h"

struct CamStruct
{
	BOOL bCameraMoving;
	WPARAM PressedKey;
	USHORT mouseX;
	USHORT mouseY;
	GLuint CameraTurboSpeed;
};

struct ClientSize
{
	USHORT ClientWidth;
	USHORT ClientHeight;
};



class MyWin32
{
public:

	MyWin32(HINSTANCE hInstance, TCHAR * className, int iCmdShow);
	~MyWin32();
	
	//functions
	MSG StartGameLoop(void);	

	//variables
	static vmath::mat4 gProjectionMatrix;
	static vmath::mat4 gInvProjectionMatrix;

	static vmath::mat4 gViewMatrix;
	static vmath::mat4 gPreviousViewMatrix;
	static vmath::mat4 gInvViewMatrix;

	static vmath::mat4 gJitteredProjectionMatrix;
	static vmath::mat4 gPreviousJitteredProjectionMatrix;

	static vmath::mat4 gLightViewProjectionMatrix;
	static vmath::mat4 gLightOrthoProjectionMatrix;
	static vmath::mat4 gProjectionMatrixForDepth;
	static vmath::mat4 gViewProjectionMatrixForDepth;
	static vmath::mat4 gViewProjectionMatrix;

	static vmath::mat4 gPreviousInfiniteViewMatrix;

	static vmath::vec3 gPrevCameraPosition;
	static vmath::dvec2 gNearFarFrustum;
	static GLfloat fovY;

	static CamStruct myCamStruct;
	static ClientSize myClientSize;
	static 	BOOL bFirstFrame;
	static 	BOOL bRecentResize;
	static BOOL isGUI;
	static BOOL addObject;
	static BOOL StartDemo;
	static bool TAA;
	static BOOL bToggleSpaceGroundSpeed;
	static unsigned int iFrameID;
	static unsigned char iBlockPixelID;

	//V-Sync

	//PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT = NULL;
	//PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT = NULL;

	//bool WGLExtensionSupported(const char* extension_name)
	//{
	//	// this is pointer to function which returns pointer to string with list of all wgl extensions
	//	PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;

	//	// determine pointer to wglGetExtensionsStringEXT function
	//	_wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");

	//	if (strstr(_wglGetExtensionsStringEXT(), extension_name) == NULL)
	//	{
	//		// string was not found
	//		return false;
	//	}

	//	// extension is supported
	//	return true;
	//}
		
private:
	//functions
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	void InitMyWin32(TCHAR* className, HINSTANCE hInstance, int iCmdShow);
	static void ToggleFullScreen(HWND hwnd);
	static void Resize(int width, int height);
	void Uninitialize(void);

	//class members
	BOOL gameloopStarted;
	HWND hwnd;
	const char* logFileName = "../Log/myWin32log.txt";
	MyLog MyWin32Log;
	
	MyOpenGL myOpenGL;
			
};








