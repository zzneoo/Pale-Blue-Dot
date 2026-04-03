#include "CubeMapShaderLoader.h"
#include"MyOpenGL.h"

CubeMapShaderLoader::CubeMapShaderLoader()
{
	ModelViewProjXUniform = 0;
	SamplerCube_AlbedoTextureUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}

};
CubeMapShaderLoader::~CubeMapShaderLoader()
{
};

bool CubeMapShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();

	return true;
}

void CubeMapShaderLoader::LoadModelViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(ModelViewProjXUniform, matrix);
}
void CubeMapShaderLoader::LoadAlbedoCubeMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTextureCubeMap(SamplerCube_AlbedoTextureUniform, TEXTURE, tNo, textureID);
}

void CubeMapShaderLoader::GetAllUniformLocations(void)
{
	ModelViewProjXUniform = GetUniformLocation("m4ModelViewProjX");
	SamplerCube_AlbedoTextureUniform = GetUniformLocation("tSamplerCube");
}

