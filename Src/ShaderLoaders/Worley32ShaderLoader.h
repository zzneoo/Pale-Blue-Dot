#pragma once
#include "ShaderLoader.h"
//#include"MyOpenGL.h"

class Worley32ShaderLoader : public ZzShaderLoader
{
public:
	Worley32ShaderLoader();
	~Worley32ShaderLoader();

	void LoadSampler1D_permutationTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadiTextureSize_Uniform(GLint iSize);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar* filepath_cs = "Resources/Shaders/Worley32/Worley32.compute";
	GLuint Sampler1D_permutationTextureUniform;
	GLuint iTextureSize_Uniform;
};