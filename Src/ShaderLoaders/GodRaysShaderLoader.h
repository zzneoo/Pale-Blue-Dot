#pragma once
#include "ShaderLoader.h"
class GodRaysShaderLoader :public ZzShaderLoader
{
public:
	GodRaysShaderLoader();
	~GodRaysShaderLoader();

	//samplers
	void LoadSampler2D_LightAreaUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_BlueNoiseUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	//vectors
	void Loadv2Resolution_Uniform(vmath::vec2 resolution);
	void Loadv2ScreenSunPos_Uniform(vmath::vec2 ScreenSunPos);
	//float
	void LoadfTime_Uniform(GLfloat fTime);
	void LoadiFrameID_Uniform(GLuint iFrameID);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//variables
	const GLchar* filepath_cs = "Resources/Shaders/RadialBlur/RadialBlur.compute";

	GLuint Sampler2D_LightAreaUniform;
	GLuint Sampler2D_BlueNoiseUniform;
	GLuint v2Resolution_Uniform;
	GLuint v2ScreenSunPos_Uniform;
	GLuint fTime_Uniform;
	GLuint iFrameID_Uniform;
};

