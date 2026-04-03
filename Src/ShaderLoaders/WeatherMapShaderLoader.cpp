#include "WeatherMapShaderLoader.h"
#include"MyOpenGL.h"

WeatherMapShaderLoader::WeatherMapShaderLoader()
{
	iTextureSize_Uniform = 0;
	Sampler1D_permutationTextureUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}
WeatherMapShaderLoader::~WeatherMapShaderLoader()
{

}

void WeatherMapShaderLoader::LoadSampler1D_permutationTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture1D(Sampler1D_permutationTextureUniform, TEXTURE, tNo, textureID);
}

void WeatherMapShaderLoader::LoadiTextureSize_Uniform(GLint iSize)
{
	UniformLoad1i(iTextureSize_Uniform, iSize);

}

bool WeatherMapShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_cs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void WeatherMapShaderLoader::GetAllUniformLocations(void)
{
	Sampler1D_permutationTextureUniform = GetUniformLocation("Sampler1D_permutation");
	iTextureSize_Uniform = GetUniformLocation("iTextureSize");
}