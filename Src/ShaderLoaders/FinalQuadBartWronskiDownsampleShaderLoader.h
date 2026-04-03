#pragma once
#include "ShaderLoader.h"
class FinalQuadBartWronskiDownsampleShaderLoader :	public ZzShaderLoader
{
public:
	FinalQuadBartWronskiDownsampleShaderLoader();
	~FinalQuadBartWronskiDownsampleShaderLoader();

	void LoadSampler2DTexture0_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

private:

	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar* filepath_vs = "Resources/Shaders/FinalQuad/FinalQuad.vs";
	const GLchar* filepath_fs = "Resources/Shaders/FinalQuad/FinalQuadBartWronskiDownsample.fs";

	//Uniform Locations
	GLuint Sampler2DTexture0_Uniform;

};

