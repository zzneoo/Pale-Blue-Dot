#pragma once
#include"ShaderLoader.h"

class ParticleSystemShaderLoader :public ZzShaderLoader
{
public:
	ParticleSystemShaderLoader();
	~ParticleSystemShaderLoader();

	void LoadModelViewProjXUniform(vmath::mat4 matrix);
	void LoadiFrameID_Uniform(GLuint iFrameID);
	void LoadfTime_Uniform(GLfloat fTime);
	void LoadAlbedoTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

private:
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	const GLchar * filepath_vs = "Resources/Shaders/ParticleSystem/ParticleSystem.vs";
	const GLchar * filepath_fs = "Resources/Shaders/ParticleSystem/ParticleSystem.fs";

	//uniforms
	GLuint ModelViewProjXUniform;
	GLuint Sampler2D_AlbedoTextureUniform;
	GLuint iFrameID_Uniform;
	GLuint fTime_Uniform;
};


