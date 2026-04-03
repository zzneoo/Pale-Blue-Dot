#pragma once
#include "ShaderLoader.h"
class TwoTextureQuadShaderLoader :	public ZzShaderLoader
{
public:
	TwoTextureQuadShaderLoader();
	~TwoTextureQuadShaderLoader();

	void LoadSampler2DTexture0_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2DTexture1_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2DBlueNoise_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadBlendFactor(GLfloat factor);
	void LoadDitherFactor(GLfloat factor);
	void LoadFrameID_Uniform(GLuint FrameID);

private:

	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar* filepath_vs = "Resources/Shaders/TwoTextureQuad/TwoTextureQuad.vs";
	const GLchar* filepath_fs = "Resources/Shaders/TwoTextureQuad/TwoTextureQuad.fs";

	//Uniform Locations
	GLuint Sampler2DTexture0_Uniform;
	GLuint Sampler2DTexture1_Uniform;
	GLuint Sampler2DBlueNoise_Uniform;

	GLuint fBlendFactor_Uniform;
	GLuint fDitherFactor_Uniform;
	GLuint iFrameID_Uniform;

};

