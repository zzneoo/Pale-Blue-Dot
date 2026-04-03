#include "UnderWaterQuadShaderLoader.h"
#include"MyOpenGL.h"

UnderWaterQuadShaderLoader::UnderWaterQuadShaderLoader()
{
	ModelViewProjXUniform = 0;
	v3ColorUniform = 0;
	v3SunPosUniform = 0;
	v3EyePosUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}

};
UnderWaterQuadShaderLoader::~UnderWaterQuadShaderLoader()
{
};

bool UnderWaterQuadShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();

	return true;
}

void UnderWaterQuadShaderLoader::LoadModelViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(ModelViewProjXUniform, matrix);
}

void UnderWaterQuadShaderLoader::LoadModelXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(ModelXUniform, matrix);
}
void UnderWaterQuadShaderLoader::Loadv3ColorUniform(vmath::vec3 color)
{
	UniformLoadVec3(v3ColorUniform, color);
}
void UnderWaterQuadShaderLoader::Loadv3SunPosUniform(vmath::vec3 sunPos)
{
	UniformLoadVec3(v3SunPosUniform, sunPos);
}
void UnderWaterQuadShaderLoader::Loadv3EyePosUniform(vmath::vec3 eyePos)
{
	UniformLoadVec3(v3EyePosUniform, eyePos);
}


void UnderWaterQuadShaderLoader::GetAllUniformLocations(void)
{
	ModelViewProjXUniform = GetUniformLocation("m4ModelViewProjX");
	ModelXUniform = GetUniformLocation("m4ModelX");
	v3ColorUniform = GetUniformLocation("v3Color");
	v3SunPosUniform = GetUniformLocation("v3SunPos");
	v3EyePosUniform = GetUniformLocation("v3EyePos");
}

