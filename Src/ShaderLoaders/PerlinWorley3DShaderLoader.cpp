#include "PerlinWorley3DShaderLoader.h"
#include"MyOpenGL.h"

PerlinWorley3DShaderLoader::PerlinWorley3DShaderLoader()
{
	iTextureSize_Uniform = 0;
	Sampler1D_permutationTextureUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}
PerlinWorley3DShaderLoader::~PerlinWorley3DShaderLoader()
{

}

void PerlinWorley3DShaderLoader::LoadSampler1D_permutationTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture1D(Sampler1D_permutationTextureUniform, TEXTURE, tNo, textureID);
}

void PerlinWorley3DShaderLoader::LoadiTextureSize_Uniform(GLint iSize)
{
	UniformLoad1i(iTextureSize_Uniform, iSize);

}

bool PerlinWorley3DShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_cs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void PerlinWorley3DShaderLoader::GetAllUniformLocations(void)
{
	Sampler1D_permutationTextureUniform = GetUniformLocation("Sampler1D_permutation");
	iTextureSize_Uniform = GetUniformLocation("iTextureSize");
}