#include "PBR_AnimatedShadowShaderLoader.h"
#include"MyOpenGL.h"

PBR_AnimatedShadowShaderLoader::PBR_AnimatedShadowShaderLoader()
{
	//uniform locations
	m4JointTransformX_Uniform = 0;
	m4ModelX_Uniform = 0;
	m4ViewX_Uniform = 0;
	m4ProjX_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}
PBR_AnimatedShadowShaderLoader::~PBR_AnimatedShadowShaderLoader()
{
}

//matrices-------------------------------------------------------------------------------------------

void PBR_AnimatedShadowShaderLoader::Loadm4JointTransformXUniform(vmath::mat4* matricesPointer,GLsizei count)
{
	UniformLoadMatrixCounted(m4JointTransformX_Uniform, matricesPointer, count);
}

void PBR_AnimatedShadowShaderLoader::LoadModelXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelX_Uniform, matrix);
}
void PBR_AnimatedShadowShaderLoader::LoadViewXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ViewX_Uniform, matrix);
}
void PBR_AnimatedShadowShaderLoader::LoadProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ProjX_Uniform, matrix);
}




bool PBR_AnimatedShadowShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void PBR_AnimatedShadowShaderLoader::GetAllUniformLocations(void)
{
	//matrices
	m4ModelX_Uniform = GetUniformLocation("m4ModelX");
	m4ViewX_Uniform = GetUniformLocation("m4ViewX");
	m4ProjX_Uniform = GetUniformLocation("m4ProjX");

	m4JointTransformX_Uniform = GetUniformLocation("u_jointTransforms");
	
}
