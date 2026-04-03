#include "GroundShaderLoader.h"
#include"MyOpenGL.h"

GroundShaderLoader::GroundShaderLoader()
{
	//uniforms
	//float
	fGroundFactorUniform = 0;
	fTimeUniform = 0;
	//matrices
	m4ModelX_Uniform = 0;
	m4ViewX_Uniform = 0;
	m4ProjX_Uniform = 0;
	m4ModelViewProjX_Uniform = 0;
	m4LightSpaceX_Uniform = 0;

	v3SunPos_Uniform = 0;
	v3EyePos_Uniform = 0;
	v3LightColor_Uniform = 0;

	//ground
	Sampler2D_GroundAlbedoUniform = 0;
	Sampler2D_GroundAO_Uniform = 0;
	Sampler2D_GroundNormalUniform = 0;
	Sampler2D_GroundRoughnessUniform = 0;
	Sampler2D_GroundDisplacementUniform = 0;
	Sampler2D_ShadowMapUniform = 0;


	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}


GroundShaderLoader::~GroundShaderLoader()
{
	
}

//floats
void GroundShaderLoader::LoadfGroundFactorUniform(GLfloat fTerrainFactor)
{
	UniformLoad1f(fGroundFactorUniform, fTerrainFactor);
}
void GroundShaderLoader::LoadfTimeUniform(GLfloat time)
{
	UniformLoad1f(fTimeUniform, time);
}

//matrices
void GroundShaderLoader::LoadModelXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelX_Uniform, matrix);
}

void GroundShaderLoader::LoadViewXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ViewX_Uniform, matrix);
}

void GroundShaderLoader::LoadProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ProjX_Uniform, matrix);
}

void GroundShaderLoader::LoadModelViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelViewProjX_Uniform, matrix);
}

void GroundShaderLoader::Loadm4LightSpaceXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4LightSpaceX_Uniform, matrix);
}


//------------------------------------------
void GroundShaderLoader::Loadv3SunPosUniform(vmath::vec3 sunPos)
{
	UniformLoadVec3(v3SunPos_Uniform, sunPos);
}
void GroundShaderLoader::Loadv3EyePosUniform(vmath::vec3 eyePos)
{
	UniformLoadVec3(v3EyePos_Uniform, eyePos);
}
void GroundShaderLoader::Loadv3LightColorUniform(vmath::vec3 LightColor)
{
	UniformLoadVec3(v3LightColor_Uniform, LightColor);
}

//grass-----------------------------------------------------------------------------------------------------------------------------------
void GroundShaderLoader::LoadTerrainGroundTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_GroundAlbedoUniform, TEXTURE, tNo, textureID);
}
void GroundShaderLoader::LoadTerrainGroundAOTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_GroundAO_Uniform, TEXTURE, tNo, textureID);
}
void GroundShaderLoader::LoadTerrainGroundNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_GroundNormalUniform, TEXTURE, tNo, textureID);
}

void GroundShaderLoader::LoadTerrainGroundRoughnessTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_GroundRoughnessUniform, TEXTURE, tNo, textureID);
}
void GroundShaderLoader::LoadTerrainGroundDisplacementTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_GroundDisplacementUniform, TEXTURE, tNo, textureID);
}

void GroundShaderLoader::LoadShadowMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_ShadowMapUniform, TEXTURE, tNo, textureID);
}

bool GroundShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs, filepath_tcs, filepath_tes, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void GroundShaderLoader::GetAllUniformLocations(void)
{
	//floats
	fGroundFactorUniform = GetUniformLocation("fGroundFactor");
	fTimeUniform = GetUniformLocation("fTime");

	//matrices
	m4ModelX_Uniform = GetUniformLocation("m4ModelX");
	m4ViewX_Uniform = GetUniformLocation("m4ViewX");
	m4ProjX_Uniform = GetUniformLocation("m4ProjX");

	m4ModelViewProjX_Uniform= GetUniformLocation("m4ModelViewProjX");

	m4LightSpaceX_Uniform = GetUniformLocation("m4LightSpaceX");

	//vectors
	v3SunPos_Uniform= GetUniformLocation("v3SunPos");
	v3EyePos_Uniform= GetUniformLocation("v3EyePos");
	v3LightColor_Uniform= GetUniformLocation("v3LightColor");


	//Ground
	{
		Sampler2D_GroundAlbedoUniform = GetUniformLocation("tSampler_AlbedoGround");
		Sampler2D_GroundAO_Uniform = GetUniformLocation("tSampler_AO_Ground");
		Sampler2D_GroundNormalUniform = GetUniformLocation("tSampler_NormalGround");
		Sampler2D_GroundRoughnessUniform = GetUniformLocation("tSampler_RoughnessGround");
		Sampler2D_GroundDisplacementUniform = GetUniformLocation("tSampler_DisplacementGround");
	}
	Sampler2D_ShadowMapUniform = GetUniformLocation("tSampler_ShadowMap");

}
