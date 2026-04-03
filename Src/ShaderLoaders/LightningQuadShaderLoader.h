#pragma once
#include "ShaderLoader.h"
class LightningQuadShaderLoader : public ZzShaderLoader
{
public:
	LightningQuadShaderLoader();
	~LightningQuadShaderLoader();

	void LoadSampler2DTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadBlendFactorUniform(GLfloat factor);

private:

	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar* filepath_vs = "Resources/Shaders/LightningQuad/LightningQuad.vs";
	const GLchar* filepath_fs = "Resources/Shaders/LightningQuad/LightningQuad.fs";

	//Uniform Locations
	GLuint Sampler2DTexture_Uniform;
	GLuint fBlendFactorUniform;

};

