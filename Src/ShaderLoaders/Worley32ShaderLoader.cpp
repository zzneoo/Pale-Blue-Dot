#include "Worley32ShaderLoader.h"
#include"MyOpenGL.h"

Worley32ShaderLoader::Worley32ShaderLoader()
{
	iTextureSize_Uniform = 0;
	Sampler1D_permutationTextureUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}
Worley32ShaderLoader::~Worley32ShaderLoader()
{

}

void Worley32ShaderLoader::LoadSampler1D_permutationTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture1D(Sampler1D_permutationTextureUniform, TEXTURE, tNo, textureID);
}

void Worley32ShaderLoader::LoadiTextureSize_Uniform(GLint iSize)
{
	UniformLoad1i(iTextureSize_Uniform, iSize);

}

bool Worley32ShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_cs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void Worley32ShaderLoader::GetAllUniformLocations(void)
{
	Sampler1D_permutationTextureUniform = GetUniformLocation("Sampler1D_permutation");
	iTextureSize_Uniform = GetUniformLocation("iTextureSize");
}