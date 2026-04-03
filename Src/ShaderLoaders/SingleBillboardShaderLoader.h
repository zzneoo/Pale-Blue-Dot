#pragma once
#include "ShaderLoader.h"

class SingleBillboardShaderLoader :public ZzShaderLoader
{
public:
	SingleBillboardShaderLoader();
	~SingleBillboardShaderLoader();

	void LoadModelViewProjXUniform(vmath::mat4 matrix);
	void Loadv3ScaleUniform(vmath::vec3 scale);
	void LoadfIntensityUniform(GLfloat Intensity);
	void LoadSamplerBillboardUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);


private:
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	const GLchar* filepath_vs = "Resources/Shaders/SingleBillboard/SingleBillboard.vs";
	const GLchar* filepath_fs = "Resources/Shaders/SingleBillboard/SingleBillboard.fs";

	//uniforms
	GLuint m4ModelViewProjXUniform;
	GLuint v3ScaleUniform;
	GLuint fIntensityUniform;
	GLuint tSamplerBillboardUniform;

};



