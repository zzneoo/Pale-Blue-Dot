#pragma once
#include "ShaderLoader.h"
class CloudShaderLoader :	public ZzShaderLoader
{
public:
	CloudShaderLoader(GLuint AtmoShader);
	~CloudShaderLoader();

	void LoadSampler3D_PerlinWorleyUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler3D_Worley32Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_WeatherMapUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_WeatherMapTilableUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_BlueNoiseUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_CurlNoiseUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_BeerShadowMapUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	void LoadSampler2D_CirrusCloudUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_CirroStratusCloudUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	void LoadSampler2D_CirroCumulusCloudUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	void Loadm4InvViewX_Uniform(vmath::mat4 ViewMatrix);
	void Loadm4LightViewProjX_Uniform(vmath::mat4 LightViewProjMatrix);
	void Loadm4PrevViewX_Uniform(vmath::mat4 PrevViewMatrix);
	void Loadm4InvProjX_Uniform(vmath::mat4 ProjMatrix);
	void Loadm4ProjX_Uniform(vmath::mat4 ProjMatrix);

	void Loadv3CameraPos_Uniform(vmath::vec3 CameraPos);
	void Loadv3SunPos_Uniform(vmath::vec3 SunPos);
	void Loadv3SunColor_Uniform(vmath::vec3 SunColor);

	void Loadv2Resolution_Uniform(vmath::vec2 Resolution);
	void Loadv2InnerOuterRadius_Uniform(vmath::vec2 InnerOuterRadius);

	void LoadfTime_Uniform(GLfloat fTime);
	void LoadfCloudFactor_Uniform(GLfloat fCloudScale);
	void LoadfEarthRadius_Uniform(GLfloat EarthRadius);
	void LoadfCloudVolumetricCoverage_Uniform(GLfloat fCloudVolumetricCoverage);
	void LoadfCloudHighAltitudeCoverage_Uniform(GLfloat fCloudHighAltitudeCoverage);

	void LoadiPixelID_Uniform(GLint PixelID);
	void LoadiFrameID_Uniform(GLuint iFrameID);

private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;
	void SetAtmosphereShaderObject(GLuint AtmoshpereShaderID);

	//variables
	const GLchar* filepath_cs = "Resources/Shaders/Clouds/Clouds.Compute";

	//samplers
	GLuint Sampler3D_PerlinWorley3DUniform;
	GLuint Sampler3D_Worley32Uniform;
	GLuint Sampler2D_WeatherMapUniform;
	GLuint Sampler2D_WeatherMapTilableUniform;
	GLuint Sampler2D_BlueNoiseUniform;
	GLuint Sampler2D_CurlNoiseUniform;
	GLuint Sampler2D_BeerShadowMapUniform;

	GLuint Sampler2D_CirrusCloudUniform;
	GLuint Sampler2D_CirroStratusCloudUniform;
	GLuint Sampler2D_CirroCumulusCloudUniform;
	//matrices
	GLuint m4InvViewX_Uniform;
	GLuint m4LightViewProjX_Uniform;
	GLuint m4PrevViewX_Uniform;
	GLuint m4InvProjX_Uniform;
	GLuint m4ProjX_Uniform;

	//vectors
	GLuint v3CameraPos_Uniform;
	GLuint v3SunPos_Uniform;
	GLuint v3SunColor_Uniform;

	GLuint v2Resolution_Uniform;
	GLuint v2InnerOuterRadius_Uniform;

	//floats
	GLuint fTime_Uniform;
	GLuint fCloudFactor_Uniform;//for GUI
	GLuint fEarthRadius_Uniform;
	GLuint fCloudVolumetricCoverage_Uniform;
	GLuint fCloudHighAltitudeCoverage_Uniform;

	//ints
	GLuint iPixelID_Uniform;
	GLuint iFrameID_Uniform;
};

