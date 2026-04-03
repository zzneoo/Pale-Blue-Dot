#pragma once
#include "ShaderLoader.h"
class TerrainShaderLoader :	public ZzShaderLoader
{
public:
	TerrainShaderLoader();
	~TerrainShaderLoader();

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
	void LoadTerrainGrassTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainGrassNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainGrassAOTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainGrassRoughnessTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainGrassDisplacementTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	//sand
	void LoadTerrainSandTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainSandNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainSandAOTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainSandRoughnessTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainSandDisplacementTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	//rock
	void LoadTerrainRockTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainRockNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainRockAOTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainRockRoughnessTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainRockDisplacementTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	//talus
	void LoadTerrainTalusTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainTalusNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainTalusAOTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainTalusRoughnessTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainTalusDisplacementTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	//maps
	void LoadTerrainLandSWTR_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainWaterRBWD_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainWaterGradient_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	//shadow
	void LoadShadowMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void Loadm4LightSpaceXUniform(vmath::mat4 matrix);

	void Loadv3SunPosUniform(vmath::vec3 sunPos);
	void Loadv3EyePosUniform(vmath::vec3 eyePos);
	void Loadv3LightColorUniform(vmath::vec3 LightColor);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar * filepath_vs = "Resources/Shaders/Terrain/Terrain.vs";
	const GLchar * filepath_tcs = "Resources/Shaders/Terrain/Terrain.tcs";
	const GLchar * filepath_tes = "Resources/Shaders/Terrain/Terrain.tes";
	const GLchar * filepath_fs = "Resources/Shaders/Terrain/Terrain.fs";

	//uniforms
	GLuint m4ModelX_Uniform;
	GLuint m4ViewX_Uniform;
	GLuint m4ProjX_Uniform;
	GLuint m4ModelViewProjX_Uniform;

	GLuint v3SunPos_Uniform;
	GLuint v3EyePos_Uniform;
	GLuint v3LightColor_Uniform;
	GLuint Sampler2D_HeightmapUniform;
	GLuint Sampler2D_WaterGradientUniform;
	GLuint Sampler2D_TerrainNormalUniform;

	GLuint fTerrainFactorUniform;

	//grass
	GLuint Sampler2D_TerrainGrassAlbedoUniform;
	GLuint Sampler2D_TerrainGrassAO_Uniform;
	GLuint Sampler2D_TerrainGrassNormalUniform;
	GLuint Sampler2D_TerrainGrassRoughnessUniform;
	GLuint Sampler2D_TerrainGrassDisplacementUniform;
	//sand
	GLuint Sampler2D_TerrainSandAlbedoUniform;
	GLuint Sampler2D_TerrainSandAO_Uniform;
	GLuint Sampler2D_TerrainSandNormalUniform;
	GLuint Sampler2D_TerrainSandRoughnessUniform;
	GLuint Sampler2D_TerrainSandDisplacementUniform;
	//rock
	GLuint Sampler2D_TerrainRockAlbedoUniform;
	GLuint Sampler2D_TerrainRockAO_Uniform;
	GLuint Sampler2D_TerrainRockNormalUniform;
	GLuint Sampler2D_TerrainRockRoughnessUniform;
	GLuint Sampler2D_TerrainRockDisplacementUniform;
	//talus
	GLuint Sampler2D_TerrainTalusAlbedoUniform;
	GLuint Sampler2D_TerrainTalusAO_Uniform;
	GLuint Sampler2D_TerrainTalusNormalUniform;
	GLuint Sampler2D_TerrainTalusRoughnessUniform;
	GLuint Sampler2D_TerrainTalusDisplacementUniform;
	//maps
	GLuint Sampler2D_TerrainLandSWTR_Uniform;
	GLuint Sampler2D_TerrainWaterRBWD_Uniform;

	//shadow
	GLuint m4LightSpaceX_Uniform;
	GLuint Sampler2D_ShadowMapUniform;
};

