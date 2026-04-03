#pragma once
#include "ShaderLoader.h"
class CompositorSpaceShaderLoader :	public ZzShaderLoader
{
public:
	CompositorSpaceShaderLoader();
	~CompositorSpaceShaderLoader();

	void LoadSampler2D_Deffered_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_Clouds_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_FarClouds_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_Background_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_Flare_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_Extra_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void Loadv2InvResolution_Uniform(vmath::vec2 InvRes);
	void LoadfMixSpaceGround_Uniform(GLfloat factor);

private:

	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar* filepath_vs = "Resources/Shaders/Compositor/CompositorSpace.vs";
	const GLchar* filepath_fs = "Resources/Shaders/Compositor/CompositorSpace.fs";

	//Uniform Locations
	GLuint Sampler2D_Deffered_Uniform;
	GLuint Sampler2D_Clouds_Uniform;
	GLuint Sampler2D_FarClouds_Uniform;
	GLuint Sampler2D_Background_Uniform;
	GLuint Sampler2D_Flare_Uniform;
	GLuint Sampler2D_Extra_Uniform;
	GLuint v2InvResolution_Uniform;
	GLuint fMixSpaceGround_Uniform;
};

