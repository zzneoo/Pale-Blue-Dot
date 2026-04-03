#pragma once
#include "ShaderLoader.h"
class LightAreaMaskShaderLoader :public ZzShaderLoader
{
public:
	LightAreaMaskShaderLoader();
	~LightAreaMaskShaderLoader();

	void LoadSampler2DSceneMask_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2DCloudMask_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

private:

	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar* filepath_vs = "Resources/Shaders/LightAreaMask/LightAreaMask.vs";
	const GLchar* filepath_fs = "Resources/Shaders/LightAreaMask/LightAreaMask.fs";

	//Uniform Locations
	GLuint Sampler2DSceneMask_Uniform;
	GLuint Sampler2DCloudMask_Uniform;
};

