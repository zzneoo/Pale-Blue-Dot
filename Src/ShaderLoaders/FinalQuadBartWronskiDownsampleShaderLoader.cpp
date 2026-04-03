#include "FinalQuadBartWronskiDownsampleShaderLoader.h"
#include"MyOpenGL.h"

FinalQuadBartWronskiDownsampleShaderLoader::FinalQuadBartWronskiDownsampleShaderLoader()
{
	Sampler2DTexture0_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}

FinalQuadBartWronskiDownsampleShaderLoader::~FinalQuadBartWronskiDownsampleShaderLoader()
{

}

void FinalQuadBartWronskiDownsampleShaderLoader::LoadSampler2DTexture0_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DTexture0_Uniform, TEXTURE, tNo, textureID);
}


bool FinalQuadBartWronskiDownsampleShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void FinalQuadBartWronskiDownsampleShaderLoader::GetAllUniformLocations(void)
{
	Sampler2DTexture0_Uniform = GetUniformLocation("tSampler0");
}
