#include "stdafx.h"
#include"MyWin32.h"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"opencv_world430d.lib")
#pragma comment(lib,"OpenAL32.lib")
#pragma comment(lib,"assimp-vc142-mtd.lib")

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdLine, _In_ int iCmdShow)//used _In_ to Remove VS19 warnings
{
	TCHAR className[] = { TEXT("myOpenGLWindow") };

	MyWin32 MyWin32(hInstance, className, iCmdShow);
	MSG msg = MyWin32.StartGameLoop();

	return (static_cast<int>(msg.wParam));
}
