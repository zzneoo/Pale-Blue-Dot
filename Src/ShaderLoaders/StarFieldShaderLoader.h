#pragma once
#include"ShaderLoader.h"

class StarFieldShaderLoader :public ZzShaderLoader
{
public:
	StarFieldShaderLoader();
	~StarFieldShaderLoader();

	void LoadModelViewProjXUniform(vmath::mat4 matrix);	
	void LoadIntensityFactorUniform(GLfloat Factor);

private:
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	const GLchar * filepath_vs = "Resources/Shaders/StarField/StarField.vs";
	const GLchar * filepath_fs = "Resources/Shaders/StarField/StarField.fs";

	//uniforms
	GLuint m4ModelViewProjXUniform;
	GLuint fIntensityFactorUniform;
};


