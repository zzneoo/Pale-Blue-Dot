#pragma once
#include "ShaderLoader.h"
class BlackBlendQuadShaderLoader : public ZzShaderLoader
{
public:
	BlackBlendQuadShaderLoader();
	~BlackBlendQuadShaderLoader();

	void LoadSampler2DTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadBlendFactorUniform(GLfloat factor);

private:

	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar* filepath_vs = "Resources/Shaders/BlackBlendQuad/BlackBlendQuad.vs";
	const GLchar* filepath_fs = "Resources/Shaders/BlackBlendQuad/BlackBlendQuad.fs";

	//Uniform Locations
	GLuint Sampler2DTexture_Uniform;
	GLuint fBlendFactorUniform;

};

