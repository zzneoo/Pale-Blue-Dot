#pragma once
#include "ShaderLoader.h"

class Axis3D_ShaderLoader :public ZzShaderLoader
{
public:
	Axis3D_ShaderLoader();
	~Axis3D_ShaderLoader();

	void LoadModelViewXUniform(vmath::mat4 matrix);
	void LoadProjXUniform(vmath::mat4 matrix);

	void Loadv3ScaleUniform(vmath::vec3 scale);
	void Loadv3ColorUniform(vmath::vec3 color);
	void LoadfMaxEyeDistanceUniform(GLfloat FarMinusNear);

private:
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	const GLchar* filepath_vs = "Resources/Shaders/Axis3D/Axis3D.vs";
	const GLchar* filepath_fs = "Resources/Shaders/Axis3D/Axis3D.fs";

	//uniforms
	GLuint m4ModelViewXUniform;
	GLuint m4ProjXUniform;

	GLuint v3ScaleUniform;
	GLuint fMaxEyeDistanceUniform;
	GLuint v3ColorUniform;

};



