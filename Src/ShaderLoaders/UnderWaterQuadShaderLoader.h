#pragma once
#include"ShaderLoader.h"

class UnderWaterQuadShaderLoader :public ZzShaderLoader
{
public:
	UnderWaterQuadShaderLoader();
	~UnderWaterQuadShaderLoader();

	void LoadModelViewProjXUniform(vmath::mat4 matrix);
	void LoadModelXUniform(vmath::mat4 matrix);
	void Loadv3ColorUniform(vmath::vec3 color);
	void Loadv3SunPosUniform(vmath::vec3 sunPos);
	void Loadv3EyePosUniform(vmath::vec3 eyePos);

private:
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	const GLchar* filepath_vs = "Resources/Shaders/UnderWaterQuad/UnderWaterQuad.vs";
	const GLchar* filepath_fs = "Resources/Shaders/UnderWaterQuad/UnderWaterQuad.fs";

	//uniforms
	GLuint ModelViewProjXUniform;
	GLuint ModelXUniform;
	GLuint v3ColorUniform;
	GLuint v3SunPosUniform;
	GLuint v3EyePosUniform;
};





