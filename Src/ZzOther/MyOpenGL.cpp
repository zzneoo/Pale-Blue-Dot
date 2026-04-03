#include"stdafx.h"
#include"MyWin32.h"


GLfloat MyOpenGL::fDeltaTime = 0.0f;
GLfloat MyOpenGL::fTime = 0.0f;
GLfloat MyOpenGL::fExposure = 10.0f;
bool MyOpenGL::bShaderInitStatus = true;
bool MyOpenGL::bObject3DInitStatus = true;

MyOpenGL::MyOpenGL()
{
	Renderer = nullptr;

	this->hdc = NULL;
	this->hrc = NULL;
	this->hwnd = NULL;// copy of win32 class's hwnd
}

MyOpenGL::~MyOpenGL()
{
	CleanUp();
}

bool MyOpenGL::InitializeOpenGL(HWND hwndFromWin32)
{
	this->hwnd = hwndFromWin32;

	PIXELFORMATDESCRIPTOR pfd = {};
	int iPixelFormatIndex;

	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));


	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	hdc = GetDC(this->hwnd);
	if(hdc==NULL)
	{
		MessageBox(this->hwnd, TEXT("GetDC() Failed"), TEXT("Error"), MB_OK | MB_TOPMOST);
		return false;
	}

	iPixelFormatIndex = ChoosePixelFormat(hdc, &pfd);
	if (iPixelFormatIndex == 0)
	{
		MessageBox(hwnd, TEXT("ChoosePixelFormat() Failed"), TEXT("Error"), MB_OK|MB_TOPMOST);
		return false;
	}


	if (SetPixelFormat(hdc, iPixelFormatIndex, &pfd) == false)
	{
		MessageBox(this->hwnd, TEXT("SetPixelFormat() Failed"), TEXT("Error"), MB_OK | MB_TOPMOST);
		return false;
	}


	hrc = wglCreateContext(hdc);
	if (hrc == NULL)
	{
		MessageBox(this->hwnd, TEXT("wglCreateContext() Failed"), TEXT("Error"), MB_OK | MB_TOPMOST);
		return false;
	}


	if (wglMakeCurrent(hdc, hrc) == false)
	{
		MessageBox(this->hwnd, TEXT("wglMakeCurrent() Failed"), TEXT("Error"), MB_OK | MB_TOPMOST);
		return false;
	}


	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK)
	{
		MessageBox(this->hwnd, TEXT("glewInit() Failed"), TEXT("Error"), MB_OK | MB_TOPMOST);
		return false;
	}

	//imgui
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplWin32_Init(this->hwnd);
		const char* glsl_version = "#version 460";
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	//DepthRelatedStuff
	glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);

//	glClearDepth(0.0);
//	glDepthFunc(GL_GREATER);
//	glDepthRangedNV(1.0, 0.0);
	//glEnable(GL_FRAMEBUFFER_SRGB);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_PROGRAM_POINT_SIZE);

	//vmath::vec4 waterGradient = vmath::vec4(0.1411764f, 0.1725490f, 0.16862745f,1.0f);
	//vmath::vec4 waterBase = vmath::vec4(0.20392156f, 0.41960784f, 0.41568627f,1.0f);

	//vmath::vec4 finalWaterColor = (waterBase*(1.0- 0.57813f)+waterGradient* 0.56813f)* (0.79687f);

	//glClearColor(finalWaterColor[0], finalWaterColor[1], finalWaterColor[2], 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return InitializeAfterGLEW();//Very Important Function Call--------------------------------------
}
bool MyOpenGL::InitializeAfterGLEW(void)
{
	//renderer
	MyOpenGL::bShaderInitStatus = true;
	MyOpenGL::bObject3DInitStatus = true;
	Renderer = new ZzRenderer;

	return (MyOpenGL::bShaderInitStatus && MyOpenGL::bObject3DInitStatus);
}

void MyOpenGL::CleanUp(void)
{
	//heap classes
	if (Renderer)
	{
		delete Renderer;
		Renderer = nullptr;
	}

	//WGL stuff
	if (wglGetCurrentContext() == hrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if (hrc)
	{
		wglDeleteContext(hrc);
		hrc = NULL;
	}
	if (hdc)
	{
		ReleaseDC(this->hwnd, hdc);
		hdc = NULL;
	}
}


void MyOpenGL::RenderScene(void)
{
	Renderer->ResizeFramebuffers(MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight);
	Renderer->Update(this->hwnd);

	Renderer->Render_All(this->hwnd);
//	Renderer->Render_TestScene(this->hwnd);

	//GUI
	Renderer->Render_GUI();

	Renderer->GetLastFrameData(); //Reprojection
}
