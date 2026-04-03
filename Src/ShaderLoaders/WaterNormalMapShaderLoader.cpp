#include "WaterNormalMapShaderLoader.h"
#include"MyOpenGL.h"

WaterNormalMapShaderLoader::WaterNormalMapShaderLoader()
{

	Sampler2D_WaterHeightMapTextureUniform = 0;
	iTextureSize_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}
WaterNormalMapShaderLoader::~WaterNormalMapShaderLoader()
{

}
//uniform sampler
void WaterNormalMapShaderLoader::LoadSampler2D_WaterHeightMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_WaterHeightMapTextureUniform, TEXTURE, tNo, textureID);
}
void WaterNormalMapShaderLoader::LoadiTextureSize_Uniform(GLint iSize)
{
	UniformLoad1i(iTextureSize_Uniform, iSize);
}

bool WaterNormalMapShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_cs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void WaterNormalMapShaderLoader::GetAllUniformLocations(void)
{
	Sampler2D_WaterHeightMapTextureUniform = GetUniformLocation("Sampler2D_WaterHeightMap");
	iTextureSize_Uniform = GetUniformLocation("iTextureSize");

}