#include "TerrainBasicShaderLoader.h"
#include"MyOpenGL.h"

TerrainBasicShaderLoader::TerrainBasicShaderLoader()
{
	//uniforms
	//float
	fArtistFactor_Uniform = 0;
	//matrices
	m4ModelX_Uniform = 0;
	m4ViewX_Uniform = 0;
	m4ProjX_Uniform = 0;
	m4ModelViewProjX_Uniform = 0;

	v3SunPos_Uniform = 0;
	v3EyePos_Uniform = 0;

	Sampler2D_Heightmap_Uniform = 0;
	Sampler2D_TerrainNormal_Uniform = 0;

	//Material
	Sampler2D_MaterialAlbedo_Uniform = 0;
	 Sampler2D_MaterialAO_Uniform = 0;
	 Sampler2D_MaterialNormal_Uniform = 0;
	 Sampler2D_MaterialRoughness_Uniform = 0;
	//Sampler2D_TerrainGrassDisplacementUniform = 0;
	
	//shadow
	//Sampler2D_ShadowMapUniform = 0;
	//m4LightSpaceX_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}


TerrainBasicShaderLoader::~TerrainBasicShaderLoader()
{
	
}

//floats
void TerrainBasicShaderLoader::LoadArtistFactor_Uniform(GLfloat fArtistFactor)
{
	UniformLoad1f(fArtistFactor_Uniform, fArtistFactor);
}

//matrices
void TerrainBasicShaderLoader::LoadModelX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelX_Uniform, matrix);
}

void TerrainBasicShaderLoader::LoadViewX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ViewX_Uniform, matrix);
}

void TerrainBasicShaderLoader::LoadProjX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ProjX_Uniform, matrix);
}

void TerrainBasicShaderLoader::LoadJitteredProjX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4JitteredProjX_Uniform, matrix);
}

void TerrainBasicShaderLoader::LoadPrevViewX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4PrevViewX_Uniform, matrix);
}

void TerrainBasicShaderLoader::LoadModelViewProjX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelViewProjX_Uniform, matrix);
}

void TerrainBasicShaderLoader::Loadv3SunPos_Uniform(vmath::vec3 sunPos)
{
	UniformLoadVec3(v3SunPos_Uniform, sunPos);
}
void TerrainBasicShaderLoader::Loadv3EyePos_Uniform(vmath::vec3 eyePos)
{
	UniformLoadVec3(v3EyePos_Uniform, eyePos);
}


void TerrainBasicShaderLoader::LoadHeightMapTexture_Uniform(GLenum TEXTURE,GLuint tNo,GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_Heightmap_Uniform, TEXTURE, tNo, textureID);
}

void TerrainBasicShaderLoader::LoadTerrainNormalTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainNormal_Uniform, TEXTURE, tNo, textureID);
}

//Material-----------------------------------------------------------------------------------------------------------------------------------
void TerrainBasicShaderLoader::LoadSampler2D_MaterialAlbedo_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_MaterialAlbedo_Uniform, TEXTURE, tNo, textureID);
}
void TerrainBasicShaderLoader::LoadSampler2D_MaterialAO_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_MaterialAO_Uniform, TEXTURE, tNo, textureID);
}
void TerrainBasicShaderLoader::LoadSampler2D_MaterialNormal_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_MaterialNormal_Uniform, TEXTURE, tNo, textureID);
}

void TerrainBasicShaderLoader::LoadSampler2D_MaterialRoughness_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_MaterialRoughness_Uniform, TEXTURE, tNo, textureID);
}
//void TerrainBasicShaderLoader::LoadTerrainGrassDisplacementTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
//{
//	UniformLoadTexture2D(Sampler2D_TerrainGrassDisplacementUniform, TEXTURE, tNo, textureID);
//}


////Shadow
//void TerrainBasicShaderLoader::LoadShadowMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
//{
//	UniformLoadTexture2D(Sampler2D_ShadowMapUniform, TEXTURE, tNo, textureID);
//}
//
//void TerrainBasicShaderLoader::Loadm4LightSpaceXUniform(vmath::mat4 matrix)
//{
//	UniformLoadMatrix(m4LightSpaceX_Uniform, matrix);
//}

bool TerrainBasicShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs, filepath_tcs, filepath_tes, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void TerrainBasicShaderLoader::GetAllUniformLocations(void)
{
	//floats
	fArtistFactor_Uniform = GetUniformLocation("fArtistFactor");

	//matrices
	m4ModelX_Uniform = GetUniformLocation("m4ModelX");
	m4ViewX_Uniform = GetUniformLocation("m4ViewX");
	m4ProjX_Uniform = GetUniformLocation("m4ProjX");
	m4PrevViewX_Uniform = GetUniformLocation("m4PrevViewX");
	m4JitteredProjX_Uniform = GetUniformLocation("m4JitteredProjX");

	m4ModelViewProjX_Uniform= GetUniformLocation("m4ModelViewProjX");

	////vectors
	v3SunPos_Uniform= GetUniformLocation("v3SunPos");
	v3EyePos_Uniform= GetUniformLocation("v3EyePos");
	//v3LightColor_Uniform= GetUniformLocation("v3LightColor");

	//samplers
	Sampler2D_Heightmap_Uniform = GetUniformLocation("tSampler_HeightMap");
	Sampler2D_TerrainNormal_Uniform = GetUniformLocation("tSampler_NormalMap");

	//Material
	{
		Sampler2D_MaterialAlbedo_Uniform = GetUniformLocation("tSampler_MaterialAlbedo");
		Sampler2D_MaterialAO_Uniform = GetUniformLocation("tSampler_MaterialAO");
		Sampler2D_MaterialNormal_Uniform = GetUniformLocation("tSampler_MaterialNormal");
		Sampler2D_MaterialRoughness_Uniform = GetUniformLocation("tSampler_MaterialRoughness");
		//Sampler2D_TerrainGrassDisplacementUniform = GetUniformLocation("tSampler_terrainDisplacementGrass");
	}

	//shadow
	//m4LightSpaceX_Uniform = GetUniformLocation("m4LightSpaceX");
	//Sampler2D_ShadowMapUniform = GetUniformLocation("tSampler_ShadowMap");
}
