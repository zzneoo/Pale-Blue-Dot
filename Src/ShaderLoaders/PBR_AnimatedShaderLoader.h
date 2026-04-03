#pragma once
#include "ShaderLoader.h"
class PBR_AnimatedShaderLoader :public ZzShaderLoader
{
public:
	PBR_AnimatedShaderLoader();
	~PBR_AnimatedShaderLoader();

	//matrices
	void LoadModelViewProjXUniform(vmath::mat4 matrix);
	void Loadm4JointTransformXUniform(vmath::mat4* matricesPointer, GLsizei count);
	void LoadModelXUniform(vmath::mat4 matrix);
	void LoadViewXUniform(vmath::mat4 matrix);
	void LoadProjXUniform(vmath::mat4 matrix);

	//vectors
	void LoadSunPositionUniform(vmath::vec3 sunPos);
	void LoadEyePositionUniform(vmath::vec3 eyePos);
	void LoadLightColorUniform(vmath::vec3 lightColor);

	void LoadAmbientFactorUniform(GLfloat factor);
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
	const GLchar* filepath_vs = "Resources/Shaders/PBR/PBR_Animated.vs";
	const GLchar* filepath_fs = "Resources/Shaders/PBR/PBR_Animated.fs";

	//uniform locations
	GLuint m4ModelViewProjX_Uniform;
	GLuint m4ModelX_Uniform;
	GLuint m4ViewX_Uniform;
	GLuint m4ProjX_Uniform;

	GLuint m4JointTransformX_Uniform;
	//vectors
	GLuint v3SunPosUniform;
	GLuint v3EyePosUniform;
	GLuint v3LightColorUniform;
	//Samplers
	GLuint Sampler2D_PBR_AlbedoUniform;
	GLuint Sampler2D_PBR_NormalUniform;
	GLuint Sampler2D_PBR_RoughnessUniform;
	GLuint Sampler2D_PBR_AmbientOcclusionUniform;
	GLuint Sampler2D_PBR_MetallicUniform;

	//float
	GLuint fAmbientFactorUniform;
};

