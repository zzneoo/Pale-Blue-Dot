#include "EarthLODEverestShaderLoader.h"
#include"MyOpenGL.h"

EarthLODEverestShaderLoader::EarthLODEverestShaderLoader()
{
	//uniforms
	//float
	fArtistFactor_Uniform = 0;
	//matrices
	m4ModelX_Uniform = 0;
	m4ViewProjX_Uniform = 0;
	m4JitteredViewProjX_Uniform = 0;
	m4EarthRotX_Uniform = 0;
	m4PrevViewProjX_Uniform = 0;

	FcoefForLog_Uniform = 0;

	v3SunPos_Uniform = 0;
	v3EyePos_Uniform = 0;

	Sampler2D_HeightMap_Uniform = 0;
	Sampler2D_NormalAO_Map_Uniform = 0;
	Sampler2D_Albedo_Uniform = 0;
	Sampler2D_LodBlend_Uniform = 0;

	//shadow
	//Sampler2D_ShadowMapUniform = 0;
	//m4LightSpaceX_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}


EarthLODEverestShaderLoader::~EarthLODEverestShaderLoader()
{
	
}

//floats
void EarthLODEverestShaderLoader::LoadArtistFactorUniform(GLfloat Factor)
{
	UniformLoad1f(fArtistFactor_Uniform, Factor);
}
//Double
void EarthLODEverestShaderLoader::LoadFcoefForLogUniform(GLfloat Fcoef)
{
	UniformLoad1f(FcoefForLog_Uniform, Fcoef);
}

//matrices
void EarthLODEverestShaderLoader::LoadEarthRotX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4EarthRotX_Uniform, matrix);
}
void EarthLODEverestShaderLoader::LoadModelX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelX_Uniform, matrix);
}

void EarthLODEverestShaderLoader::LoadViewProjX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ViewProjX_Uniform, matrix);
}
void EarthLODEverestShaderLoader::LoadJitteredViewProjX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4JitteredViewProjX_Uniform, matrix);
}

void EarthLODEverestShaderLoader::LoadPrevViewProjX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4PrevViewProjX_Uniform, matrix);
}

void EarthLODEverestShaderLoader::Loadv3SunPosUniform(vmath::vec3 sunPos)
{
	UniformLoadVec3(v3SunPos_Uniform, sunPos);
}
void EarthLODEverestShaderLoader::Loadv3EyePosUniform(vmath::vec3 eyePos)
{
	UniformLoadVec3(v3EyePos_Uniform, eyePos);
}

//maps------------------------------------------------------------------------------------------------------------------------------------------
void EarthLODEverestShaderLoader::LoadSampler2D_Albedo_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_Albedo_Uniform, TEXTURE, tNo, textureID);
}
void EarthLODEverestShaderLoader::LoadSampler2D_HeightMap_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_HeightMap_Uniform, TEXTURE, tNo, textureID);
}

void EarthLODEverestShaderLoader::LoadSampler2D_NormalAOmap_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_NormalAO_Map_Uniform, TEXTURE, tNo, textureID);
}

void EarthLODEverestShaderLoader::LoadSampler2D_LodBlend_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_LodBlend_Uniform, TEXTURE, tNo, textureID);
}


//void EarthLOD1ShaderLoader::LoadTerrainWaterGradient_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
//{
//	UniformLoadTexture2D(Sampler2D_WaterGradientUniform, TEXTURE, tNo, textureID);
//}

////Shadow
//void EarthLOD1ShaderLoader::LoadShadowMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
//{
//	UniformLoadTexture2D(Sampler2D_ShadowMapUniform, TEXTURE, tNo, textureID);
//}
//
//void EarthLOD1ShaderLoader::Loadm4LightSpaceXUniform(vmath::mat4 matrix)
//{
//	UniformLoadMatrix(m4LightSpaceX_Uniform, matrix);
//}

bool EarthLODEverestShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs, filepath_tcs, filepath_tes, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void EarthLODEverestShaderLoader::GetAllUniformLocations(void)
{
	//floats
	fArtistFactor_Uniform = GetUniformLocation("fArtistFactor");
	FcoefForLog_Uniform = GetUniformLocation("FcoefForLog");

	//matrices
	m4ModelX_Uniform = GetUniformLocation("m4ModelX");
	m4PrevViewProjX_Uniform = GetUniformLocation("m4PrevViewProjX");
	m4EarthRotX_Uniform = GetUniformLocation("m4EarthRotX");

	m4ViewProjX_Uniform= GetUniformLocation("m4ViewProjX");
	m4JitteredViewProjX_Uniform= GetUniformLocation("m4JitteredViewProjX");

	////vectors
	v3SunPos_Uniform= GetUniformLocation("v3SunPos");
	v3EyePos_Uniform= GetUniformLocation("v3EyePos");
	//v3LightColor_Uniform= GetUniformLocation("v3LightColor");

	//Samplers
	Sampler2D_Albedo_Uniform = GetUniformLocation("tSampler_Albedo");
	Sampler2D_HeightMap_Uniform = GetUniformLocation("tSampler_Heightmap");
	Sampler2D_NormalAO_Map_Uniform = GetUniformLocation("tSampler_NormalAO");
	Sampler2D_LodBlend_Uniform = GetUniformLocation("tSampler_LODBlend");

	//shadow
	//m4LightSpaceX_Uniform = GetUniformLocation("m4LightSpaceX");
	//Sampler2D_ShadowMapUniform = GetUniformLocation("tSampler_ShadowMap");
}
