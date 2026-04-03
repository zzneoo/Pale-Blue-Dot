#include "PBR_AnimatedShaderLoader.h"
#include"MyOpenGL.h"

PBR_AnimatedShaderLoader::PBR_AnimatedShaderLoader()
{
	//uniform locations
	m4ModelViewProjX_Uniform = 0;
	m4JointTransformX_Uniform = 0;
	m4ModelX_Uniform = 0;
	m4ViewX_Uniform = 0;
	m4ProjX_Uniform = 0;
	//vectors
	v3SunPosUniform = 0;
	v3EyePosUniform = 0;
	v3LightColorUniform = 0;
	//Samplers
	Sampler2D_PBR_AlbedoUniform = 0;
	Sampler2D_PBR_NormalUniform = 0;
	Sampler2D_PBR_RoughnessUniform = 0;
	Sampler2D_PBR_AmbientOcclusionUniform = 0;
	Sampler2D_PBR_MetallicUniform = 0;

	//float
	fAmbientFactorUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}
PBR_AnimatedShaderLoader::~PBR_AnimatedShaderLoader()
{
}

//matrices-------------------------------------------------------------------------------------------
void PBR_AnimatedShaderLoader::LoadModelViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelViewProjX_Uniform, matrix);
}
void PBR_AnimatedShaderLoader::Loadm4JointTransformXUniform(vmath::mat4* matricesPointer,GLsizei count)
{
	UniformLoadMatrixCounted(m4JointTransformX_Uniform, matricesPointer, count);
}

void PBR_AnimatedShaderLoader::LoadModelXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelX_Uniform, matrix);
}
void PBR_AnimatedShaderLoader::LoadViewXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ViewX_Uniform, matrix);
}
void PBR_AnimatedShaderLoader::LoadProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ProjX_Uniform, matrix);
}

//vectors-------------------------------------------------------------------------------------------------

void PBR_AnimatedShaderLoader::LoadSunPositionUniform(vmath::vec3 sunPos)
{
	UniformLoadVec3(v3SunPosUniform, sunPos);
}
void PBR_AnimatedShaderLoader::LoadEyePositionUniform(vmath::vec3 eyePos)
{
	UniformLoadVec3(v3EyePosUniform, eyePos);
}

void PBR_AnimatedShaderLoader::LoadLightColorUniform(vmath::vec3 lightColor)
{
	UniformLoadVec3(v3LightColorUniform, lightColor);
}

void PBR_AnimatedShaderLoader::LoadAmbientFactorUniform(GLfloat factor)
{
	UniformLoad1f(fAmbientFactorUniform, factor);
}

//samplers------------------------------------------------------------------------------------------------
void PBR_AnimatedShaderLoader::LoadAlbedoTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_AlbedoUniform, TEXTURE, tNo, textureID);
}
void PBR_AnimatedShaderLoader::LoadNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_NormalUniform, TEXTURE, tNo, textureID);
}
void PBR_AnimatedShaderLoader::LoadRoughnessTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_RoughnessUniform, TEXTURE, tNo, textureID);
}
void PBR_AnimatedShaderLoader::LoadAmbientOcclusionTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_AmbientOcclusionUniform, TEXTURE, tNo, textureID);
}
void PBR_AnimatedShaderLoader::LoadMetallicTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_MetallicUniform, TEXTURE, tNo, textureID);
}
//----------------------------------------------------------------------------------------------------------

bool PBR_AnimatedShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void PBR_AnimatedShaderLoader::GetAllUniformLocations(void)
{
	//matrices
	m4ModelX_Uniform = GetUniformLocation("m4ModelX");
	m4ViewX_Uniform = GetUniformLocation("m4ViewX");
	m4ProjX_Uniform = GetUniformLocation("m4ProjX");

	m4JointTransformX_Uniform = GetUniformLocation("u_jointTransforms");
	//matrices
	m4ModelViewProjX_Uniform = GetUniformLocation("m4ModelViewProjX");
	//vectors
	v3SunPosUniform = GetUniformLocation("v3SunPos");
	v3EyePosUniform = GetUniformLocation("v3EyePos");
	v3LightColorUniform = GetUniformLocation("v3LightColor");
	//samplers
	Sampler2D_PBR_AlbedoUniform = GetUniformLocation("Sampler2D_Albedo");
	Sampler2D_PBR_NormalUniform = GetUniformLocation("Sampler2D_Normal");
	Sampler2D_PBR_RoughnessUniform = GetUniformLocation("Sampler2D_Roughness");
	Sampler2D_PBR_AmbientOcclusionUniform = GetUniformLocation("Sampler2D_AO");
	Sampler2D_PBR_MetallicUniform = GetUniformLocation("Sampler2D_Metallic");

	//float
	fAmbientFactorUniform = GetUniformLocation("fAmbientFactor");
}
