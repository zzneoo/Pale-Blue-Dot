#pragma once
#include"stdafx.h"
#include "common.h"
#include <exception>

class Util
{
public:
	static BITMAP_INFO loadTexture(GLuint* texture, TCHAR imageResourceId[], bool initializeVertices, bool applyAnIsoTropicFilter);
	static GLuint generateShader(GLenum shaderName, const GLchar* shaderSourceCode);
	static vmath::mat4 inverse(vmath::mat4 mat);
	
    static BITMAP LoadTextureFromFile(std::string path);
};

