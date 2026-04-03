#include "Axis3D_ShaderLoader.h"
#include"MyOpenGL.h"

Axis3D_ShaderLoader::Axis3D_ShaderLoader()
{

	m4ModelViewXUniform = 0;
	m4ProjXUniform = 0;
	v3ScaleUniform = 0;
	v3ColorUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}

};
Axis3D_ShaderLoader::~Axis3D_ShaderLoader()
{
};

bool Axis3D_ShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();

	return true;
}


void Axis3D_ShaderLoader::LoadModelViewXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelViewXUniform, matrix);
}
void Axis3D_ShaderLoader::LoadProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ProjXUniform, matrix);
}

void Axis3D_ShaderLoader::Loadv3ScaleUniform(vmath::vec3 scale)
{
	UniformLoadVec3(v3ScaleUniform, scale);
}

void Axis3D_ShaderLoader::Loadv3ColorUniform(vmath::vec3 color)
{
	UniformLoadVec3(v3ColorUniform, color);
}

void Axis3D_ShaderLoader::LoadfMaxEyeDistanceUniform(GLfloat FarMinusNear)
{
	UniformLoad1f(fMaxEyeDistanceUniform, FarMinusNear);
}


void Axis3D_ShaderLoader::GetAllUniformLocations(void)
{

	m4ModelViewXUniform = GetUniformLocation("m4ModelViewX");
	m4ProjXUniform = GetUniformLocation("m4ProjX");
	v3ScaleUniform = GetUniformLocation("v3Scale");
	v3ColorUniform = GetUniformLocation("v3Color");
	fMaxEyeDistanceUniform = GetUniformLocation("fMaxEyeDistance");
}

