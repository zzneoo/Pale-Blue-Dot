#pragma once
#include "ShaderLoader.h"
class PBR_AnimatedShadowShaderLoader :public ZzShaderLoader
{
public:
	PBR_AnimatedShadowShaderLoader();
	~PBR_AnimatedShadowShaderLoader();

	//matrices
	void Loadm4JointTransformXUniform(vmath::mat4* matricesPointer, GLsizei count);
	void LoadModelXUniform(vmath::mat4 matrix);
	void LoadViewXUniform(vmath::mat4 matrix);
	void LoadProjXUniform(vmath::mat4 matrix);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Variables
	//shader file names
	const GLchar* filepath_vs = "Resources/Shaders/PBR/PBR_AnimatedShadow.vs";
	const GLchar* filepath_fs = "Resources/Shaders/PBR/PBR_AnimatedShadow.fs";

	//uniform locations
	GLuint m4ModelX_Uniform;
	GLuint m4ViewX_Uniform;
	GLuint m4ProjX_Uniform;

	GLuint m4JointTransformX_Uniform;

};

