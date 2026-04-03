#include "DefferedPBR_SpeedTreeInstancedShaderLoader.h"
#include"MyOpenGL.h"

DefferedPBR_SpeedTreeInstancedShaderLoader::DefferedPBR_SpeedTreeInstancedShaderLoader()
{
	//uniform locations
	m4ModelViewProjX_Uniform = 0;
	m4PrevViewProjX_Uniform = 0;
	m4ViewProjX_Uniform = 0;

	//Samplers
	Sampler2D_PBR_AlbedoUniform = 0;
	Sampler2D_PBR_NormalUniform = 0;
	Sampler2D_PBR_SubsurfaceUniform = 0;
	Sampler2D_PBR_BlueNoise = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}
DefferedPBR_SpeedTreeInstancedShaderLoader::~DefferedPBR_SpeedTreeInstancedShaderLoader()
{
}

//matrices-------------------------------------------------------------------------------------------
void DefferedPBR_SpeedTreeInstancedShaderLoader::LoadModelViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelViewProjX_Uniform, matrix);
}

void DefferedPBR_SpeedTreeInstancedShaderLoader::LoadPrevViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4PrevViewProjX_Uniform, matrix);
}

void DefferedPBR_SpeedTreeInstancedShaderLoader::LoadViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ViewProjX_Uniform, matrix);
}


//samplers------------------------------------------------------------------------------------------------
void DefferedPBR_SpeedTreeInstancedShaderLoader::LoadAlbedoTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_AlbedoUniform, TEXTURE, tNo, textureID);
}
void DefferedPBR_SpeedTreeInstancedShaderLoader::LoadNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_NormalUniform, TEXTURE, tNo, textureID);
}
void DefferedPBR_SpeedTreeInstancedShaderLoader::LoadSubsurfaceTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_SubsurfaceUniform, TEXTURE, tNo, textureID);
}
void DefferedPBR_SpeedTreeInstancedShaderLoader::LoadBlueNoiseTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_BlueNoise, TEXTURE, tNo, textureID);
}

//----------------------------------------------------------------------------------------------------------

bool DefferedPBR_SpeedTreeInstancedShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void DefferedPBR_SpeedTreeInstancedShaderLoader::GetAllUniformLocations(void)
{
	//matrices
//	m4ModelX_Uniform = GetUniformLocation("m4ModelX");
//	m4ViewX_Uniform = GetUniformLocation("m4ViewX");
//	m4ProjX_Uniform = GetUniformLocation("m4ProjX");

	//matrices
	m4ModelViewProjX_Uniform = GetUniformLocation("m4ModelViewProjX");
	m4PrevViewProjX_Uniform = GetUniformLocation("m4PrevViewProjX");
	m4ViewProjX_Uniform = GetUniformLocation("m4ViewProjX");

	//samplers
	Sampler2D_PBR_AlbedoUniform = GetUniformLocation("Sampler2D_Albedo");
	Sampler2D_PBR_NormalUniform = GetUniformLocation("Sampler2D_Normal");
	Sampler2D_PBR_SubsurfaceUniform = GetUniformLocation("Sampler2D_Subsurface");
	Sampler2D_PBR_BlueNoise = GetUniformLocation("Sampler2D_BlueNoise");
}
