#pragma once
#include "ShaderLoader.h"
class WaterHeightMapShaderLoader : public ZzShaderLoader
{
public:
	WaterHeightMapShaderLoader();
	~WaterHeightMapShaderLoader();

	void LoadSampler1D_permutationTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler3D_perlinWorleyTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadiTextureSize_Uniform(GLint iSize);
	void LoadfTime_Uniform(GLfloat time);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar* filepath_cs = "Resources/Shaders/WaterHeightMap/WaterHeightMap.compute";
	GLuint Sampler1D_permutationTextureUniform;
	GLuint Sampler3D_perlinWorleyTextureUniform;
	GLuint iTextureSize_Uniform;
	GLuint fTime_Uniform;
};



