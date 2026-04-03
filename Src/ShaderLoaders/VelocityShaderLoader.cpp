#include "VelocityShaderLoader.h"
#include"MyOpenGL.h"

VelocityShaderLoader::VelocityShaderLoader()
{

	Sampler2DCurrDepth_Uniform = 0;

	v2Halton23_Uniform = 0;
	v2NearFar_Uniform = 0;

	v3EyePos_Uniform = 0;

	m4InvViewX_Uniform = 0;
	m4ProjX_Uniform = 0;
	m4InvProjX_Uniform = 0;
	m4PrevViewX_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}

VelocityShaderLoader::~VelocityShaderLoader()
{

}

//depth
void VelocityShaderLoader::LoadSampler2DCurrDepth_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DCurrDepth_Uniform, TEXTURE, tNo, textureID);
}

//uniform matrices
void VelocityShaderLoader::Loadm4ProjX_Uniform(vmath::mat4 ProjX)
{
	UniformLoadMatrix(m4ProjX_Uniform, ProjX);
}
void VelocityShaderLoader::Loadm4InvProjX_Uniform(vmath::mat4 InvProjX)
{
	UniformLoadMatrix(m4InvProjX_Uniform, InvProjX);
}

void VelocityShaderLoader::Loadm4PrevViewX_Uniform(vmath::mat4 PrevViewX)
{
	UniformLoadMatrix(m4PrevViewX_Uniform, PrevViewX);
}

void VelocityShaderLoader::Loadm4InvViewX_Uniform(vmath::mat4 InvViewX)
{
	UniformLoadMatrix(m4InvViewX_Uniform, InvViewX);
}

void VelocityShaderLoader::Loadv2Halton23_Uniform(vmath::vec2 Halton23)
{
	UniformLoadVec2(v2Halton23_Uniform, Halton23);
}

void VelocityShaderLoader::Loadv2NearFar_Uniform(vmath::vec2 NearFar)
{
	UniformLoadVec2(v2NearFar_Uniform, NearFar);
}
void VelocityShaderLoader::Loadv3EyePos_Uniform(vmath::vec3 EyePos)
{
	UniformLoadVec3(v3EyePos_Uniform, EyePos);
}

bool VelocityShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void VelocityShaderLoader::GetAllUniformLocations(void)
{

	Sampler2DCurrDepth_Uniform = GetUniformLocation("tSamplerCurrDepth");

	v2Halton23_Uniform = GetUniformLocation("v2Halton23");
	v2NearFar_Uniform = GetUniformLocation("v2NearFar");

	v3EyePos_Uniform = GetUniformLocation("v3EyePos");

	m4ProjX_Uniform = GetUniformLocation("m4ProjX");
	m4InvProjX_Uniform = GetUniformLocation("m4InvProjX");
	m4PrevViewX_Uniform = GetUniformLocation("m4PrevViewX");
	m4InvViewX_Uniform = GetUniformLocation("m4InvViewX");

}
