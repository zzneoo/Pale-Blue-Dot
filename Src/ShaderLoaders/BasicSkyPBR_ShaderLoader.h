#pragma once
#include "ShaderLoader.h"
class BasicSkyPBR_ShaderLoader :public ZzShaderLoader
{
public:
	BasicSkyPBR_ShaderLoader(GLuint AtmosphereFragShaderID);
	~BasicSkyPBR_ShaderLoader();

	//floats
	void LoadArtistFactor_Uniform(GLfloat factor);
	void LoadExposure_Uniform(GLfloat Exposure);
	void LoadTime_Uniform(GLfloat fTime);

	//vectors
	void LoadSunPosition_Uniform(vmath::vec3 sunPos);
	void LoadSunSize_Uniform(vmath::vec2 sunSize);
	void LoadColorPicker_Uniform(vmath::vec3 ColorPicker);
	void LoadEyePosition_Uniform(vmath::vec3 eyePos);
	void LoadEarthCenter_Uniform(vmath::vec3 earthCenter);
	//Matrices
	void LoadInvViewX_Uniform(vmath::mat4 matrix);
	void LoadInvProjX_Uniform(vmath::mat4 matrix);
	void Loadm4LightViewProjX_Uniform(vmath::mat4 matrix);
	void LoadPrevViewProjX_Uniform(vmath::mat4 matrix);

	//Samplers
	void LoadWorldSpacePositionTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadWorldSpaceNormalTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadAlbedoAOTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSpecRoughAlphaDitherTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadDefferedVelocityTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	//Post Process
	void LoadCloudsTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadBlueNoiseTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadCloudShadowMap_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadFroxelTransmittance_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadFroxelInScatter_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;
	void SetAtmosphereFragShaderObject(GLuint AtmoshpereShaderID);

	//Variables
	//shader file names
	const GLchar* filepath_vs = "Resources/Shaders/PBR/BasicSkyPBR.vs";
	const GLchar* filepath_fs = "Resources/Shaders/PBR/BasicSkyPBR.fs";

	//uniform locations
	//floats
	GLuint fArtistFactor_Uniform;
	GLuint fExposure_Uniform;
	GLuint fTime_Uniform;
	//Matrices
	GLuint m4InvViewX_Uniform;
	GLuint m4InvProjX_Uniform;
	GLuint m4PrevViewProjX_Uniform;
	GLuint m4LightViewProjX_Uniform;
	//vectors
	GLuint v2SunSize_Uniform;
	GLuint v3SunPos_Uniform;
	GLuint v3ColorPicker_Uniform;
	GLuint v3EyePos_Uniform;
	GLuint v3EarthCenter_Uniform;
	//Samplers PBR
	GLuint Sampler2D_WorldSpacePosition;
	GLuint Sampler2D_WorldSpaceNormal;
	GLuint Sampler2D_AlbedoAO;
	GLuint Sampler2D_SpecRoughAlphaDither;
	GLuint Sampler2D_DefferedVelocity;

	//PostProcess
	GLuint Sampler2D_CloudShadowMap;
	GLuint Sampler2D_CloudsTexture_Uniform;
	GLuint Sampler3D_FroxelTransmittance_Uniform;
	GLuint Sampler3D_FroxelInScatter_Uniform;
	GLuint Sampler2D_BlueNoiseTexture_Uniform;

};

