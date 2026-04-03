#pragma once
#include "ShaderLoader.h"
class WeatherMapShaderLoader : public ZzShaderLoader
{
public:
	WeatherMapShaderLoader();
	~WeatherMapShaderLoader();

	void LoadSampler1D_permutationTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadiTextureSize_Uniform(GLint iSize);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar* filepath_cs = "Resources/Shaders/WeatherMap/WeatherMap.compute";
	GLuint Sampler1D_permutationTextureUniform;
	GLuint iTextureSize_Uniform;
};

