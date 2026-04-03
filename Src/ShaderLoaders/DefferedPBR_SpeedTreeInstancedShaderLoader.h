#pragma once
#include "ShaderLoader.h"
class DefferedPBR_SpeedTreeInstancedShaderLoader :public ZzShaderLoader
{
public:
	DefferedPBR_SpeedTreeInstancedShaderLoader();
	~DefferedPBR_SpeedTreeInstancedShaderLoader();

	//matrices
	void LoadModelViewProjXUniform(vmath::mat4 matrix);
	void LoadPrevViewProjXUniform(vmath::mat4 matrix);
	void LoadViewProjXUniform(vmath::mat4 matrix);

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
	const GLchar* filepath_vs = "Resources/Shaders/PBR/DefferedPBR_SpeedTreeInstanced.vs";
	const GLchar* filepath_fs = "Resources/Shaders/PBR/DefferedPBR_SpeedTreeInstanced.fs";

	//uniform locations
	GLuint m4ModelViewProjX_Uniform;
	GLuint m4PrevViewProjX_Uniform;
	GLuint m4ViewProjX_Uniform;

	//Samplers
	GLuint Sampler2D_PBR_AlbedoUniform;
	GLuint Sampler2D_PBR_NormalUniform;
	GLuint Sampler2D_PBR_SubsurfaceUniform;
	GLuint Sampler2D_PBR_BlueNoise;
};

