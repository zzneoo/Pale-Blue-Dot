#include "CullingIndirectShaderLoader.h"
#include"MyOpenGL.h"

CullingIndirectShaderLoader::CullingIndirectShaderLoader()
{
	ProjXUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}

};
CullingIndirectShaderLoader::~CullingIndirectShaderLoader()
{
};

bool CullingIndirectShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_cs))
	{
		return false;
	}
	GetAllUniformLocations();

	return true;
}

void CullingIndirectShaderLoader::LoadProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(ProjXUniform, matrix);
}


void CullingIndirectShaderLoader::GetAllUniformLocations(void)
{
	ProjXUniform = GetUniformLocation("m4ProjX");

}


