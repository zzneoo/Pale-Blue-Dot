#pragma once
#include "ShaderLoader.h"
class SkyPBR_ShaderLoader:public ZzShaderLoader
{
public:
	SkyPBR_ShaderLoader(GLuint AtmosphereFragShaderID);
	~SkyPBR_ShaderLoader();

	//floats
	void LoadFactorUniform(GLfloat factor);
	void LoadTimeUniform(GLfloat fTime);
	void LoadExposureUniform(GLfloat Exposure);

	//vectors
	void LoadSunPositionUniform(vmath::vec3 sunPos);
	void LoadCloudScaleVisibilityUniform(vmath::vec2 CloudScale_Visibility);
	void LoadSunSizeUniform(vmath::vec2 sunSize);
	void LoadColorPickerUniform(vmath::vec3 ColorPicker);
	void LoadWaterTintUniform(vmath::vec3 WaterTint);
	void LoadEyePositionUniform(vmath::vec3 eyePos);
	void LoadEarthCenterUniform(vmath::vec3 earthCenter);
	//Matrices
	void LoadInvViewXUniform(vmath::mat4 matrix);
	void LoadInvProjXUniform(vmath::mat4 matrix);
	void Loadm4LightViewProjXUniform(vmath::mat4 matrix);
	void LoadPrevViewProjXUniform(vmath::mat4 matrix);
	void LoadEarthRotationXUniform(vmath::mat4 matrix);
	void LoadEarthCloudRotationXUniform(vmath::mat4 matrix);

	//Samplers
	void LoadWorldSpacePositionTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadWorldSpaceNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadAlbedoAOTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSpecRoughAlphaDitherTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadDefferedVelocityTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	void LoadEarthAlbedoSpecTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadEarthCloudsTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadEarthNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	void LoadEarthLodBlendMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadEarthWaterNormalUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	//Post Process
	void LoadSunGlareTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadCloudsTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadWhirlNoiseTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadWhirlTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadBlueNoiseTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadCloudShadowMapUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;
	void SetAtmosphereFragShaderObject(GLuint AtmoshpereShaderID);

	//Variables
	//shader file names
	const GLchar* filepath_vs = "Resources/Shaders/PBR/SkyPBR.vs";
	const GLchar* filepath_fs = "Resources/Shaders/PBR/SkyPBR.fs";

	//uniform locations
	//floats
	GLuint fFactorUniform;
	GLuint fTimeUniform;
	GLuint fExposureUniform;
	//Matrices
	GLuint m4InvViewX_Uniform;
	GLuint m4InvProjX_Uniform;
	GLuint m4PrevViewProjX_Uniform;
	GLuint m4LightViewProjX_Uniform;
	GLuint m4EarthRotationX_Uniform;
	GLuint m4EarthCloudRotationX_Uniform;
	//vectors
	GLuint v2SunSizeUniform;
	GLuint v2CloudScaleVisibilityUniform;
	GLuint v3SunPosUniform;
	GLuint v3ColorPickerUniform;
	GLuint v3WaterTintUniform;
	GLuint v3EyePosUniform;
	GLuint v3EarthCenterUniform;
	//Samplers PBR
	GLuint Sampler2D_WorldSpacePosition;
	GLuint Sampler2D_WorldSpaceNormal;
	GLuint Sampler2D_AlbedoAO;
	GLuint Sampler2D_SpecRoughAlphaDither;
	GLuint Sampler2D_DefferedVelocity;

	GLuint Sampler2D_EarthAlbedoSpec;
	GLuint Sampler2D_EarthClouds;
	GLuint Sampler2D_EarthNormal;

	GLuint Sampler2D_EarthLodBlendMap;
	GLuint Sampler2D_EarthWaterNormalMap;

	GLuint Sampler2D_CloudShadowMap;
	//PostProcess
	GLuint Sampler2D_CloudsTextureUniform;
	GLuint Sampler2D_SunGlareTextureUniform;
	GLuint Sampler2D_WhirlNoiseTextureUniform;
	GLuint Sampler2D_WhirlTextureUniform;
	GLuint Sampler2D_BlueNoiseTextureUniform;

};

