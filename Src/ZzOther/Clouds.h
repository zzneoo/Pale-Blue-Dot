#pragma once
#include"stdafx.h"
#include"FrameBuffer.h"
#include"CloudShaderLoader.h"
#include"CloudGroundShaderLoader.h"
#include"CloudSpaceShaderLoader.h"
#include"CloudShadowMapShaderLoader.h"
#include"TemporalPassShaderLoader.h"
#include"TemporalShadowShaderLoader.h"

class Clouds
{
public:
	Clouds(GLuint AtmoShader, vmath::uvec3 atmoTextures);
	~Clouds();

	GLuint GetCloudTemporalUpscaledTexture(void)
	{
		return Fbo_TemporalUpscale->GetFbo_Color_Texture(0);
	}

	GLuint GetCloudShadowMapTexture(void)
	{
		return CloudShadowMapTexture;
	}

	GLuint GetCloudWeatherMapTexture(void)
	{
		return TextureID_WeatherMap;
	}

	GLuint GetCloudWeatherMapTileableTexture(void)
	{
		return TextureID_WeatherMapTileable;
	}

	GLuint GetCloudPrevShadowMapTexture(void)
	{
		return CloudPrevShadowMapTexture;
	}
	GLuint GetCloudTemporalUpscaledFrameExtra(void)
	{
		return Fbo_TemporalUpscale->GetFbo_Color_Texture(1);
	}

	GLfloat* GetfCloudFactor(void)
	{
		return &fCloudFactor;
	}

	vmath::vec3* Getv3SunColor(void)
	{
		return &v3SunColor;
	}

	//lightColor
	GLdouble clamp(GLdouble factor, GLdouble minimum, GLdouble maximum)
	{
#pragma push_macro("min")
#undef min

#pragma push_macro("max")
#undef max
		return vmath::min(vmath::max(factor, minimum), maximum);
#pragma pop_macro("min")
#pragma pop_macro("max")
	}

	GLdouble smoothstep(GLdouble edge0, GLdouble edge1, GLdouble factor)
	{
		GLdouble t = clamp((factor - edge0) / (edge1 - edge0), 0.0, 1.0);
		return t * t * (3.0 - 2.0 * t);
	}
	vmath::vec3 getLightColor(GLdouble sunTheta);

	void ResizeFramebuffers(GLsizei Width, GLsizei Height);

	void RenderComputeCloudShadowMap(GLfloat fEarthRadius, vmath::mat4 CameraViewMatrix, vmath::vec3 CameraPos, GLuint TextureID_PerlinWorley);
	void RenderComputeClouds(GLuint TextureID_PerlinWorley, GLuint TextureID_Worley32, GLuint TextureID_WeatherMap, GLuint TextureID_BlueNoise, vmath::mat4 CameraViewMatrix, vmath::vec3 CameraPos, vmath::vec3 SunPos, GLfloat VolumetricCoverage, GLfloat HighAltitudeCoverage, GLfloat timeMultiplier,GLfloat EarthRadius);
	void RenderComputeCloudsGround(GLuint TextureID_PerlinWorley, GLuint TextureID_Worley32, GLuint TextureID_WeatherMap, GLuint TextureID_BlueNoise, vmath::mat4 CameraViewMatrix, vmath::vec3 CameraPos, vmath::vec3 SunPos, GLfloat VolumetricCoverage, GLfloat HighAltitudeCoverage, GLfloat timeMultiplier, GLfloat EarthRadius);
	void RenderComputeCloudsSpace(GLuint TextureID_PerlinWorley, GLuint TextureID_Worley32, GLuint TextureID_WeatherMap, GLuint TextureID_BlueNoise, vmath::mat4 CameraViewMatrix, vmath::vec3 CameraPos, vmath::vec3 SunPos, GLfloat VolumetricCoverage, GLfloat HighAltitudeCoverage, GLfloat timeMultiplier, GLfloat EarthRadius);
	void RenderTemporalUpscaleQuad(vmath::mat4 ViewMatrix, vmath::vec3 CameraPos);
	void RenderTemporalUpscaleShadowMapQuad(vmath::mat4 ViewMatrix, vmath::vec3 CameraPos);
	void ResolveToPreviousFrame(void);

	GLuint GetCloudCurrFrameNearestTexture(void)
	{
		return CloudCurrFrameNearestTexture;
	}

	GLuint GetCloudCurrFrameLinearTexture(void)
	{
		return CloudCurrFrameLinearTexture;
	}

	GLuint GetCloudCurrFrameFarCloudsTexture(void)
	{
		return CloudFarCurrFrameTexture;
	}

private:
	//functions
	void Initialize(void);
	void InitializeAll(void);
	void SetProgramUniformsAtmoTextures(GLuint program, GLuint transmittance_texture_unit, GLuint scattering_texture_unit, GLuint irradiance_texture_unit, vmath::uvec3 atmoTextures);
	//class members

	//shaderloaders
	CloudShaderLoader *cloudShader;
	CloudGroundShaderLoader *cloudGroundShader;
	CloudSpaceShaderLoader *cloudSpaceShader;
	CloudShadowMapShaderLoader cloudShadowMapShader;
	TemporalPassShaderLoader temporalPassShader;
	TemporalShadowShaderLoader temporalShadowShader;

	//Clouds buffer textures
	GLuint CloudPreviousFrameTexture;
	GLuint CloudFarPreviousFrameTexture;
	GLuint CloudCurrFrameNearestTexture;
	GLuint CloudFarCurrFrameTexture;
	GLuint CloudCurrFrameLinearTexture;
	GLuint CloudVelocityTexture;
	//GLuint CloudDepthTexture;
	GLuint CloudShadowMapTexture;
	GLuint CloudPrevShadowMapTexture;
	//noise stock textures
	GLuint TextureID_CurlNoise;
	GLuint TextureID_CirrusCloud;
	GLuint TextureID_CirroStratusCloud;
	GLuint TextureID_CirroCumulusCloud;
	GLuint TextureID_WeatherMap;
	GLuint TextureID_WeatherMapTileable;

	//WeatherMap
	const GLchar* WeatherMapTextureFilename = {"Resources/Textures/MyWeather.tga"};
	const GLchar* WeatherMapTileableTextureFilename = {"Resources/Textures/MyWeather_.tga"};
	//curl noise
	//const GLchar* CurlNoiseTextureFilename = {"Resources/Textures/CurlNoise.tga"};
	const GLchar* CurlNoiseTextureFilename = {"Resources/Textures/CumuloNimbus.tga"};
	//2D cloud
	const GLchar* CirrusCloudTextureFilename = { "Resources/Textures/Clouds/cirrus.tga" };
	const GLchar* CirroStratusCloudTextureFilename = { "Resources/Textures/Clouds/cirrostratus.tga" };
	const GLchar* CirroCumulusCloudTextureFilename = { "Resources/Textures/Clouds/cirrocumulus.tga" };

	//variables
	GLfloat fCloudFactor;
	vmath::vec3 v3SunColor;

	//FrameBuffers
	FrameBuffer * Fbo_TemporalUpscale;
	FrameBuffer * Fbo_TemporalShadowUpscale;
	//FrameBuffer *Fbo_PostProcess;

	//atmosphere
	vmath::uvec3 AtmoTextures;
};

