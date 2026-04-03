#pragma once
#include "ShaderLoader.h"

class RainDropShaderLoader :public ZzShaderLoader
{
public:
	RainDropShaderLoader();
	~RainDropShaderLoader();

	void LoadViewXUniform(vmath::mat4 matrix);
	void LoadProjXUniform(vmath::mat4 matrix);
	void LoadModelXUniform(vmath::mat4 matrix);

	void LoadEyePosUniform(vmath::vec3 eyePos);
	void LoadReflectionTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadRefractionTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

private:
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	const GLchar* filepath_vs = "Resources/Shaders/RainDrop/RainDrop.vs";
	const GLchar* filepath_fs = "Resources/Shaders/RainDrop/RainDrop.fs";

	//uniforms
	GLuint m4ProjXUniform;
	GLuint m4ViewXUniform;
	GLuint m4ModelXUniform;

	GLuint v3EyePosUniform;
	GLuint SamplerCube_ReflectionTextureUniform;
	GLuint SamplerCube_RefractionTextureUniform;

};



