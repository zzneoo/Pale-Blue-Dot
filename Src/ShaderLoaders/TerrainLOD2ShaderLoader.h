#pragma once
#include "ShaderLoader.h"
class TerrainLOD2ShaderLoader :	public ZzShaderLoader
{
public:
	TerrainLOD2ShaderLoader();
	~TerrainLOD2ShaderLoader();

	//matrices
	void LoadTerrainFactorUniform(GLfloat terrainFactor);

	void LoadModelXUniform(vmath::mat4 matrix);
	void LoadViewXUniform(vmath::mat4 matrix);
	void LoadProjXUniform(vmath::mat4 matrix);
	void LoadModelViewProjXUniform(vmath::mat4 matrix);
	//
	void LoadHeightMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	//grass
	//void LoadTerrainGrassTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	//void LoadTerrainGrassNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	//void LoadTerrainGrassAOTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	//void LoadTerrainGrassRoughnessTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	//void LoadTerrainGrassDisplacementTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);


	//maps
	void LoadTerrainWearSlopeSnowAO_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	//void LoadTerrainWaterGradient_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	//shadow
	//void LoadShadowMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	//void Loadm4LightSpaceXUniform(vmath::mat4 matrix);

	void Loadv3SunPosUniform(vmath::vec3 sunPos);
	void Loadv3EyePosUniform(vmath::vec3 eyePos);
	//void Loadv3LightColorUniform(vmath::vec3 LightColor);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar * filepath_vs = "Resources/Shaders/TerrainLOD2/Terrain.vs";
	const GLchar * filepath_tcs = "Resources/Shaders/TerrainLOD2/Terrain.tcs";
	const GLchar * filepath_tes = "Resources/Shaders/TerrainLOD2/Terrain.tes";
	const GLchar * filepath_fs = "Resources/Shaders/TerrainLOD2/Terrain.fs";

	//uniforms
	GLuint m4ModelX_Uniform;
	GLuint m4ViewX_Uniform;
	GLuint m4ProjX_Uniform;
	GLuint m4ModelViewProjX_Uniform;

	GLuint v3SunPos_Uniform;
	GLuint v3EyePos_Uniform;
	//GLuint v3LightColor_Uniform;
	GLuint Sampler2D_HeightmapUniform;
	//GLuint Sampler2D_WaterGradientUniform;
	GLuint Sampler2D_TerrainNormalUniform;

	GLuint fTerrainFactorUniform;

	//grass
	//GLuint Sampler2D_TerrainGrassAlbedoUniform;
	//GLuint Sampler2D_TerrainGrassAO_Uniform;
	//GLuint Sampler2D_TerrainGrassNormalUniform;
	//GLuint Sampler2D_TerrainGrassRoughnessUniform;
	//GLuint Sampler2D_TerrainGrassDisplacementUniform;
	
	//maps
	GLuint Sampler2D_TerrainWearSlopeSnowAO_Uniform;
	//GLuint Sampler2D_TerrainWaterRBWD_Uniform;

	//shadow
//	GLuint m4LightSpaceX_Uniform;
//	GLuint Sampler2D_ShadowMapUniform;
};

