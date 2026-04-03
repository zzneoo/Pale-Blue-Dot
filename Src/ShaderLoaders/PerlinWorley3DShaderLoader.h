#pragma once
#include "ShaderLoader.h"
class PerlinWorley3DShaderLoader :	public ZzShaderLoader
{
public:
	PerlinWorley3DShaderLoader();
	~PerlinWorley3DShaderLoader();

	void LoadSampler1D_permutationTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadiTextureSize_Uniform(GLint iSize);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar* filepath_cs = "Resources/Shaders/PerlinWorley3D/PerlinWorley3D.compute";
	GLuint Sampler1D_permutationTextureUniform;
	GLuint iTextureSize_Uniform;
};

