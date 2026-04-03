#pragma once
#include "ShaderLoader.h"
class GroundShaderLoader :	public ZzShaderLoader
{
public:
	GroundShaderLoader();
	~GroundShaderLoader();

	//matrices
	void LoadfGroundFactorUniform(GLfloat groundFactor);
	void LoadfTimeUniform(GLfloat time);

	void LoadModelXUniform(vmath::mat4 matrix);
	void LoadViewXUniform(vmath::mat4 matrix);
	void LoadProjXUniform(vmath::mat4 matrix);
	void LoadModelViewProjXUniform(vmath::mat4 matrix);
	void Loadm4LightSpaceXUniform(vmath::mat4 matrix);

	//ground
	void LoadTerrainGroundTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainGroundNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainGroundAOTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainGroundRoughnessTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadTerrainGroundDisplacementTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadShadowMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	void Loadv3SunPosUniform(vmath::vec3 sunPos);
	void Loadv3EyePosUniform(vmath::vec3 eyePos);
	void Loadv3LightColorUniform(vmath::vec3 LightColor);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar * filepath_vs = "Resources/Shaders/Ground/Ground.vs";
	const GLchar * filepath_tcs = "Resources/Shaders/Ground/Ground.tcs";
	const GLchar * filepath_tes = "Resources/Shaders/Ground/Ground.tes";
	const GLchar * filepath_fs = "Resources/Shaders/Ground/Ground.fs";

	//uniforms
	GLuint m4ModelX_Uniform;
	GLuint m4ViewX_Uniform;
	GLuint m4ProjX_Uniform;
	GLuint m4ModelViewProjX_Uniform;

	GLuint m4LightSpaceX_Uniform;

	GLuint v3SunPos_Uniform;
	GLuint v3EyePos_Uniform;
	GLuint v3LightColor_Uniform;

	GLuint fGroundFactorUniform;
	GLuint fTimeUniform;

	//grass
	GLuint Sampler2D_GroundAlbedoUniform;
	GLuint Sampler2D_GroundAO_Uniform;
	GLuint Sampler2D_GroundNormalUniform;
	GLuint Sampler2D_GroundRoughnessUniform;
	GLuint Sampler2D_GroundDisplacementUniform;
	GLuint Sampler2D_ShadowMapUniform;

};

