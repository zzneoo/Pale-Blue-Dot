#pragma once
#include"stdafx.h"
#include"MyWin32.h"

CamStruct MyWin32::myCamStruct = {};
ClientSize  MyWin32::myClientSize = {};
BOOL MyWin32::bFirstFrame=TRUE;
BOOL MyWin32::isGUI = TRUE;
BOOL MyWin32::addObject = FALSE;
BOOL MyWin32::StartDemo = FALSE;
bool MyWin32::TAA = true;//for imGui
BOOL MyWin32::bToggleSpaceGroundSpeed = TRUE;//for imGui
BOOL MyWin32::bRecentResize = FALSE;
unsigned int MyWin32::iFrameID = 0;
unsigned char MyWin32::iBlockPixelID = 0;

vmath::mat4 MyWin32::gProjectionMatrix = vmath::mat4::identity();
vmath::mat4 MyWin32::gInvProjectionMatrix = vmath::mat4::identity();

vmath::mat4 MyWin32::gViewMatrix = vmath::mat4::identity();
vmath::mat4 MyWin32::gInvViewMatrix = vmath::mat4::identity();

vmath::mat4 MyWin32::gJitteredProjectionMatrix = vmath::mat4::identity();
vmath::mat4 MyWin32::gPreviousJitteredProjectionMatrix = vmath::mat4::identity();

vmath::mat4 MyWin32::gLightViewProjectionMatrix = vmath::mat4::identity();
vmath::mat4 MyWin32::gLightOrthoProjectionMatrix = vmath::mat4::identity();
vmath::mat4 MyWin32::gProjectionMatrixForDepth = vmath::mat4::identity();
vmath::mat4 MyWin32::gViewProjectionMatrixForDepth = vmath::mat4::identity();
vmath::mat4 MyWin32::gViewProjectionMatrix = vmath::mat4::identity();
vmath::mat4 MyWin32::gPreviousViewMatrix = vmath::mat4::identity();
vmath::mat4 MyWin32::gPreviousInfiniteViewMatrix = vmath::mat4::identity();

vmath::vec3 MyWin32::gPrevCameraPosition = vmath::vec3(0.0);
vmath::dvec2 MyWin32::gNearFarFrustum = vmath::dvec2(0.0);
GLfloat MyWin32::fovY = 70.0f;

IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

MyWin32::MyWin32(HINSTANCE hInstance, TCHAR * className,int iCmdShow)
{
		
	MyWin32Log.StartLog(this->logFileName);

	MyWin32::gameloopStarted = FALSE;//for exit() in uninitialize

	this->InitMyWin32(className, hInstance, iCmdShow);//we get hwnd here

	myCamStruct = {FALSE,0,(USHORT)(MyWin32::myClientSize.ClientWidth/2),(USHORT)(MyWin32::myClientSize.ClientHeight / 2) ,FALSE};

	if (!myOpenGL.InitializeOpenGL(this->hwnd))
	{
		this->Uninitialize();
	}

	//for FullScreen
	SendMessage(this->hwnd, WM_KEYDOWN, 0x46, 0);
	//V-Sync

	//if (WGLExtensionSupported("WGL_EXT_swap_control"))
	//{
	//	// Extension is supported, init pointers.
	//	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

	//	// this is another function from WGL_EXT_swap_control extension
	//	wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
	//}

	//wglSwapIntervalEXT(1);

}
MyWin32::~MyWin32()
{
//	this->Uninitialize();
}


LRESULT CALLBACK MyWin32::WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int Width;
	static int Height;

	if (ImGui_ImplWin32_WndProcHandler(hwnd, iMsg, wParam, lParam))
		return true;

	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_ERASEBKGND:
		return(0);

	case WM_SIZE:
		Width = LOWORD(lParam);
		Height = HIWORD(lParam);

		MyWin32::myClientSize.ClientWidth = Width;
		MyWin32::myClientSize.ClientHeight = Height;

		MyWin32::Resize(Width, Height);
		break;

	case WM_MOUSEMOVE:
		MyWin32::myCamStruct.mouseX = GET_X_LPARAM(lParam);
		MyWin32::myCamStruct.mouseY = GET_Y_LPARAM(lParam);
		break;


	case WM_KEYDOWN:

		switch (wParam)
		{
		case 0x30:
			MyWin32::myCamStruct.CameraTurboSpeed = 0;
			break;
		case 0x31:
			MyWin32::myCamStruct.CameraTurboSpeed = 1;
			break;
		case 0x32:
			MyWin32::myCamStruct.CameraTurboSpeed = 2;
			break;
		case 0x33:
			MyWin32::myCamStruct.CameraTurboSpeed = 3;
			break;
		case 0x34:
			MyWin32::myCamStruct.CameraTurboSpeed = 4;
			break;
		case 0x35:
			MyWin32::myCamStruct.CameraTurboSpeed = 5;
			break;
		case 0x36:
			MyWin32::myCamStruct.CameraTurboSpeed = 6;
			break;
		case 0x37:
			MyWin32::myCamStruct.CameraTurboSpeed = 7;
			break;
		case 0x38:
			MyWin32::myCamStruct.CameraTurboSpeed = 8;
			break;
		case 0x39:
			MyWin32::myCamStruct.CameraTurboSpeed = 9;
			break;

		case VK_NUMPAD0:
			if (MyWin32::bToggleSpaceGroundSpeed == TRUE)
				MyWin32::bToggleSpaceGroundSpeed = FALSE;
			else
				MyWin32::bToggleSpaceGroundSpeed = TRUE;
			break;

		case 0x46://F
			MyWin32::ToggleFullScreen(hwnd);
			MyWin32::bFirstFrame = TRUE;
			break;

		case 0x57: //W
			MyWin32::myCamStruct.bCameraMoving = TRUE;
			MyWin32::myCamStruct.PressedKey = wParam;
			break;

		case 0x41: //A
			MyWin32::myCamStruct.bCameraMoving = TRUE;
			MyWin32::myCamStruct.PressedKey = wParam;
			break;

		case 0x53: //S
			MyWin32::myCamStruct.bCameraMoving = TRUE;
			MyWin32::myCamStruct.PressedKey = wParam;
			break;

		case 0x44: //D
			MyWin32::myCamStruct.bCameraMoving = TRUE;
			MyWin32::myCamStruct.PressedKey = wParam;
			break;

		case 0x54: //T	
			MyWin32::myCamStruct.CameraTurboSpeed = 2;
			break;

		case 0x47: //G
			if (FALSE == MyWin32::isGUI)
			{
				MyWin32::isGUI = TRUE;
				MyWin32::bFirstFrame = TRUE;
				ShowCursor(TRUE);
			}
			else
			{
				MyWin32::isGUI = FALSE;
				ShowCursor(FALSE);
			}
			break;

		case VK_NUMPAD5:
			if (FALSE == MyWin32::addObject)
			{
				MyWin32::addObject = TRUE;
			}
			break;

		case 0x20:  //VK_SPACE
			MyWin32::StartDemo = TRUE;
			MyWin32::isGUI = FALSE;
			break;

		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}

		break;

	case WM_KEYUP:

		switch (wParam)
		{

		case 0x57: //W
			MyWin32::myCamStruct.bCameraMoving = FALSE;
			break;

		case 0x41: //A
			MyWin32::myCamStruct.bCameraMoving = FALSE;
			break;

		case 0x53: //S
			MyWin32::myCamStruct.bCameraMoving = FALSE;
			break;

		case 0x44: //D
			MyWin32::myCamStruct.bCameraMoving = FALSE;
			break;
		}

		break;

	}

	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}
void MyWin32::InitMyWin32(TCHAR* className, HINSTANCE hInstance,int iCmdShow)
{
	WNDCLASSEX wndClass = {};

	ZeroMemory(&wndClass, sizeof(wndClass));

	wndClass.cbSize = sizeof(wndClass);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = MyWin32::WndProc;
	wndClass.lpszClassName = className;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW |CS_OWNDC;

	if (0 == RegisterClassEx(&wndClass))
	{
		MessageBox(NULL, TEXT("RegisterClassEX Failed"), TEXT("Error"), MB_OK);
		MyWin32Log.EndLog(this->logFileName);
		exit(0);
	}
	else
	{
		this->hwnd = CreateWindowEx(
			WS_EX_APPWINDOW,
			className,
			TEXT("NiNaD_Engine"),
			WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
			440,
			40,
			1024,
			900,
			NULL,
			NULL,
			hInstance,
			NULL);
	}

	ShowWindow(this->hwnd, iCmdShow);
	SetForegroundWindow(this->hwnd);
	SetFocus(this->hwnd);

}
void MyWin32::ToggleFullScreen(HWND hwnd)
{
	// variable declarations
	MONITORINFO mi = {};
	DWORD dwStyle=0;

	static bool bIsFullScreen=false;
	static WINDOWPLACEMENT wpPrev = {  };
	wpPrev.length = sizeof(WINDOWPLACEMENT);

	if (bIsFullScreen == false) {
		dwStyle = GetWindowLong(hwnd, GWL_STYLE);

		if (dwStyle & WS_OVERLAPPEDWINDOW) {
			mi = { sizeof(MONITORINFO) };

			if (
				GetWindowPlacement(hwnd, &wpPrev)
				&&
				GetMonitorInfo(MonitorFromWindow(hwnd, MONITORINFOF_PRIMARY), &mi)
				) {
				SetWindowLong(hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(
					hwnd,
					HWND_TOP,
					mi.rcMonitor.left,
					mi.rcMonitor.top,
					(mi.rcMonitor.right - mi.rcMonitor.left),
					(mi.rcMonitor.bottom - mi.rcMonitor.top),
					SWP_NOZORDER | SWP_FRAMECHANGED
				);
			}
		}
		ShowCursor(FALSE);
		bIsFullScreen = true;
	}
	else {
		SetWindowLong(hwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(hwnd, &wpPrev);
		SetWindowPos(
			hwnd,
			HWND_TOP,
			0,
			0,
			0,
			0,
			SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER
		);

		ShowCursor(TRUE);
		bIsFullScreen = false;
	}
}
MSG MyWin32::StartGameLoop(void)
{
	MSG msg = {0};
	gameloopStarted = TRUE;

	__int64 prevTime=0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTime);

	__int64 countsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);

	GLfloat secsPerCount = 1.0f / countsPerSec;
	
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
		}
		else
		{
			__int64 curTime = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
			GLfloat deltaTime = (curTime - prevTime)*secsPerCount;

			//game
			MyOpenGL::fDeltaTime = deltaTime;

			//OpenGL is aleady initialised before gameloop starts
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			if (MyWin32::StartDemo == TRUE)
			{
				myOpenGL.RenderScene();
				MyWin32::iFrameID += 1;
			}
			// MyWin32::iFrameID += 1;

			SwapBuffers(myOpenGL.getHDC());
			prevTime = curTime;
		}


	}

	this->Uninitialize();

	return msg;
}
 void MyWin32::Resize(int width, int height)
{
	if (height == 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	MyWin32::gProjectionMatrix = vmath::Perspective(50.0f, (GLfloat)width / (GLfloat)height,0.1f, 1500000000.0f);//
	MyWin32::gInvProjectionMatrix = vmath::inverse(MyWin32::gProjectionMatrix);
	//MyWin32::gProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height,1.7f, 100000.0f);//best for terrain
	//MyWin32::gInfiniteProjectionMatrix = vmath::myInfiniteProj(45.0f, (GLfloat)width / (GLfloat)height, 0.01f, 1000.0f);
	
	MyWin32::bRecentResize = TRUE;
}
 
 void MyWin32::Uninitialize(void)
 {
	 myOpenGL.CleanUp();
	 MyWin32Log.EndLog(this->logFileName);

	 ImGui_ImplOpenGL3_Shutdown();
	 ImGui_ImplWin32_Shutdown();
	 ImGui::DestroyContext();

	 if ( FALSE==gameloopStarted )
	 {
		 exit(EXIT_SUCCESS);
	 }
	 else if(hwnd)
	 {
		 DestroyWindow(hwnd);
	 }

}
