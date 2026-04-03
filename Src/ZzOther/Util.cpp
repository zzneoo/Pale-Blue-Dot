#include "Util.h"
#include <vector>
//#include "OpenGLState.h"
//#include "GL/wglext.h"
#include "GL/wglext.h"
#include <filesystem>
#include "ZzRenderer.h"



//BITMAP_INFO Util::loadTexture(GLuint* texture, TCHAR imageResourceId[], bool initializeVertices, bool applyAnIsoTropicFilter) {
//	void initVertices();
//
//	// local variables
//	BITMAP_INFO bInfo;
//	HBITMAP hBitmap = NULL;
//    BITMAP bmp = {};
//	BOOL bStatus = FALSE;
//
//	// code
//	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceId, IMAGE_BITMAP, NULL, NULL, LR_CREATEDIBSECTION);
//
//	if (!hBitmap) {
//		return bInfo;
//	}
//
//	bStatus = TRUE;
//	GetObject(hBitmap, sizeof(BITMAP), &bmp);
//
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
//	glGenTextures(1, texture);
//	glBindTexture(GL_TEXTURE_2D, *texture);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//	if (initializeVertices) {
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		initVertices();
//	}
//
//	if (applyAnIsoTropicFilter) {
//		float largest;
//		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &largest);
//
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, largest);
//	}
//
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.bmBits);
//	glGenerateMipmap(GL_TEXTURE_2D);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	bInfo = BITMAP_INFO(bmp, 3);//bmp.bmBits wont be accessible after DeleteObject();
//	DeleteObject(hBitmap);
//
//	return bInfo;
//}

GLuint Util::generateShader(GLenum shaderName, const GLchar* shaderSourceCode) {
	//void uninitialize(void);

	GLuint shader;

	shader = glCreateShader(shaderName);

	// specify above source code to the vertex shader object
	glShaderSource(
		shader,	// whom to provide / attach the above source code
		1,						// length of the string in source code, if we are providing multiple strings then this value will be different
		(const GLchar**)&shaderSourceCode,	// specify address of source code
		NULL					// if source code array has multiple strings then here provide arrays of length of strings
	);

	// compile the vertex shader
	glCompileShader(shader);

	// steps for catching shader related errors
	GLint iShaderCompileStatus = 0;
	GLint iInfoLogLength = 0;
	volatile GLchar* szInfoLog = NULL;//dont want szInfoLog to get optimized away in released builds, hence volatile

	// get compile status of shader
	glGetShaderiv(shader, GL_COMPILE_STATUS, &iShaderCompileStatus);

	// check it (value of iShaderCompileStatus) for GL_FALSE, coz if error then value will be GL_FALSE
	if (iShaderCompileStatus == GL_FALSE) {
		// now call the same function as above to get the info log length (for error log) as now we know that there is an error
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &iInfoLogLength);

		// if info log length is greater than 0, get error log
		if (iInfoLogLength > 0) {

			szInfoLog = (GLchar*)malloc(iInfoLogLength);

			if (szInfoLog != NULL) {
				GLsizei written;

				glGetShaderInfoLog(shader, iInfoLogLength, &written, (GLchar*)szInfoLog);

				//fprintf_s(gpFile, "%d Shader Error Log: \n%s", shaderName, szInfoLog);
				//free(szInfoLog);
				//uninitialize();
				MessageBox(NULL, TEXT("Shader Compilation Failed.\n For More Info Check Log File: Blue Window With Shader Log.txt"), TEXT("Error"), MB_OK | MB_ICONERROR);
                UTIL_SET_BREAKPOINT;
                free((GLchar*)szInfoLog);
				exit(0);
			}
		}
	}

	return shader;
}

vmath::mat4 Util::inverse(vmath::mat4 mat)
{
	//float a0 = a.data[0] * a.data[5] - a.data[4] * a.data[1];
	float a0 = mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];

	//float a1 = a.data[0] * a.data[9] - a.data[8] * a.data[1];
	float a1 = mat[0][0] * mat[2][1] - mat[2][0] * mat[0][1];

	//float a2 = a.data[0] * a.data[13] - a.data[12] * a.data[1];
	float a2 = mat[0][0] * mat[3][1] - mat[3][0] * mat[0][1];

	//float a3 = a.data[4] * a.data[9] - a.data[8] * a.data[5];
	float a3 = mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1];

	//float a4 = a.data[4] * a.data[13] - a.data[12] * a.data[5];
	float a4 = mat[1][0] * mat[3][1] - mat[3][0] * mat[1][1];

	//float a5 = a.data[8] * a.data[13] - a.data[12] * a.data[9];
	float a5 = mat[2][0] * mat[3][1] - mat[3][0] * mat[2][1];

	//float b0 = a.data[2] * a.data[7] - a.data[6] * a.data[3];
	float b0 = mat[0][2] * mat[1][3] - mat[1][2] * mat[0][3];

	//float b1 = a.data[2] * a.data[11] - a.data[10] * a.data[3];
	float b1 = mat[0][2] * mat[2][3] - mat[2][2] * mat[0][3];

	//float b2 = a.data[2] * a.data[15] - a.data[14] * a.data[3];
	float b2 = mat[0][2] * mat[3][3] - mat[3][2] * mat[0][3];

	//float b3 = a.data[6] * a.data[11] - a.data[10] * a.data[7];
	float b3 = mat[1][2] * mat[2][3] - mat[2][2] * mat[1][3];

	//float b4 = a.data[6] * a.data[15] - a.data[14] * a.data[7];
	float b4 = mat[1][2] * mat[3][3] - mat[3][2] * mat[1][3];

	//float b5 = a.data[10] * a.data[15] - a.data[14] * a.data[11];
	float b5 = mat[2][2] * mat[3][3] - mat[3][2] * mat[2][3];

	float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

	if (0 == det)
	{
		return vmath::mat4(INFINITY);
	}

	float inv_det = 1.0f / det;

	vmath::mat4 inv;
	inv[0][0] = +mat[1][1] * b5 - mat[2][1] * b4 + mat[3][1] * b3;
	inv[0][1] = -mat[0][1] * b5 + mat[2][1] * b2 - mat[3][1] * b1;
	inv[0][2] = +mat[0][1] * b4 - mat[1][1] * b2 + mat[3][1] * b0;
	inv[0][3] = -mat[0][1] * b3 + mat[1][1] * b1 - mat[2][1] * b0;
	inv[1][0] = -mat[1][0] * b5 + mat[2][0] * b4 - mat[3][0] * b3;
	inv[1][1] = +mat[0][0] * b5 - mat[2][0] * b2 + mat[3][0] * b1;
	inv[1][2] = -mat[0][0] * b4 + mat[1][0] * b2 - mat[3][0] * b0;
	inv[1][3] = +mat[0][0] * b3 - mat[1][0] * b1 + mat[2][0] * b0;
	inv[2][0] = +mat[1][3] * a5 - mat[2][3] * a4 + mat[3][3] * a3;
	inv[2][1] = -mat[0][3] * a5 + mat[2][3] * a2 - mat[3][3] * a1;
	inv[2][2] = +mat[0][3] * a4 - mat[1][3] * a2 + mat[3][3] * a0;
	inv[2][3] = -mat[0][3] * a3 + mat[1][3] * a1 - mat[2][3] * a0;
	inv[3][0] = -mat[1][2] * a5 + mat[2][2] * a4 - mat[3][2] * a3;
	inv[3][1] = +mat[0][2] * a5 - mat[2][2] * a2 + mat[3][2] * a1;
	inv[3][2] = -mat[0][2] * a4 + mat[1][2] * a2 - mat[3][2] * a0;
	inv[3][3] = +mat[0][2] * a3 - mat[1][2] * a1 + mat[2][2] * a0;
	inv *= inv_det;

	return inv;
}





BITMAP Util::LoadTextureFromFile(std::string path)
{//TODO: possible memory leak : delete object on hbitmap - LR_SHARED may still limit amount of leak
    DEBUG_ASSERT(std::filesystem::exists(path), "texture doesn't exist at path");
    auto hBitmap = (HBITMAP)LoadImageW(NULL, S2WTR(path).c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_SHARED);
    BITMAP bmp = {};
    GetObject(hBitmap, sizeof(bmp), &bmp);
    return bmp;
}



void OpenGlDebugCallBack(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message,void* userParam)
{
#ifdef _WIN32
    char* str = new char[length + 1];
    strcpy_s(str, length + 1, message);

    OutputDebugStringA("\n##ARB_DEBUG_START##\n");
    OutputDebugStringA(str);
    OutputDebugStringA("\n##ARB_DEBUG_END##\n");
#endif
}

void ArbDebugEnable(HDC& hdc, HGLRC& hrc)
{
    int attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,//set below
        WGL_CONTEXT_MINOR_VERSION_ARB, 1,//set belowglXCreateContextAttribsARB
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
        0
    };

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if (nullptr != wglCreateContextAttribsARB)
    {
        HGLRC newHrc = wglCreateContextAttribsARB(hdc, 0, attribs);
        if (NULL != newHrc)
        {
            wglMakeCurrent(hdc, newHrc);
            wglDeleteContext(hrc);
            hrc = newHrc;

            //register callback
            //code from https://sites.google.com/site/opengltutorialsbyaks/introduction-to-opengl-4-1---tutorial-05
            typedef void (*PFNGLDEBUGMESSAGECALLBACKARBPROC) (GLDEBUGPROCARB callback, void* userParam);
            PFNGLDEBUGMESSAGECALLBACKARBPROC  glDebugMessageCallbackARB = nullptr;
            glDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKARBPROC)wglGetProcAddress("glDebugMessageCallbackARB");
            if (nullptr != glDebugMessageCallbackARB)
            {
                glDebugMessageCallbackARB((GLDEBUGPROCARB)&OpenGlDebugCallBack, nullptr);
            }

        }
        //else
        //{
        //    DEBUG_ASSERT(false, "wglCreateContextAttribsARB() failed");
        //}

    }
}