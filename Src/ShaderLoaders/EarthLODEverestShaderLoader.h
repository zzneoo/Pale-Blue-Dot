#pragma once
#include "ShaderLoader.h"
class EarthLODEverestShaderLoader :	public ZzShaderLoader
{
public:
	EarthLODEverestShaderLoader();
	~EarthLODEverestShaderLoader();

	void LoadArtistFactorUniform(GLfloat Factor);
	void LoadFcoefForLogUniform(GLfloat fCoef);
	//matrices
	void LoadModelX_Uniform(vmath::mat4 matrix);
	void LoadEarthRotX_Uniform(vmath::mat4 matrix);
	void LoadViewProjX_Uniform(vmath::mat4 matrix);
	void LoadJitteredViewProjX_Uniform(vmath::mat4 matrix);
	void LoadPrevViewProjX_Uniform(vmath::mat4 matrix);
	//
	//maps
	void LoadSampler2D_Albedo_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_HeightMap_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_NormalAOmap_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_LodBlend_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);


	void Loadv3SunPosUniform(vmath::vec3 sunPos);
	void Loadv3EyePosUniform(vmath::vec3 eyePos);
	//void Loadv3LightColorUniform(vmath::vec3 LightColor);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar * filepath_vs = "Resources/Shaders/EarthLODEverest/EarthLODEverest.vs";
	const GLchar * filepath_tcs = "Resources/Shaders/EarthLODEverest/EarthLODEverest.tcs";
	const GLchar * filepath_tes = "Resources/Shaders/EarthLODEverest/EarthLODEverest.tes";
	const GLchar * filepath_fs = "Resources/Shaders/EarthLODEverest/EarthLODEverest.fs";

	//uniforms
	GLuint m4ModelX_Uniform;
	GLuint m4PrevViewProjX_Uniform;
	GLuint m4EarthRotX_Uniform;
	GLuint m4ViewProjX_Uniform;
	GLuint m4JitteredViewProjX_Uniform;

	GLuint v3SunPos_Uniform;
	GLuint v3EyePos_Uniform;
	//GLuint v3LightColor_Uniform;


	GLuint fArtistFactor_Uniform;
	GLuint FcoefForLog_Uniform;
	
	//maps
	GLuint Sampler2D_Albedo_Uniform;
	GLuint Sampler2D_HeightMap_Uniform;
	GLuint Sampler2D_NormalAO_Map_Uniform;

	GLuint Sampler2D_LodBlend_Uniform;
	//GLuint Sampler2D_TerrainWaterRBWD_Uniform;

	//shadow
//	GLuint m4LightSpaceX_Uniform;
//	GLuint Sampler2D_ShadowMapUniform;
};

