#pragma once
#include "ShaderLoader.h"

class SunGlareShaderLoader :public ZzShaderLoader
{
public:
	SunGlareShaderLoader();
	~SunGlareShaderLoader();

	void LoadModelViewProjXUniform(vmath::mat4 matrix);
	void Loadv3ScaleUniform(vmath::vec3 scale);
	void LoadSamplerStockFlareUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadFactorPitchYawUniform(vmath::vec3 factorPitchYaw);

private:
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	const GLchar* filepath_vs = "Resources/Shaders/SunGlare/SunGlare.vs";
	const GLchar* filepath_fs = "Resources/Shaders/SunGlare/SunGlare.fs";

	//uniforms
	GLuint m4ModelViewProjXUniform;
	GLuint v3ScaleUniform;
	GLuint tSampler_StockFlareUniform;
	GLuint fFactorPitchYawUniform;

};



