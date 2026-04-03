#include "EarthLODIndiaShaderLoader.h"
#include"MyOpenGL.h"

EarthLODIndiaShaderLoader::EarthLODIndiaShaderLoader()
{
	//uniforms
	//float
	fEarthLODIndiaFactorUniform = 0;
	//uint
	uCullIndiaLOD_Uniform = 0;
	//matrices
	m4ModelX_Uniform = 0;
	m4ViewProjX_Uniform = 0;
	m4JitteredViewProjX_Uniform = 0;
	m4EarthRotX_Uniform = 0;
	m4PrevViewProjX_Uniform = 0;
	Sampler2D_EarthLodBlend_Uniform = 0;
	FcoefForLog_Uniform = 0;

	v3SunPos_Uniform = 0;
	v3EyePos_Uniform = 0;
	v3TintColor_Uniform = 0;

	Sampler2D_HeightmapUniform = 0;
	//Sampler2D_WaterGradientUniform = 0;
	Sampler2D_NormalAOmapUniform = 0;
	
	//maps
	Sampler2D_EarthLODIndiaAlbedo_Uniform = 0;

	//shadow
	//Sampler2D_ShadowMapUniform = 0;
	//m4LightSpaceX_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}


EarthLODIndiaShaderLoader::~EarthLODIndiaShaderLoader()
{
	
}

//floats
void EarthLODIndiaShaderLoader::LoadEarthLODIndiaFactorUniform(GLfloat fTerrainFactor)
{
	UniformLoad1f(fEarthLODIndiaFactorUniform, fTerrainFactor);
}
//Double
void EarthLODIndiaShaderLoader::LoadFcoefForLogUniform(GLfloat Fcoef)
{
	UniformLoad1f(FcoefForLog_Uniform, Fcoef);
}

//matrices
void EarthLODIndiaShaderLoader::LoadEarthRotXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4EarthRotX_Uniform, matrix);
}
void EarthLODIndiaShaderLoader::LoadModelXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelX_Uniform, matrix);
}

void EarthLODIndiaShaderLoader::LoadViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ViewProjX_Uniform, matrix);
}
void EarthLODIndiaShaderLoader::LoadJitteredViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4JitteredViewProjX_Uniform, matrix);
}

void EarthLODIndiaShaderLoader::LoadPrevViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4PrevViewProjX_Uniform, matrix);
}

void EarthLODIndiaShaderLoader::Loadv3SunPosUniform(vmath::vec3 sunPos)
{
	UniformLoadVec3(v3SunPos_Uniform, sunPos);
}
void EarthLODIndiaShaderLoader::Loadv3EyePosUniform(vmath::vec3 eyePos)
{
	UniformLoadVec3(v3EyePos_Uniform, eyePos);
}

void EarthLODIndiaShaderLoader::Loadv3TintColor_Uniform(vmath::vec3 Tint)
{
	UniformLoadVec3(v3TintColor_Uniform, Tint);
}

void EarthLODIndiaShaderLoader::LoadIsCullIndiaLOD_Uniform(GLuint CullIndiaLOD)
{
	UniformLoad1ui(uCullIndiaLOD_Uniform, CullIndiaLOD);
}


void EarthLODIndiaShaderLoader::LoadHeightMapTextureUniform(GLenum TEXTURE,GLuint tNo,GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_HeightmapUniform, TEXTURE, tNo, textureID);
}

void EarthLODIndiaShaderLoader::LoadNormalAOMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_NormalAOmapUniform, TEXTURE, tNo, textureID);
}


//maps------------------------------------------------------------------------------------------------------------------------------------------
void EarthLODIndiaShaderLoader::LoadEarthLODIndiaAlbedo_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_EarthLODIndiaAlbedo_Uniform, TEXTURE, tNo, textureID);
}
void EarthLODIndiaShaderLoader::LoadEarthLODBlend_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_EarthLodBlend_Uniform, TEXTURE, tNo, textureID);
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

bool EarthLODIndiaShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs, filepath_tcs, filepath_tes, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void EarthLODIndiaShaderLoader::GetAllUniformLocations(void)
{
	//floats
	fEarthLODIndiaFactorUniform = GetUniformLocation("fEarthFactor");
	FcoefForLog_Uniform = GetUniformLocation("FcoefForLog");

	//uint
	uCullIndiaLOD_Uniform = GetUniformLocation("uCullIndiaLOD");

	//matrices
	m4ModelX_Uniform = GetUniformLocation("m4ModelX");
	m4PrevViewProjX_Uniform = GetUniformLocation("m4PrevViewProjX");
	m4EarthRotX_Uniform = GetUniformLocation("m4EarthRotX");

	m4ViewProjX_Uniform= GetUniformLocation("m4ViewProjX");
	m4JitteredViewProjX_Uniform= GetUniformLocation("m4JitteredViewProjX");

	////vectors
	v3SunPos_Uniform= GetUniformLocation("v3SunPos");
	v3EyePos_Uniform= GetUniformLocation("v3EyePos");
	v3TintColor_Uniform = GetUniformLocation("v3TintColor");

	//samplers
	Sampler2D_HeightmapUniform = GetUniformLocation("tSampler_Heightmap");
//	Sampler2D_WaterGradientUniform = GetUniformLocation("tSampler_waterGradient");
	Sampler2D_NormalAOmapUniform = GetUniformLocation("tSampler_NormalAO");


	//maps
	Sampler2D_EarthLODIndiaAlbedo_Uniform = GetUniformLocation("tSampler_EarthLODIndiaAlbedo");
	Sampler2D_EarthLodBlend_Uniform = GetUniformLocation("tSampler_EarthLODBlend");

	//shadow
	//m4LightSpaceX_Uniform = GetUniformLocation("m4LightSpaceX");
	//Sampler2D_ShadowMapUniform = GetUniformLocation("tSampler_ShadowMap");
}
