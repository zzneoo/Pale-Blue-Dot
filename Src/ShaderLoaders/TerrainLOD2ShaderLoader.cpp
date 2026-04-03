#include "TerrainLOD2ShaderLoader.h"
#include"MyOpenGL.h"

TerrainLOD2ShaderLoader::TerrainLOD2ShaderLoader()
{
	//uniforms
	//float
	fTerrainFactorUniform = 0;
	//matrices
	m4ModelX_Uniform = 0;
	m4ViewX_Uniform = 0;
	m4ProjX_Uniform = 0;
	m4ModelViewProjX_Uniform = 0;

	v3SunPos_Uniform = 0;
	v3EyePos_Uniform = 0;

	Sampler2D_HeightmapUniform = 0;
	//Sampler2D_WaterGradientUniform = 0;
	Sampler2D_TerrainNormalUniform = 0;

	//grass
	//Sampler2D_TerrainGrassAlbedoUniform = 0;
	//Sampler2D_TerrainGrassAO_Uniform = 0;
	//Sampler2D_TerrainGrassNormalUniform = 0;
	//Sampler2D_TerrainGrassRoughnessUniform = 0;
	//Sampler2D_TerrainGrassDisplacementUniform = 0;
	
	//maps
	Sampler2D_TerrainWearSlopeSnowAO_Uniform = 0;

	//shadow
	//Sampler2D_ShadowMapUniform = 0;
	//m4LightSpaceX_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}


TerrainLOD2ShaderLoader::~TerrainLOD2ShaderLoader()
{
	
}

//floats
void TerrainLOD2ShaderLoader::LoadTerrainFactorUniform(GLfloat fTerrainFactor)
{
	UniformLoad1f(fTerrainFactorUniform, fTerrainFactor);
}

//matrices
void TerrainLOD2ShaderLoader::LoadModelXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelX_Uniform, matrix);
}

void TerrainLOD2ShaderLoader::LoadViewXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ViewX_Uniform, matrix);
}

void TerrainLOD2ShaderLoader::LoadProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ProjX_Uniform, matrix);
}

void TerrainLOD2ShaderLoader::LoadModelViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelViewProjX_Uniform, matrix);
}

void TerrainLOD2ShaderLoader::Loadv3SunPosUniform(vmath::vec3 sunPos)
{
	UniformLoadVec3(v3SunPos_Uniform, sunPos);
}
void TerrainLOD2ShaderLoader::Loadv3EyePosUniform(vmath::vec3 eyePos)
{
	UniformLoadVec3(v3EyePos_Uniform, eyePos);
}


void TerrainLOD2ShaderLoader::LoadHeightMapTextureUniform(GLenum TEXTURE,GLuint tNo,GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_HeightmapUniform, TEXTURE, tNo, textureID);
}

void TerrainLOD2ShaderLoader::LoadTerrainNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainNormalUniform, TEXTURE, tNo, textureID);
}

////grass-----------------------------------------------------------------------------------------------------------------------------------
//void TerrainLOD2ShaderLoader::LoadTerrainGrassTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
//{
//	UniformLoadTexture2D(Sampler2D_TerrainGrassAlbedoUniform, TEXTURE, tNo, textureID);
//}
//void TerrainLOD2ShaderLoader::LoadTerrainGrassAOTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
//{
//	UniformLoadTexture2D(Sampler2D_TerrainGrassAO_Uniform, TEXTURE, tNo, textureID);
//}
//void TerrainLOD2ShaderLoader::LoadTerrainGrassNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
//{
//	UniformLoadTexture2D(Sampler2D_TerrainGrassNormalUniform, TEXTURE, tNo, textureID);
//}
//
//void TerrainLOD2ShaderLoader::LoadTerrainGrassRoughnessTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
//{
//	UniformLoadTexture2D(Sampler2D_TerrainGrassRoughnessUniform, TEXTURE, tNo, textureID);
//}
//void TerrainLOD2ShaderLoader::LoadTerrainGrassDisplacementTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
//{
//	UniformLoadTexture2D(Sampler2D_TerrainGrassDisplacementUniform, TEXTURE, tNo, textureID);
//}


//maps------------------------------------------------------------------------------------------------------------------------------------------
void TerrainLOD2ShaderLoader::LoadTerrainWearSlopeSnowAO_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainWearSlopeSnowAO_Uniform, TEXTURE, tNo, textureID);
}


//void TerrainLOD2ShaderLoader::LoadTerrainWaterGradient_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
//{
//	UniformLoadTexture2D(Sampler2D_WaterGradientUniform, TEXTURE, tNo, textureID);
//}

////Shadow
//void TerrainLOD2ShaderLoader::LoadShadowMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
//{
//	UniformLoadTexture2D(Sampler2D_ShadowMapUniform, TEXTURE, tNo, textureID);
//}
//
//void TerrainLOD2ShaderLoader::Loadm4LightSpaceXUniform(vmath::mat4 matrix)
//{
//	UniformLoadMatrix(m4LightSpaceX_Uniform, matrix);
//}

bool TerrainLOD2ShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs, filepath_tcs, filepath_tes, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void TerrainLOD2ShaderLoader::GetAllUniformLocations(void)
{
	//floats
	fTerrainFactorUniform = GetUniformLocation("fTerrainFactor");

	//matrices
	m4ModelX_Uniform = GetUniformLocation("m4ModelX");
	m4ViewX_Uniform = GetUniformLocation("m4ViewX");
	m4ProjX_Uniform = GetUniformLocation("m4ProjX");

	m4ModelViewProjX_Uniform= GetUniformLocation("m4ModelViewProjX");

	////vectors
	v3SunPos_Uniform= GetUniformLocation("v3SunPos");
	v3EyePos_Uniform= GetUniformLocation("v3EyePos");
	//v3LightColor_Uniform= GetUniformLocation("v3LightColor");

	//samplers
	Sampler2D_HeightmapUniform = GetUniformLocation("tSampler_heightmap");
//	Sampler2D_WaterGradientUniform = GetUniformLocation("tSampler_waterGradient");
	Sampler2D_TerrainNormalUniform = GetUniformLocation("tSampler_terrainNormal");

	////grass
	//{
	//	Sampler2D_TerrainGrassAlbedoUniform = GetUniformLocation("tSampler_terrainAlbedoGrass");
	//	Sampler2D_TerrainGrassAO_Uniform = GetUniformLocation("tSampler_terrainAO_Grass");
	//	Sampler2D_TerrainGrassNormalUniform = GetUniformLocation("tSampler_terrainNormalGrass");
	//	Sampler2D_TerrainGrassRoughnessUniform = GetUniformLocation("tSampler_terrainRoughnessGrass");
	//	Sampler2D_TerrainGrassDisplacementUniform = GetUniformLocation("tSampler_terrainDisplacementGrass");
	//}

	//maps
	Sampler2D_TerrainWearSlopeSnowAO_Uniform = GetUniformLocation("tSampler_terrainWearSlopeSnowAO");
	//Sampler2D_TerrainWaterRBWD_Uniform = GetUniformLocation("tSampler_terrainAlbedoWaterRBWD");

	//shadow
	//m4LightSpaceX_Uniform = GetUniformLocation("m4LightSpaceX");
	//Sampler2D_ShadowMapUniform = GetUniformLocation("tSampler_ShadowMap");
}
