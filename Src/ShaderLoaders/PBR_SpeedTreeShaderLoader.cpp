#include "PBR_SpeedTreeShaderLoader.h"
#include"MyOpenGL.h"

PBR_SpeedTreeShaderLoader::PBR_SpeedTreeShaderLoader()
{
	//uniform locations
	m4ModelViewProjX_Uniform = 0;
	m4ModelX_Uniform = 0;
	//vectors
	v3SunPosUniform = 0;
	v3EyePosUniform = 0;
	v3LightColorUniform = 0;
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
PBR_SpeedTreeShaderLoader::~PBR_SpeedTreeShaderLoader()
{
}

//matrices-------------------------------------------------------------------------------------------
void PBR_SpeedTreeShaderLoader::LoadModelViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelViewProjX_Uniform, matrix);
}

void PBR_SpeedTreeShaderLoader::LoadModelXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelX_Uniform, matrix);
}

//vectors-------------------------------------------------------------------------------------------------

void PBR_SpeedTreeShaderLoader::LoadSunPositionUniform(vmath::vec3 sunPos)
{
	UniformLoadVec3(v3SunPosUniform, sunPos);
}
void PBR_SpeedTreeShaderLoader::LoadEyePositionUniform(vmath::vec3 eyePos)
{
	UniformLoadVec3(v3EyePosUniform, eyePos);
}
void PBR_SpeedTreeShaderLoader::Loadv3LightColorUniform(vmath::vec3 lightColor)
{
	UniformLoadVec3(v3LightColorUniform, lightColor);
}

//samplers------------------------------------------------------------------------------------------------
void PBR_SpeedTreeShaderLoader::LoadAlbedoTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_AlbedoUniform, TEXTURE, tNo, textureID);
}
void PBR_SpeedTreeShaderLoader::LoadNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_NormalUniform, TEXTURE, tNo, textureID);
}
void PBR_SpeedTreeShaderLoader::LoadSubsurfaceTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_SubsurfaceUniform, TEXTURE, tNo, textureID);
}
void PBR_SpeedTreeShaderLoader::LoadBlueNoiseTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_BlueNoise, TEXTURE, tNo, textureID);
}

//----------------------------------------------------------------------------------------------------------

bool PBR_SpeedTreeShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void PBR_SpeedTreeShaderLoader::GetAllUniformLocations(void)
{
	//matrices
//	m4ModelX_Uniform = GetUniformLocation("m4ModelX");
//	m4ViewX_Uniform = GetUniformLocation("m4ViewX");
//	m4ProjX_Uniform = GetUniformLocation("m4ProjX");

	//matrices
	m4ModelViewProjX_Uniform = GetUniformLocation("m4ModelViewProjX");
	m4ModelX_Uniform = GetUniformLocation("m4ModelX");
	//vectors
	v3SunPosUniform = GetUniformLocation("v3SunPos");
	v3EyePosUniform = GetUniformLocation("v3EyePos");

	v3LightColorUniform = GetUniformLocation("v3LightColor");
	//samplers
	Sampler2D_PBR_AlbedoUniform = GetUniformLocation("Sampler2D_Albedo");
	Sampler2D_PBR_NormalUniform = GetUniformLocation("Sampler2D_Normal");
	Sampler2D_PBR_SubsurfaceUniform = GetUniformLocation("Sampler2D_Subsurface");
	Sampler2D_PBR_BlueNoise = GetUniformLocation("Sampler2D_BlueNoise");
}
