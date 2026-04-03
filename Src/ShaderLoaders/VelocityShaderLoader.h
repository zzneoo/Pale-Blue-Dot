#pragma once
#include "ShaderLoader.h"
class VelocityShaderLoader : public ZzShaderLoader
{
public:
	VelocityShaderLoader();
	~VelocityShaderLoader();

	//Uniform Samplers
	void LoadSampler2DCurrDepth_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	//vec2
	void Loadv2Halton23_Uniform(vmath::vec2 Halton23);
	void Loadv2NearFar_Uniform(vmath::vec2 NearFar);
	//vec3
	void Loadv3EyePos_Uniform(vmath::vec3 EyePos);
	//uniform matrices
	void Loadm4ProjX_Uniform(vmath::mat4 ProjX);
	void Loadm4InvProjX_Uniform(vmath::mat4 InvProjX);
	void Loadm4PrevViewX_Uniform(vmath::mat4 PrevViewX);
	void Loadm4InvViewX_Uniform(vmath::mat4 InvViewX);

private:

	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
//	const GLchar* filepath_cs = "Resources/Shaders/Velocity/Velocity.compute";
	const GLchar* filepath_vs = "Resources/Shaders/Velocity/Velocity.vs";
	const GLchar* filepath_fs = "Resources/Shaders/Velocity/Velocity.fs";

	//currDepth
	GLuint Sampler2DCurrDepth_Uniform;

	//vec2
	GLuint v2Halton23_Uniform;
	GLuint v2NearFar_Uniform;

	//vec3
	GLuint v3EyePos_Uniform;

	//matrices
	GLuint m4ProjX_Uniform;
	GLuint m4InvProjX_Uniform;
	GLuint m4PrevViewX_Uniform;
	GLuint m4InvViewX_Uniform;

};
