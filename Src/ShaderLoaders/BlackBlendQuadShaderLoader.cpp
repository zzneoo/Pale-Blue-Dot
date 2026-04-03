#include "BlackBlendQuadShaderLoader.h"
#include"MyOpenGL.h"

BlackBlendQuadShaderLoader::BlackBlendQuadShaderLoader()
{
	Sampler2DTexture_Uniform = 0;
	fBlendFactorUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}

BlackBlendQuadShaderLoader::~BlackBlendQuadShaderLoader()
{

}

void BlackBlendQuadShaderLoader::LoadSampler2DTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DTexture_Uniform, TEXTURE, tNo, textureID);
}

void BlackBlendQuadShaderLoader::LoadBlendFactorUniform(GLfloat factor)
{
	UniformLoad1f(fBlendFactorUniform, factor);
}


bool BlackBlendQuadShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void BlackBlendQuadShaderLoader::GetAllUniformLocations(void)
{
	Sampler2DTexture_Uniform = GetUniformLocation("tSampler");
	fBlendFactorUniform = GetUniformLocation("fBlendFactor");
}
