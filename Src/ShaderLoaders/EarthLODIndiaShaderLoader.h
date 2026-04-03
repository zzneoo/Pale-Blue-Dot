#pragma once
#include "ShaderLoader.h"
class EarthLODIndiaShaderLoader :	public ZzShaderLoader
{
public:
	EarthLODIndiaShaderLoader();
	~EarthLODIndiaShaderLoader();

	//matrices
	void LoadEarthLODIndiaFactorUniform(GLfloat EarthFactor);
	void LoadFcoefForLogUniform(GLfloat fCoef);

	void LoadModelXUniform(vmath::mat4 matrix);
	void LoadEarthRotXUniform(vmath::mat4 matrix);
	void LoadViewProjXUniform(vmath::mat4 matrix);
	void LoadJitteredViewProjXUniform(vmath::mat4 matrix);
	void LoadPrevViewProjXUniform(vmath::mat4 matrix);
	//
	void LoadHeightMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadNormalAOMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);


	//maps
	void LoadEarthLODIndiaAlbedo_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadEarthLODBlend_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);


	void Loadv3SunPosUniform(vmath::vec3 sunPos);
	void Loadv3EyePosUniform(vmath::vec3 eyePos);
	void Loadv3TintColor_Uniform(vmath::vec3 Tint);
	void LoadIsCullIndiaLOD_Uniform(GLuint CullIndiaLOD);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar * filepath_vs = "Resources/Shaders/EarthLODIndia/EarthLODIndia.vs";
	const GLchar * filepath_tcs = "Resources/Shaders/EarthLODIndia/EarthLODIndia.tcs";
	const GLchar * filepath_tes = "Resources/Shaders/EarthLODIndia/EarthLODIndia.tes";
	const GLchar * filepath_fs = "Resources/Shaders/EarthLODIndia/EarthLODIndia.fs";

	//uniforms
	GLuint m4ModelX_Uniform;
	GLuint m4PrevViewProjX_Uniform;
	GLuint m4EarthRotX_Uniform;
	GLuint m4ViewProjX_Uniform;
	GLuint m4JitteredViewProjX_Uniform;

	GLuint v3SunPos_Uniform;
	GLuint v3EyePos_Uniform;
	GLuint v3TintColor_Uniform;

	GLuint uCullIndiaLOD_Uniform;

	GLuint Sampler2D_HeightmapUniform;
	//GLuint Sampler2D_WaterGradientUniform;
	GLuint Sampler2D_NormalAOmapUniform;

	GLuint fEarthLODIndiaFactorUniform;
	GLuint FcoefForLog_Uniform;
	
	//maps
	GLuint Sampler2D_EarthLODIndiaAlbedo_Uniform;
	GLuint Sampler2D_EarthLodBlend_Uniform;
	//GLuint Sampler2D_TerrainWaterRBWD_Uniform;

	//shadow
//	GLuint m4LightSpaceX_Uniform;
//	GLuint Sampler2D_ShadowMapUniform;
};

