#pragma once
#include "ShaderLoader.h"
class WaterShaderLoader :public ZzShaderLoader
{
public:
	WaterShaderLoader();
	~WaterShaderLoader();

	//matrices
	void LoadModelXUniform(vmath::mat4 matrix);
	void LoadViewXUniform(vmath::mat4 matrix);
	void LoadProjXUniform(vmath::mat4 matrix);
	void LoadModelViewProjXUniform(vmath::mat4 matrix);
	//
	void Loadv3EyePosUniform(vmath::vec3 eyePos);
	void Loadv3SunPosUniform(vmath::vec3 sunPos);
	void Loadv3EyeDirUniform(vmath::vec3 eyeDir);
	void Loadv3ColorFactorUniform(vmath::vec3 v3Color);
	void Loadv3SpecularColorUniform(vmath::vec3 v3SpecularColor);

	void LoadfWaterFactorUniform(GLfloat factor);

	void LoadHeightMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadNormalMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadRefractionTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadReflectionTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainDepthTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar* filepath_vs = "Resources/Shaders/Water/Water.vs";
	const GLchar* filepath_tcs = "Resources/Shaders/Water/Water.tcs";
	const GLchar* filepath_tes = "Resources/Shaders/Water/Water.tes";
	const GLchar* filepath_fs = "Resources/Shaders/Water/Water.fs";

	//uniforms
	GLuint m4ModelX_Uniform;
	GLuint m4ViewX_Uniform;
	GLuint m4ProjX_Uniform;
	GLuint m4ModelViewProjX_Uniform;

	GLuint v3EyePos_Uniform;
	GLuint v3SunPos_Uniform;
	GLuint v3EyeDir_Uniform;

	GLuint fWaterFactor_Uniform;
	GLuint v3ColorFactor_Uniform;
	GLuint v3SpecularColor_Uniform;

	GLuint Sampler2D_HeightMapUniform;
	GLuint Sampler2D_NormalMapUniform;
	GLuint Sampler2D_RefractionUniform;
	GLuint Sampler2D_ReflectionUniform;
	GLuint Sampler2D_TerrainDepthUniform;
};
