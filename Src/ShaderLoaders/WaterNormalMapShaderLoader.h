#pragma once
#include "ShaderLoader.h"
class WaterNormalMapShaderLoader : public ZzShaderLoader
{
public:
	WaterNormalMapShaderLoader();
	~WaterNormalMapShaderLoader();

	void LoadSampler2D_WaterHeightMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadiTextureSize_Uniform(GLint iSize);	

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar* filepath_cs = "Resources/Shaders/WaterNormalMap/WaterNormalMap.compute";

	GLuint Sampler2D_WaterHeightMapTextureUniform;
	GLuint iTextureSize_Uniform;
};



