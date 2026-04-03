#include "FinalQuadShaderLoader.h"
#include"MyOpenGL.h"

FinalQuadShaderLoader::FinalQuadShaderLoader()
{
	Sampler2DTexture0_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}

FinalQuadShaderLoader::~FinalQuadShaderLoader()
{

}

void FinalQuadShaderLoader::LoadSampler2DTexture0_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DTexture0_Uniform, TEXTURE, tNo, textureID);
}


bool FinalQuadShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void FinalQuadShaderLoader::GetAllUniformLocations(void)
{
	Sampler2DTexture0_Uniform = GetUniformLocation("tSampler0");
}
