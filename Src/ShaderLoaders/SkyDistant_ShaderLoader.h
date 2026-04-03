#pragma once
#include "ShaderLoader.h"
class SkyDistant_ShaderLoader :public ZzShaderLoader
{
public:
	SkyDistant_ShaderLoader(GLuint AtmosphereFragShaderID);
	~SkyDistant_ShaderLoader();

	//floats
	void LoadArtistFactor_Uniform(GLfloat factor);
	void LoadExposure_Uniform(GLfloat Exposure);

	//vectors
	void LoadSunPosition_Uniform(vmath::vec3 sunPos);
	void LoadSunSize_Uniform(vmath::vec2 sunSize);
	void LoadNearFar_Uniform(vmath::vec2 NearFar);
	void LoadEyePosition_Uniform(vmath::vec3 eyePos);
	void LoadEarthCenter_Uniform(vmath::vec3 earthCenter);
	//Matrices
	void LoadInvViewX_Uniform(vmath::mat4 matrix);
	void LoadInvProjX_Uniform(vmath::mat4 matrix);

	//Post Process
	void LoadBlueNoiseTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);


private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;
	void SetAtmosphereFragShaderObject(GLuint AtmoshpereShaderID);

	//Variables
	//shader file names
	const GLchar* filepath_vs = "Resources/Shaders/SkyDistant/SkyDistant.vs";
	const GLchar* filepath_fs = "Resources/Shaders/SkyDistant/SkyDistant.fs";

	//uniform locations
	//floats
	GLuint fArtistFactor_Uniform;
	GLuint fExposure_Uniform;
	//vec2
	GLuint v2NearFar_Uniform;
	//Matrices
	GLuint m4InvViewX_Uniform;
	GLuint m4InvProjX_Uniform;
	//vectors
	GLuint v2SunSize_Uniform;
	GLuint v3SunPos_Uniform;
	GLuint v3EyePos_Uniform;
	GLuint v3EarthCenter_Uniform;

	//PostProcess
	GLuint Sampler2D_BlueNoiseTexture_Uniform;

};

