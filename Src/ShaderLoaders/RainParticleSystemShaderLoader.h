#pragma once
#include"ShaderLoader.h"

class RainParticleSystemShaderLoader :public ZzShaderLoader
{
public:
	RainParticleSystemShaderLoader();
	~RainParticleSystemShaderLoader();

	void LoadModelViewProjXUniform(vmath::mat4 matrix);
	void LoadiFrameID_Uniform(GLuint iFrameID);
	void LoadfTime_Uniform(GLfloat fTime);
	void LoadAlbedoTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

private:
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	const GLchar * filepath_vs = "Resources/Shaders/RainParticleSystem/RainParticleSystem.vs";
	const GLchar * filepath_fs = "Resources/Shaders/RainParticleSystem/RainParticleSystem.fs";

	//uniforms
	GLuint ModelViewProjXUniform;
	GLuint Sampler2D_AlbedoTextureUniform;
	GLuint iFrameID_Uniform;
	GLuint fTime_Uniform;
};


