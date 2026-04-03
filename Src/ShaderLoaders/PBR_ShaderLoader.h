#pragma once
#include "ShaderLoader.h"
class PBR_ShaderLoader :public ZzShaderLoader
{
public:
	PBR_ShaderLoader();
	~PBR_ShaderLoader();

	//floats
	void LoadTimeUniform(GLfloat time);

	//matrices
	void LoadModelXUniform(vmath::mat4 matrix);
	void LoadViewProjXUniform(vmath::mat4 matrix);

	//vectors
	void LoadSunPositionUniform(vmath::vec3 sunPos);
	void LoadEyePositionUniform(vmath::vec3 eyePos);
	void LoadTintColorUniform(vmath::vec3 eyePos);
	//Samplers
	void LoadAlbedoTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadRoughnessTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadAmbientOcclusionTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadMetallicTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Variables
	//shader file names
	const GLchar* filepath_vs = "Resources/Shaders/PBR/DefferedPBR.vs";
	const GLchar* filepath_fs = "Resources/Shaders/PBR/DefferedPBR.fs";

	//uniform locations
	//floats
	GLuint fTimeUniform;
	//matrices
	GLuint m4ModelX_Uniform;
	GLuint m4ViewProjX_Uniform;

	//vectors
	GLuint v3SunPosUniform;
	GLuint v3EyePosUniform;
	GLuint v3TintColorUniform;
	//Samplers
	GLuint Sampler2D_PBR_AlbedoUniform;
	GLuint Sampler2D_PBR_NormalUniform;
	GLuint Sampler2D_PBR_RoughnessUniform;
	GLuint Sampler2D_PBR_AmbientOcclusionUniform;
	GLuint Sampler2D_PBR_MetallicUniform;
};

