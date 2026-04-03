#pragma once
#include "ShaderLoader.h"
class PBR_SpeedTreeShaderLoader :public ZzShaderLoader
{
public:
	PBR_SpeedTreeShaderLoader();
	~PBR_SpeedTreeShaderLoader();

	//matrices
	void LoadModelViewProjXUniform(vmath::mat4 matrix);
	void LoadModelXUniform(vmath::mat4 matrix);

	//vectors
	void LoadSunPositionUniform(vmath::vec3 sunPos);
	void LoadEyePositionUniform(vmath::vec3 eyePos);
	void Loadv3LightColorUniform(vmath::vec3 lightColor);
	//Samplers
	void LoadAlbedoTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSubsurfaceTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadBlueNoiseTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Variables
	//shader file names
	const GLchar* filepath_vs = "Resources/Shaders/PBR/PBR_SpeedTree.vs";
	const GLchar* filepath_fs = "Resources/Shaders/PBR/PBR_SpeedTree.fs";

	//uniform locations
	GLuint m4ModelViewProjX_Uniform;
	GLuint m4ModelX_Uniform;
	//vectors
	GLuint v3SunPosUniform;
	GLuint v3EyePosUniform;
	GLuint v3LightColorUniform;
	//Samplers
	GLuint Sampler2D_PBR_AlbedoUniform;
	GLuint Sampler2D_PBR_NormalUniform;
	GLuint Sampler2D_PBR_SubsurfaceUniform;
	GLuint Sampler2D_PBR_BlueNoise;
};

