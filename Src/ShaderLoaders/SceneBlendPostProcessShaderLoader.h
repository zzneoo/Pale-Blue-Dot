#pragma once
#include "ShaderLoader.h"

class SceneBlendPostProcessShaderLoader : public ZzShaderLoader
{
public:
	SceneBlendPostProcessShaderLoader();
	~SceneBlendPostProcessShaderLoader();

	void Loadv3SunPos_Uniform(vmath::vec3 sunPos);
	void Loadv3EyePos_Uniform(vmath::vec3 eyePos);
	void Loadv3EyeDir_Uniform(vmath::vec3 eyePos);

	void LoadfVignette_Uniform(GLfloat fVignette);

	void Loadm4InvProjX_Uniform(vmath::mat4 InvProjMatrix);
	void Loadm4InvViewX_Uniform(vmath::mat4 InvViewMatrix);
	void Loadm4LightSpaceX_Uniform(vmath::mat4 LightSpaceMatrix);

	void LoadSampler2D_CloudTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_ShadowMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_DepthTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_SkyTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_SceneTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_SceneMaskTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_GodRaysUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//variables
	const GLchar* filepath_vs = "Resources/Shaders/SceneBlendPostProcess/SceneBlendPostProcess.vs";
	const GLchar* filepath_fs = "Resources/Shaders/SceneBlendPostProcess/SceneBlendPostProcess.fs";

	//extra
	GLuint v3SunPos_Uniform;
	GLuint v3EyeDir_Uniform;
	GLuint v3EyePos_Uniform;
	//matrices
	GLuint  m4InvProjX_Uniform;
	GLuint  m4InvViewX_Uniform;
	GLuint  m4LightSpaceX_Uniform;
	//scene
	GLuint Sampler2D_CloudTextureUniform;
	GLuint Sampler2D_SkyTextureUniform;
	GLuint Sampler2D_SceneTextureUniform;
	GLuint Sampler2D_SceneMaskTextureUniform;
	//post process
	GLuint Sampler2D_GodRaysUniform;
	//extra
	GLuint Sampler2D_ShadowMapTextureUniform;
	GLuint Sampler2D_DepthTextureUniform;

	GLuint fVignetteUniform;

};
