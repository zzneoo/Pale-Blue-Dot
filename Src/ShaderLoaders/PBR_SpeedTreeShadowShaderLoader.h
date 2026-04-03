#pragma once
#include "ShaderLoader.h"
class PBR_SpeedTreeShadowShaderLoader :public ZzShaderLoader
{
public:
	PBR_SpeedTreeShadowShaderLoader();
	~PBR_SpeedTreeShadowShaderLoader();

	//matrices
	void LoadModelViewProjXUniform(vmath::mat4 matrix);
	//Samplers
	void LoadAlbedoTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Variables
	//shader file names
	const GLchar* filepath_vs = "Resources/Shaders/PBR/PBR_SpeedTreeShadow.vs";
	const GLchar* filepath_fs = "Resources/Shaders/PBR/PBR_SpeedTreeShadow.fs";

	//uniform locations
	GLuint m4ModelViewProjX_Uniform;
	//Samplers
	GLuint Sampler2D_PBR_AlbedoUniform;

};

