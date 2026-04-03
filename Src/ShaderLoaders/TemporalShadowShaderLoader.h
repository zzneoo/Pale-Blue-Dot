#pragma once
#include "ShaderLoader.h"
class TemporalShadowShaderLoader : public ZzShaderLoader
{
public:
	TemporalShadowShaderLoader();
	~TemporalShadowShaderLoader();

	//Uniform Samplers
	void LoadSampler2DCurrFrameNearest_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2DPrevFrame_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2DCurrVelocity_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	//Uniform floats
	void LoadfTime_Uniform(GLfloat time);
	//uniform matrices
	void Loadm4ProjX_Uniform(vmath::mat4 ProjX);
	void Loadm4PrevViewX_Uniform(vmath::mat4 PrevViewX);
	void Loadm4ViewX_Uniform(vmath::mat4 ViewX);
	//uniform vec3
	void Loadv3CameraPosition_Uniform(vmath::vec3 CameraPosition);
	//uniform int
	void LoadiBlockPixelID_Uniform(GLuint iBlockPixelID);


private:

	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar* filepath_vs = "Resources/Shaders/TemporalUpscale/TemporalShadowUpscale.vs";
	const GLchar* filepath_fs = "Resources/Shaders/TemporalUpscale/TemporalShadowUpscale.fs";

	//Uniform Locations
	GLuint Sampler2DCurrFrameNearest_Uniform;
	GLuint Sampler2DPrevFrame_Uniform;
	GLuint Sampler2DCurrVelocity_Uniform;

	GLuint m4ProjX_Uniform;
	GLuint m4PrevViewX_Uniform;
	GLuint m4ViewX_Uniform;
	GLuint v3CameraPosition_Uniform;

	GLuint fTime_Uniform;
	GLuint iBlockPixelID_Uniform;

};
