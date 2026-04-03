#pragma once
#include"ShaderLoader.h"

class CubeMapShaderLoader :public ZzShaderLoader
{
public:
	CubeMapShaderLoader();
	~CubeMapShaderLoader();

	void LoadModelViewProjXUniform(vmath::mat4 matrix);
	void LoadAlbedoCubeMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

private:
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	const GLchar* filepath_vs = "Resources/Shaders/CubeMap/CubeMap.vs";
	const GLchar* filepath_fs = "Resources/Shaders/CubeMap/CubeMap.fs";

	//uniforms
	GLuint ModelViewProjXUniform;
	GLuint SamplerCube_AlbedoTextureUniform;

};



