#pragma once
#include "ShaderLoader.h"
class TemporalAntiAliasingShaderLoader : public ZzShaderLoader
{
public:
	TemporalAntiAliasingShaderLoader();
	~TemporalAntiAliasingShaderLoader();

	//Uniform Samplers
	void LoadSampler2DCurrFrameNearest_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
//	void LoadSampler2DCurrFrameLinear_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2DPrevFrame_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2DCurrVelocity_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2DCurrDepth_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
//	void LoadSampler2DPrevDepth_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	//Uniform floats
	void LoadfTime_Uniform(GLfloat time);
	void LoadbIsTAA_Uniform(bool isTAA);
	void Loadv4Halton23_Uniform(vmath::vec4 CurrPrevHalton23);
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
	const GLchar* filepath_vs = "Resources/Shaders/TemporalAntiAliasing/TemporalAntiAliasing.vs";
	const GLchar* filepath_fs = "Resources/Shaders/TemporalAntiAliasing/TemporalAntiAliasing.fs";

	//Uniform Locations
	GLuint Sampler2DCurrFrameNearest_Uniform;
//	GLuint Sampler2DCurrFrameLinear_Uniform;
	GLuint Sampler2DPrevFrame_Uniform;
//	GLuint Sampler2DPrevDepth_Uniform;
	GLuint Sampler2DCurrVelocity_Uniform;
	GLuint Sampler2DCurrDepth_Uniform;

	GLuint m4ProjX_Uniform;
	GLuint m4PrevViewX_Uniform;
	GLuint m4ViewX_Uniform;
	GLuint v3CameraPosition_Uniform;

	GLuint fTime_Uniform;
	GLuint v4Halton23_Uniform;
	GLuint iBlockPixelID_Uniform;
	GLuint bIsTAA_Uniform;

};
