#pragma once
#include "ShaderLoader.h"
class TerrainBasicShaderLoader :	public ZzShaderLoader
{
public:
	TerrainBasicShaderLoader();
	~TerrainBasicShaderLoader();

	//matrices
	void LoadArtistFactor_Uniform(GLfloat fArtistFactor);

	void LoadModelX_Uniform(vmath::mat4 matrix);
	void LoadViewX_Uniform(vmath::mat4 matrix);
	void LoadProjX_Uniform(vmath::mat4 matrix);
	void LoadJitteredProjX_Uniform(vmath::mat4 matrix);
	void LoadPrevViewX_Uniform(vmath::mat4 matrix);
	void LoadModelViewProjX_Uniform(vmath::mat4 matrix);
	//
	void LoadHeightMapTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainNormalTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	//Material
	void LoadSampler2D_MaterialAlbedo_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_MaterialAO_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_MaterialNormal_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_MaterialRoughness_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	//void LoadTerrainGrassDisplacementTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	//shadow
	//void LoadShadowMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	//void Loadm4LightSpaceXUniform(vmath::mat4 matrix);

	void Loadv3SunPos_Uniform(vmath::vec3 sunPos);
	void Loadv3EyePos_Uniform(vmath::vec3 eyePos);
	//void Loadv3LightColorUniform(vmath::vec3 LightColor);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar * filepath_vs = "Resources/Shaders/TerrainBasic3D/TerrainBasic3D.vs";
	const GLchar * filepath_tcs = "Resources/Shaders/TerrainBasic3D/TerrainBasic3D.tcs";
	const GLchar * filepath_tes = "Resources/Shaders/TerrainBasic3D/TerrainBasic3D.tes";
	const GLchar * filepath_fs = "Resources/Shaders/TerrainBasic3D/TerrainBasic3D.fs";

	//uniforms
	GLuint m4ModelX_Uniform;
	GLuint m4ViewX_Uniform;
	GLuint m4ProjX_Uniform;
	GLuint m4PrevViewX_Uniform;
	GLuint m4JitteredProjX_Uniform;
	GLuint m4ModelViewProjX_Uniform;

	GLuint v3SunPos_Uniform;
	GLuint v3EyePos_Uniform;

	GLuint Sampler2D_Heightmap_Uniform;
	GLuint Sampler2D_TerrainNormal_Uniform;

	GLuint fArtistFactor_Uniform;

	//Material
	GLuint Sampler2D_MaterialAlbedo_Uniform;
	GLuint Sampler2D_MaterialAO_Uniform;
	GLuint Sampler2D_MaterialNormal_Uniform;
	GLuint Sampler2D_MaterialRoughness_Uniform;
//	GLuint Sampler2D_MaterialDisplacement_Uniform;
	
	//maps

	//shadow
//	GLuint m4LightSpaceX_Uniform;
//	GLuint Sampler2D_ShadowMapUniform;
};

