#include "LightAreaMaskShaderLoader.h"
#include"MyOpenGL.h"

LightAreaMaskShaderLoader::LightAreaMaskShaderLoader()
{
	Sampler2DSceneMask_Uniform = 0;
	Sampler2DCloudMask_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}

LightAreaMaskShaderLoader::~LightAreaMaskShaderLoader()
{

}

//samplers
void LightAreaMaskShaderLoader::LoadSampler2DSceneMask_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DSceneMask_Uniform, TEXTURE, tNo, textureID);
}
void LightAreaMaskShaderLoader::LoadSampler2DCloudMask_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DCloudMask_Uniform, TEXTURE, tNo, textureID);
}


bool LightAreaMaskShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void LightAreaMaskShaderLoader::GetAllUniformLocations(void)
{
	Sampler2DSceneMask_Uniform = GetUniformLocation("tSamplerSceneMask");
	Sampler2DCloudMask_Uniform = GetUniformLocation("tSamplerCloudMask");
}

