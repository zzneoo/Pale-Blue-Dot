#pragma once
#include "ShaderLoader.h"
class SkyDomeShaderLoader :	public ZzShaderLoader
{
public:
	SkyDomeShaderLoader();
	~SkyDomeShaderLoader();

	bool Initialize(void) override;

	void LoadModelViewProjXUniform(vmath::mat4 matrix);
	void LoadCameraPosUniform(vmath::vec3 cameraPos);
	void LoadLightPosUniform(vmath::vec3 lightPos);
	void LoadiFrameID_Uniform(GLuint iFrameID);
	void LoadfSkyFactor_Uniform(GLfloat fSkyFactor);

	void LoadInverseWavelengthUniform(vmath::vec3 inverseWavelength);
	void LoadOuterRadiusUniform(GLfloat OuterRadius);
	void LoadInnerRadiusUniform(GLfloat InnerRadius);

	void LoadScaleUniform(GLfloat Scale);
	void LoadScaleDepthUniform(GLfloat ScaleDepth);
	void LoadScaleOverScaleDepthUniform(GLfloat ScaleOverScaleDepth);

	void LoadKrESunUniform(GLfloat KrESun);
	void LoadKmESunUniform(GLfloat KmESun);

	void LoadKr4PIUniform(GLfloat Kr4PI);
	void LoadKm4PIUniform(GLfloat Km4PI);
	//tSampler
	void LoadTextureID_BlueNoiseUniform(GLenum TEXTURE, GLuint tNo, GLuint  TextureID_BlueNoise);

private:

	void GetAllUniformLocations(void) override;

	const GLchar * filepath_vs = "Resources/Shaders/SkyDome/SkyDome.vs";
	const GLchar * filepath_fs = "Resources/Shaders/SkyDome/SkyDome.fs";

	//Varying Scattering Uniforms
	GLuint v3LightPos_Uniform;
	GLuint m4ModelViewProjX_Uniform;
	GLuint v3CameraPos_Uniform;
	GLuint iFrameID_Uniform;
	GLuint fSkyFactor_Uniform;

	//const uniforms
	GLuint fOuterRadius_Uniform;
	GLuint fInnerRadius_Uniform;

	GLuint fScale_Uniform;
	GLuint fScaleDepth_Uniform;
	GLuint fScaleOverScaleDepth_Uniform;

	GLuint fKrESun_Uniform;
	GLuint fKmESun_Uniform;

	GLuint fKr4PI_Uniform;
	GLuint fKm4PI_Uniform;
	GLuint v3InvWavelength_Uniform;

	//sampler uniform
	GLuint tSamplerBlueNoise;

};

