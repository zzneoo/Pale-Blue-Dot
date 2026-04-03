#include "TerrainShaderLoader.h"
#include"MyOpenGL.h"

TerrainShaderLoader::TerrainShaderLoader()
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
	v3LightColor_Uniform = 0;
	Sampler2D_HeightmapUniform = 0;
	Sampler2D_WaterGradientUniform = 0;
	Sampler2D_TerrainNormalUniform = 0;

	//grass
	Sampler2D_TerrainGrassAlbedoUniform = 0;
	Sampler2D_TerrainGrassAO_Uniform = 0;
	Sampler2D_TerrainGrassNormalUniform = 0;
	Sampler2D_TerrainGrassRoughnessUniform = 0;
	Sampler2D_TerrainGrassDisplacementUniform = 0;
	//sand
	Sampler2D_TerrainSandAlbedoUniform = 0;
	Sampler2D_TerrainSandAO_Uniform = 0;
	Sampler2D_TerrainSandNormalUniform = 0;
	Sampler2D_TerrainSandRoughnessUniform = 0;
	Sampler2D_TerrainSandDisplacementUniform = 0;
	//rock
	Sampler2D_TerrainRockAlbedoUniform = 0;
	Sampler2D_TerrainRockAO_Uniform = 0;
	Sampler2D_TerrainRockNormalUniform = 0;
	Sampler2D_TerrainRockRoughnessUniform = 0;
	Sampler2D_TerrainRockDisplacementUniform = 0;
	//talus
	Sampler2D_TerrainTalusAlbedoUniform = 0;
	Sampler2D_TerrainTalusAO_Uniform = 0;
	Sampler2D_TerrainTalusNormalUniform = 0;
	Sampler2D_TerrainTalusRoughnessUniform = 0;
	Sampler2D_TerrainTalusDisplacementUniform = 0;
	//maps
	Sampler2D_TerrainLandSWTR_Uniform = 0;
	Sampler2D_TerrainWaterRBWD_Uniform = 0;

	//shadow
	Sampler2D_ShadowMapUniform = 0;
	m4LightSpaceX_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}


TerrainShaderLoader::~TerrainShaderLoader()
{
	
}

//floats
void TerrainShaderLoader::LoadTerrainFactorUniform(GLfloat fTerrainFactor)
{
	UniformLoad1f(fTerrainFactorUniform, fTerrainFactor);
}

//matrices
void TerrainShaderLoader::LoadModelXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelX_Uniform, matrix);
}

void TerrainShaderLoader::LoadViewXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ViewX_Uniform, matrix);
}

void TerrainShaderLoader::LoadProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ProjX_Uniform, matrix);
}

void TerrainShaderLoader::LoadModelViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelViewProjX_Uniform, matrix);
}

void TerrainShaderLoader::Loadv3SunPosUniform(vmath::vec3 sunPos)
{
	UniformLoadVec3(v3SunPos_Uniform, sunPos);
}
void TerrainShaderLoader::Loadv3EyePosUniform(vmath::vec3 eyePos)
{
	UniformLoadVec3(v3EyePos_Uniform, eyePos);
}
void TerrainShaderLoader::Loadv3LightColorUniform(vmath::vec3 LightColor)
{
	UniformLoadVec3(v3LightColor_Uniform, LightColor);
}

void TerrainShaderLoader::LoadHeightMapTextureUniform(GLenum TEXTURE,GLuint tNo,GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_HeightmapUniform, TEXTURE, tNo, textureID);
}

void TerrainShaderLoader::LoadTerrainNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainNormalUniform, TEXTURE, tNo, textureID);
}

//grass-----------------------------------------------------------------------------------------------------------------------------------
void TerrainShaderLoader::LoadTerrainGrassTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainGrassAlbedoUniform, TEXTURE, tNo, textureID);
}
void TerrainShaderLoader::LoadTerrainGrassAOTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainGrassAO_Uniform, TEXTURE, tNo, textureID);
}
void TerrainShaderLoader::LoadTerrainGrassNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainGrassNormalUniform, TEXTURE, tNo, textureID);
}

void TerrainShaderLoader::LoadTerrainGrassRoughnessTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainGrassRoughnessUniform, TEXTURE, tNo, textureID);
}
void TerrainShaderLoader::LoadTerrainGrassDisplacementTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainGrassDisplacementUniform, TEXTURE, tNo, textureID);
}

//sand--------------------------------------------------------------------------------------------------------------------------------------------
void TerrainShaderLoader::LoadTerrainSandTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainSandAlbedoUniform, TEXTURE, tNo, textureID);
}
void TerrainShaderLoader::LoadTerrainSandAOTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainSandAO_Uniform, TEXTURE, tNo, textureID);
}
void TerrainShaderLoader::LoadTerrainSandNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainSandNormalUniform, TEXTURE, tNo, textureID);
}

void TerrainShaderLoader::LoadTerrainSandRoughnessTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainSandRoughnessUniform, TEXTURE, tNo, textureID);
}
void TerrainShaderLoader::LoadTerrainSandDisplacementTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainSandDisplacementUniform, TEXTURE, tNo, textureID);
}

//Rock---------------------------------------------------------------------------------------------------------------------------------------
void TerrainShaderLoader::LoadTerrainRockTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainRockAlbedoUniform, TEXTURE, tNo, textureID);
}
void TerrainShaderLoader::LoadTerrainRockAOTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainRockAO_Uniform, TEXTURE, tNo, textureID);
}
void TerrainShaderLoader::LoadTerrainRockNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainRockNormalUniform, TEXTURE, tNo, textureID);
}

void TerrainShaderLoader::LoadTerrainRockRoughnessTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainRockRoughnessUniform, TEXTURE, tNo, textureID);
}
void TerrainShaderLoader::LoadTerrainRockDisplacementTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainRockDisplacementUniform, TEXTURE, tNo, textureID);
}

//Talus---------------------------------------------------------------------------------------------------------------------------------------
void TerrainShaderLoader::LoadTerrainTalusTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainTalusAlbedoUniform, TEXTURE, tNo, textureID);
}
void TerrainShaderLoader::LoadTerrainTalusAOTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainTalusAO_Uniform, TEXTURE, tNo, textureID);
}
void TerrainShaderLoader::LoadTerrainTalusNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainTalusNormalUniform, TEXTURE, tNo, textureID);
}

void TerrainShaderLoader::LoadTerrainTalusRoughnessTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainTalusRoughnessUniform, TEXTURE, tNo, textureID);
}
void TerrainShaderLoader::LoadTerrainTalusDisplacementTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainTalusDisplacementUniform, TEXTURE, tNo, textureID);
}

//maps------------------------------------------------------------------------------------------------------------------------------------------
void TerrainShaderLoader::LoadTerrainLandSWTR_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainLandSWTR_Uniform, TEXTURE, tNo, textureID);
}

void TerrainShaderLoader::LoadTerrainWaterRBWD_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainWaterRBWD_Uniform, TEXTURE, tNo, textureID);
}
void TerrainShaderLoader::LoadTerrainWaterGradient_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_WaterGradientUniform, TEXTURE, tNo, textureID);
}

//Shadow
void TerrainShaderLoader::LoadShadowMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_ShadowMapUniform, TEXTURE, tNo, textureID);
}

void TerrainShaderLoader::Loadm4LightSpaceXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4LightSpaceX_Uniform, matrix);
}

bool TerrainShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs, filepath_tcs, filepath_tes, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void TerrainShaderLoader::GetAllUniformLocations(void)
{
	//floats
	fTerrainFactorUniform = GetUniformLocation("fTerrainFactor");

	//matrices
	m4ModelX_Uniform = GetUniformLocation("m4ModelX");
	m4ViewX_Uniform = GetUniformLocation("m4ViewX");
	m4ProjX_Uniform = GetUniformLocation("m4ProjX");

	m4ModelViewProjX_Uniform= GetUniformLocation("m4ModelViewProjX");

	//vectors
	v3SunPos_Uniform= GetUniformLocation("v3SunPos");
	v3EyePos_Uniform= GetUniformLocation("v3EyePos");
	v3LightColor_Uniform= GetUniformLocation("v3LightColor");

	//samplers
	Sampler2D_HeightmapUniform = GetUniformLocation("tSampler_heightmap");
	Sampler2D_WaterGradientUniform = GetUniformLocation("tSampler_waterGradient");
	Sampler2D_TerrainNormalUniform = GetUniformLocation("tSampler_terrainNormal");

	//grass
	{
		Sampler2D_TerrainGrassAlbedoUniform = GetUniformLocation("tSampler_terrainAlbedoGrass");
		Sampler2D_TerrainGrassAO_Uniform = GetUniformLocation("tSampler_terrainAO_Grass");
		Sampler2D_TerrainGrassNormalUniform = GetUniformLocation("tSampler_terrainNormalGrass");
		Sampler2D_TerrainGrassRoughnessUniform = GetUniformLocation("tSampler_terrainRoughnessGrass");
		Sampler2D_TerrainGrassDisplacementUniform = GetUniformLocation("tSampler_terrainDisplacementGrass");
	}
	//sand
	{
		Sampler2D_TerrainSandAlbedoUniform = GetUniformLocation("tSampler_terrainAlbedoSand");
		Sampler2D_TerrainSandAO_Uniform = GetUniformLocation("tSampler_terrainAO_Sand");
		Sampler2D_TerrainSandNormalUniform = GetUniformLocation("tSampler_terrainNormalSand");
		Sampler2D_TerrainSandRoughnessUniform = GetUniformLocation("tSampler_terrainRoughnessSand");
		Sampler2D_TerrainSandDisplacementUniform = GetUniformLocation("tSampler_terrainDisplacementSand");
	}
	//rock
	{
		Sampler2D_TerrainRockAlbedoUniform = GetUniformLocation("tSampler_terrainAlbedoRock");
		Sampler2D_TerrainRockAO_Uniform = GetUniformLocation("tSampler_terrainAO_Rock");
		Sampler2D_TerrainRockNormalUniform = GetUniformLocation("tSampler_terrainNormalRock");
		Sampler2D_TerrainRockRoughnessUniform = GetUniformLocation("tSampler_terrainRoughnessRock");
		Sampler2D_TerrainRockDisplacementUniform = GetUniformLocation("tSampler_terrainDisplacementRock");
	}
	//talus
	{
		Sampler2D_TerrainTalusAlbedoUniform = GetUniformLocation("tSampler_terrainAlbedoTalus");
		Sampler2D_TerrainTalusAO_Uniform = GetUniformLocation("tSampler_terrainAO_Talus");
		Sampler2D_TerrainTalusNormalUniform = GetUniformLocation("tSampler_terrainNormalTalus");
		Sampler2D_TerrainTalusRoughnessUniform = GetUniformLocation("tSampler_terrainRoughnessTalus");
		Sampler2D_TerrainTalusDisplacementUniform = GetUniformLocation("tSampler_terrainDisplacementTalus");
	}
	//maps
	Sampler2D_TerrainLandSWTR_Uniform = GetUniformLocation("tSampler_terrainAlbedoLandSWTR");
	Sampler2D_TerrainWaterRBWD_Uniform = GetUniformLocation("tSampler_terrainAlbedoWaterRBWD");

	//shadow
	m4LightSpaceX_Uniform = GetUniformLocation("m4LightSpaceX");
	Sampler2D_ShadowMapUniform = GetUniformLocation("tSampler_ShadowMap");
}
