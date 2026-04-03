#pragma once
#include "ShaderLoader.h"
class TemporalUpscale4kShaderLoader : public ZzShaderLoader
{
public:
	TemporalUpscale4kShaderLoader();
	~TemporalUpscale4kShaderLoader();

	//Uniform Samplers
	//void LoadSampler2DCurrFrameNearest_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2DCurrFrameLinear_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2DPrevFrame_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2DCurrVelocity_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	//uniform int
	void LoadiBlockPixelID_Uniform(GLuint iBlockPixelID);
	void LoadiBlockWidth_Uniform(GLuint iBlockWidth);
	void Loadv2Resolution_Uniform(vmath::vec2 v2Resolution);


private:

	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar* filepath_vs = "Resources/Shaders/TemporalUpscale4k/TemporalUpscale4k.vs";
	const GLchar* filepath_fs = "Resources/Shaders/TemporalUpscale4k/TemporalUpscale4k.fs";
	const GLchar* filepath_cs = "Resources/Shaders/TemporalUpscale4k/TemporalUpscale4k.compute";

	//Uniform Locations
//	GLuint Sampler2DCurrFrameNearest_Uniform;
	GLuint Sampler2DCurrFrameLinear_Uniform;
	GLuint Sampler2DPrevFrame_Uniform;
	//GLuint Sampler2DPrevDepth_Uniform;
	GLuint Sampler2DCurrVelocity_Uniform;
	//GLuint Sampler2DCurrDepth_Uniform;

	//GLuint m4ProjX_Uniform;
	//GLuint m4PrevViewX_Uniform;
	//GLuint m4ViewX_Uniform;
	//GLuint v3CameraPosition_Uniform;

	//GLuint fTime_Uniform;
	GLuint iBlockPixelID_Uniform;
	GLuint iBlockWidth_Uniform;
	GLuint v2Resolution_Uniform;

};
