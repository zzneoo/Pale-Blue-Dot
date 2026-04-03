#include "PBR_ShaderLoader.h"
#include"MyOpenGL.h"

PBR_ShaderLoader::PBR_ShaderLoader()
{
	//uniform locations
	//floats
	fTimeUniform = 0;
	//matrices
	m4ViewProjX_Uniform = 0;
	m4ModelX_Uniform = 0;

	//vectors
	v3SunPosUniform = 0;
	v3EyePosUniform = 0;
	v3TintColorUniform = 0;
	//Samplers
	Sampler2D_PBR_AlbedoUniform = 0;
	Sampler2D_PBR_NormalUniform = 0;
	Sampler2D_PBR_RoughnessUniform = 0;
	Sampler2D_PBR_AmbientOcclusionUniform = 0;
	Sampler2D_PBR_MetallicUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}
PBR_ShaderLoader::~PBR_ShaderLoader()
{
}

//floats
void PBR_ShaderLoader::LoadTimeUniform(GLfloat time)
{
	UniformLoad1f(fTimeUniform, time);
}

//matrices-------------------------------------------------------------------------------------------
void PBR_ShaderLoader::LoadViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ViewProjX_Uniform, matrix);
}

void PBR_ShaderLoader::LoadModelXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelX_Uniform, matrix);
}

//vectors-------------------------------------------------------------------------------------------------

void PBR_ShaderLoader::LoadSunPositionUniform(vmath::vec3 sunPos)
{
	UniformLoadVec3(v3SunPosUniform,sunPos);
}
void PBR_ShaderLoader::LoadEyePositionUniform(vmath::vec3 eyePos)
{
	UniformLoadVec3(v3EyePosUniform,eyePos);
}

void PBR_ShaderLoader::LoadTintColorUniform(vmath::vec3 tintColor)
{
	UniformLoadVec3(v3TintColorUniform, tintColor);
}

//samplers------------------------------------------------------------------------------------------------
void PBR_ShaderLoader::LoadAlbedoTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_AlbedoUniform, TEXTURE, tNo, textureID);
}
void PBR_ShaderLoader::LoadNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_NormalUniform, TEXTURE, tNo, textureID);
}
void PBR_ShaderLoader::LoadRoughnessTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_RoughnessUniform, TEXTURE, tNo, textureID);
}
void PBR_ShaderLoader::LoadAmbientOcclusionTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_AmbientOcclusionUniform, TEXTURE, tNo, textureID);
}
void PBR_ShaderLoader::LoadMetallicTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_MetallicUniform, TEXTURE, tNo, textureID);
}
//----------------------------------------------------------------------------------------------------------

bool PBR_ShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void PBR_ShaderLoader::GetAllUniformLocations(void)
{

	//floats
	fTimeUniform = GetUniformLocation("fTime");

	//matrices
	m4ViewProjX_Uniform = GetUniformLocation("m4ViewProjX"); 
	m4ModelX_Uniform = GetUniformLocation("m4ModelX");
;
	//vectors
	v3SunPosUniform = GetUniformLocation("v3SunPos");
	v3EyePosUniform = GetUniformLocation("v3EyePos");
	v3TintColorUniform = GetUniformLocation("v3TintColor");
	//samplers
	 Sampler2D_PBR_AlbedoUniform = GetUniformLocation("Sampler2D_Albedo");
	 Sampler2D_PBR_NormalUniform = GetUniformLocation("Sampler2D_Normal");
	 Sampler2D_PBR_RoughnessUniform = GetUniformLocation("Sampler2D_Roughness");
	 Sampler2D_PBR_AmbientOcclusionUniform = GetUniformLocation("Sampler2D_AO");
	 Sampler2D_PBR_MetallicUniform = GetUniformLocation("Sampler2D_Metallic");
}
