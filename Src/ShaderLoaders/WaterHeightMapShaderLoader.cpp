#include "WaterHeightMapShaderLoader.h"
#include"MyOpenGL.h"

WaterHeightMapShaderLoader::WaterHeightMapShaderLoader()
{
	fTime_Uniform = 0;
	iTextureSize_Uniform = 0;
	Sampler1D_permutationTextureUniform = 0;
	Sampler3D_perlinWorleyTextureUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}
WaterHeightMapShaderLoader::~WaterHeightMapShaderLoader()
{

}
//sampler
void WaterHeightMapShaderLoader::LoadSampler1D_permutationTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture1D(Sampler1D_permutationTextureUniform, TEXTURE, tNo, textureID);
}

void WaterHeightMapShaderLoader::LoadSampler3D_perlinWorleyTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture3D(Sampler3D_perlinWorleyTextureUniform, TEXTURE, tNo, textureID);
}
//int
void WaterHeightMapShaderLoader::LoadiTextureSize_Uniform(GLint iSize)
{
	UniformLoad1i(iTextureSize_Uniform, iSize);
}
//float
void WaterHeightMapShaderLoader::LoadfTime_Uniform(GLfloat time)
{
	UniformLoad1f(fTime_Uniform, time);
}

bool WaterHeightMapShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_cs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void WaterHeightMapShaderLoader::GetAllUniformLocations(void)
{
	Sampler1D_permutationTextureUniform = GetUniformLocation("Sampler1D_permutation");
	Sampler3D_perlinWorleyTextureUniform = GetUniformLocation("Sampler3D_perlinWorley");
	iTextureSize_Uniform = GetUniformLocation("iTextureSize");
	fTime_Uniform = GetUniformLocation("fTime");
}