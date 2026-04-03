#pragma once
#include "ShaderLoader.h"
class VelocityMaxShaderLoader : public ZzShaderLoader
{
public:
	VelocityMaxShaderLoader();
	~VelocityMaxShaderLoader();

	//Uniform Samplers
	void LoadSampler2DCurrVelocity_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

private:

	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar* filepath_cs = "Resources/Shaders/Velocity/VelocityMax.compute";

	//currDepth
	GLuint Sampler2DCurrVelocity_Uniform;

};
