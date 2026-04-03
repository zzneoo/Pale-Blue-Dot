#include "stdafx.h"
#include "Clouds.h"
#include"MyOpenGL.h"
#include "MyWin32.h"

Clouds::Clouds(GLuint AtmoShader, vmath::uvec3 atmoTextures)
{
	CloudPreviousFrameTexture = 0;
	CloudFarPreviousFrameTexture = 0;
	CloudFarCurrFrameTexture = 0;
	CloudCurrFrameNearestTexture = 0;
	CloudCurrFrameLinearTexture = 0;
	CloudVelocityTexture = 0;
	CloudShadowMapTexture = 0;
	CloudPrevShadowMapTexture = 0;

	fCloudFactor = 0.0f;
	v3SunColor = vmath::vec3(1.0f);

	InitializeAll();

	cloudShader = new CloudShaderLoader(AtmoShader);
	cloudGroundShader = new CloudGroundShaderLoader(AtmoShader);
	cloudSpaceShader = new CloudSpaceShaderLoader(AtmoShader);
	AtmoTextures = atmoTextures;
}

Clouds::~Clouds()
{

	if (CloudCurrFrameNearestTexture)
	{
		glDeleteTextures(1, &CloudCurrFrameNearestTexture);
		CloudCurrFrameNearestTexture = 0;
	}
	if (CloudCurrFrameLinearTexture)
	{
		glDeleteTextures(1, &CloudCurrFrameLinearTexture);
		CloudCurrFrameLinearTexture = 0;
	}
	//prev
	if (CloudPreviousFrameTexture)
	{
		glDeleteTextures(1, &CloudPreviousFrameTexture);
		CloudPreviousFrameTexture = 0;
	}
	if (CloudFarPreviousFrameTexture)
	{
		glDeleteTextures(1, &CloudFarPreviousFrameTexture);
		CloudFarPreviousFrameTexture = 0;
	}

	if (CloudFarCurrFrameTexture)
	{
		glDeleteTextures(1, &CloudFarCurrFrameTexture);
		CloudFarCurrFrameTexture = 0;
	}

	//velocity
	if (CloudVelocityTexture)
	{
		glDeleteTextures(1, &CloudVelocityTexture);
		CloudVelocityTexture = 0;
	}

	//shadowMap
	if (CloudShadowMapTexture)
	{
		glDeleteTextures(1, &CloudShadowMapTexture);
		CloudShadowMapTexture = 0;
	}
	if (CloudPrevShadowMapTexture)
	{
		glDeleteTextures(1, &CloudPrevShadowMapTexture);
		CloudPrevShadowMapTexture = 0;
	}

	//2d clouds
	if (TextureID_CirroStratusCloud)
	{
		glDeleteTextures(1, &TextureID_CirroStratusCloud);
		TextureID_CirroStratusCloud = 0;
	}
	if (TextureID_CirrusCloud)
	{
		glDeleteTextures(1, &TextureID_CirrusCloud);
		TextureID_CirrusCloud = 0;
	}
	if (TextureID_CirroCumulusCloud)
	{
		glDeleteTextures(1, &TextureID_CirroCumulusCloud);
		TextureID_CirroCumulusCloud = 0;
	}

	if (TextureID_CurlNoise)
	{
		glDeleteTextures(1, &TextureID_CurlNoise);
		TextureID_CurlNoise = 0;
	}
	//weather
	if (TextureID_WeatherMap)
	{
		glDeleteTextures(1, &TextureID_WeatherMap);
		TextureID_WeatherMap = 0;
	}

	if (TextureID_WeatherMapTileable)
	{
		glDeleteTextures(1, &TextureID_WeatherMapTileable);
		TextureID_WeatherMapTileable = 0;
	}

	//Framebuffers
	if (Fbo_TemporalUpscale)
	{
		delete Fbo_TemporalUpscale;
		Fbo_TemporalUpscale = nullptr;
	}

	//if (Fbo_PostProcess)
	//{
	//	delete Fbo_PostProcess;
	//	Fbo_PostProcess = nullptr;
	//}
}

void Clouds::SetProgramUniformsAtmoTextures(GLuint program,GLuint transmittance_texture_unit,GLuint scattering_texture_unit,GLuint irradiance_texture_unit,vmath::uvec3 atmoTextures)
{
	glActiveTexture(GL_TEXTURE0 + transmittance_texture_unit);
	glBindTexture(GL_TEXTURE_2D, atmoTextures[0]);
	glUniform1i(glGetUniformLocation(program, "transmittance_texture"),
		transmittance_texture_unit);

	glActiveTexture(GL_TEXTURE0 + scattering_texture_unit);
	glBindTexture(GL_TEXTURE_3D, atmoTextures[1]);
	glUniform1i(glGetUniformLocation(program, "scattering_texture"),
		scattering_texture_unit);

	glActiveTexture(GL_TEXTURE0 + irradiance_texture_unit);
	glBindTexture(GL_TEXTURE_2D, atmoTextures[2]);
	glUniform1i(glGetUniformLocation(program, "irradiance_texture"),
		irradiance_texture_unit);

}

void Clouds::InitializeAll(void)
{
	//stock Textures
	TextureID_CurlNoise = MyTGA_Loader::LoadTGA_texture(CurlNoiseTextureFilename, false, GL_LINEAR, false);
	TextureID_CirrusCloud = MyTGA_Loader::LoadTGA_texture(CirrusCloudTextureFilename, true, GL_LINEAR, true);
	TextureID_CirroStratusCloud = MyTGA_Loader::LoadTGA_texture(CirroStratusCloudTextureFilename, true, GL_LINEAR, true);
	TextureID_CirroCumulusCloud = MyTGA_Loader::LoadTGA_texture(CirroCumulusCloudTextureFilename, true, GL_LINEAR, true);
	TextureID_WeatherMap = MyTGA_Loader::LoadTGA_texture(WeatherMapTextureFilename, true, GL_LINEAR, false);
	TextureID_WeatherMapTileable = MyTGA_Loader::LoadTGA_texture(WeatherMapTileableTextureFilename, true, GL_LINEAR, true);

	Initialize();

	//Framebuffers
	Fbo_TemporalUpscale = new FrameBuffer(GL_TRUE, 2, GL_FALSE, GL_FALSE, GL_FALSE, GL_RGBA8, GL_UNSIGNED_BYTE, GL_FALSE, 1, uvec2(1920, 1080));
	Fbo_TemporalShadowUpscale = new FrameBuffer(GL_TRUE, 1, GL_FALSE, GL_FALSE, GL_FALSE, GL_RGBA16F, GL_FLOAT, GL_FALSE, 1, uvec2(4096, 4096));//OnlyDepth, so some of other params arent used.
	//Fbo_PostProcess = new FrameBuffer(GL_TRUE, 1, GL_FALSE, GL_FALSE, 0);
}

void Clouds::Initialize(void)
{

	//currentNearest
	glGenTextures(1, &CloudCurrFrameNearestTexture);
	glBindTexture(GL_TEXTURE_2D, CloudCurrFrameNearestTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 480, 270, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	//currentLinear
	glGenTextures(1, &CloudCurrFrameLinearTexture);
	glBindTexture(GL_TEXTURE_2D, CloudCurrFrameLinearTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 3840, 2160, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	//currentFar
	glGenTextures(1, &CloudFarCurrFrameTexture);
	glBindTexture(GL_TEXTURE_2D, CloudFarCurrFrameTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 3840, 2160, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);


	//PreviousCloudFrame
	glGenTextures(1, &CloudPreviousFrameTexture);
	glBindTexture(GL_TEXTURE_2D, CloudPreviousFrameTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	//PreviousCloudFar
	glGenTextures(1, &CloudFarPreviousFrameTexture);
	glBindTexture(GL_TEXTURE_2D, CloudFarPreviousFrameTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);


	//cloudVelocity
	glGenTextures(1, &CloudVelocityTexture);
	glBindTexture(GL_TEXTURE_2D, CloudVelocityTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 480, 270, 0, GL_RG, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	//CloudShadowMap
	
	//glGenTextures(1, &CloudPrevShadowMapTexture);
	//glBindTexture(GL_TEXTURE_2D, CloudPrevShadowMapTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4096, 4096, 0, GL_RGBA, GL_FLOAT, NULL);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &CloudShadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, CloudShadowMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 8192, 8192, 0, GL_RGBA, GL_FLOAT, NULL);//256 ,256

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Clouds::ResizeFramebuffers(GLsizei Width, GLsizei Height)
{
	if(Fbo_TemporalUpscale)
	Fbo_TemporalUpscale->ResizeFramebuffer(Width, Height);
}

void Clouds::RenderComputeCloudShadowMap(GLfloat fEarthRadius, vmath::mat4 CameraViewMatrix, vmath::vec3 CameraPos, GLuint TextureID_PerlinWorley)
{
	GLushort WorkGroupSize = 32;
	vmath::vec2 resolution = vmath::vec2(8192.0, 8192.0);

	cloudShadowMapShader.Start();

	cloudShadowMapShader.LoadSampler2D_WeatherMapUniform(GL_TEXTURE0, 0, this->TextureID_WeatherMap);
	cloudShadowMapShader.LoadSampler3D_PerlinWorleyUniform(GL_TEXTURE1, 1, TextureID_PerlinWorley);
	cloudShadowMapShader.LoadSampler2D_WeatherMapTilableUniform(GL_TEXTURE2, 2, this->TextureID_WeatherMapTileable);

	cloudShadowMapShader.Loadm4LightViewProjX_Uniform(MyWin32::gLightViewProjectionMatrix);
	cloudShadowMapShader.Loadm4InvViewX_Uniform(vmath::inverse(CameraViewMatrix));
	//cloudShadowMapShader.Loadm4PrevViewX_Uniform(MyWin32::gPreviousViewMatrix);
	cloudShadowMapShader.Loadm4ProjX_Uniform(MyWin32::gLightOrthoProjectionMatrix);//testing perspective not ortho
	cloudShadowMapShader.Loadm4InvProjX_Uniform(vmath::inverse(MyWin32::gLightOrthoProjectionMatrix));
	cloudShadowMapShader.LoadfEarthRadius_Uniform(fEarthRadius);
	cloudShadowMapShader.LoadfShadowFactor_Uniform(RayCaster::fUniformScale);
	cloudShadowMapShader.Loadv3CameraPos_Uniform(CameraPos);

	cloudShadowMapShader.LoadiPixelID_Uniform(MyWin32::iFrameID%256);
	cloudShadowMapShader.Loadv2InnerOuterRadius_Uniform(vmath::vec2(1.0f, 1.79f));

	glBindImageTexture(0, CloudShadowMapTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	glDispatchCompute(INT_CEIL(resolution[0], WorkGroupSize), INT_CEIL(resolution[1], WorkGroupSize), 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);

	cloudShadowMapShader.Stop();

	//RenderTemporalUpscaleShadowMapQuad(CameraViewMatrix, CameraPos);//realtime test
}

void Clouds::RenderComputeCloudsGround(GLuint TextureID_PerlinWorley, GLuint TextureID_Worley32, GLuint TextureID_WeatherMap, GLuint TextureID_BlueNoise, vmath::mat4 CameraViewMatrix, vmath::vec3 CameraPos, vmath::vec3 SunPos, GLfloat VolumetricCoverage,GLfloat HighAltitudeCoverage,GLfloat timeMultiplier,GLfloat EarthRadius)
{
	GLushort WorkGroupSize = 32;
	vmath::vec2 resolution = vmath::vec2(3840, 2160);//quarter resolution
	cloudGroundShader->Start();

	cloudGroundShader->LoadExposure_Uniform(MyOpenGL::fExposure);
	cloudGroundShader->LoadfTime_Uniform( timeMultiplier +0000.0f);
	cloudGroundShader->LoadfCloudFactor_Uniform(fCloudFactor);
	cloudGroundShader->LoadfEarthRadius_Uniform(EarthRadius);//fEarthRadius
	cloudGroundShader->LoadfCloudVolumetricCoverage_Uniform(VolumetricCoverage);
	cloudGroundShader->LoadfCloudHighAltitudeCoverage_Uniform(HighAltitudeCoverage);

	cloudGroundShader->LoadSampler3D_PerlinWorleyUniform(GL_TEXTURE0, 0, TextureID_PerlinWorley);
	cloudGroundShader->LoadSampler3D_Worley32Uniform(GL_TEXTURE1, 1, TextureID_Worley32);
	cloudGroundShader->LoadSampler2D_WeatherMapUniform(GL_TEXTURE2, 2, TextureID_WeatherMap);
	cloudGroundShader->LoadSampler2D_BlueNoiseUniform(GL_TEXTURE3, 3, TextureID_BlueNoise);
	cloudGroundShader->LoadSampler2D_CurlNoiseUniform(GL_TEXTURE4, 4, TextureID_CurlNoise);
	cloudGroundShader->LoadSampler2D_CirrusCloudUniform(GL_TEXTURE5, 5, TextureID_CirrusCloud);
	cloudGroundShader->LoadSampler2D_CirroStratusCloudUniform(GL_TEXTURE6, 6, TextureID_CirroStratusCloud);
	cloudGroundShader->LoadSampler2D_CirroCumulusCloudUniform(GL_TEXTURE7, 7, TextureID_CirroCumulusCloud);

	SetProgramUniformsAtmoTextures(cloudGroundShader->GetShaderProgram(), 8, 9, 10, AtmoTextures);

	cloudGroundShader->LoadSampler2D_BeerShadowMapUniform(GL_TEXTURE11, 11, this->CloudPrevShadowMapTexture);

	cloudGroundShader->Loadm4LightViewProjX_Uniform(MyWin32::gLightViewProjectionMatrix);
	cloudGroundShader->Loadm4InvViewX_Uniform(vmath::inverse(CameraViewMatrix));
	cloudGroundShader->Loadm4PrevViewX_Uniform(MyWin32::gPreviousViewMatrix);
	cloudGroundShader->Loadm4ProjX_Uniform(MyWin32::gProjectionMatrix);
	cloudGroundShader->Loadm4InvProjX_Uniform(vmath::inverse(MyWin32::gProjectionMatrix));

	cloudGroundShader->Loadv3CameraPos_Uniform(CameraPos);
	cloudGroundShader->Loadv3SunPos_Uniform(SunPos);

	GLdouble SunTheta = vmath::dot(vmath::normalize(SunPos), vmath::vec3(0.0f, 1.0f, 0.0f));
	//cloudGroundShader.Loadv3SunColor_Uniform(getLightColor(SunTheta));
	//cloudGroundShader.Loadv3SunColor_Uniform(v3SunColor);
	cloudGroundShader->Loadv3SunColor_Uniform( v3SunColor);//sunIrradiance without transmittance  //vmath::vec3(1.4740000f, 1.85040000f, 1.85040000f) *

	cloudGroundShader->Loadv2Resolution_Uniform(resolution);
	cloudGroundShader->Loadv2InnerOuterRadius_Uniform(vmath::vec2(1.0f, 2.0f));//vmath::vec2(2.0f, 2.977321032f) Life

	cloudGroundShader->LoadiPixelID_Uniform(MyWin32::iBlockPixelID);
	cloudGroundShader->LoadiFrameID_Uniform(MyWin32::iFrameID);

	glBindImageTexture(0, CloudCurrFrameNearestTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glBindImageTexture(1, CloudCurrFrameLinearTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glBindImageTexture(2, CloudVelocityTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG16F);
	glBindImageTexture(3, CloudFarCurrFrameTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glDispatchCompute(INT_CEIL(resolution[0], WorkGroupSize), INT_CEIL(resolution[1], WorkGroupSize), 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	cloudGroundShader->Stop();
}

void Clouds::RenderComputeCloudsSpace(GLuint TextureID_PerlinWorley, GLuint TextureID_Worley32, GLuint TextureID_WeatherMap, GLuint TextureID_BlueNoise, vmath::mat4 CameraViewMatrix, vmath::vec3 CameraPos, vmath::vec3 SunPos, GLfloat VolumetricCoverage,GLfloat HighAltitudeCoverage,GLfloat timeMultiplier,GLfloat EarthRadius)
{
	GLushort WorkGroupSize = 32;
	vmath::vec2 resolution = vmath::vec2(3840, 2160);//quarter resolution
	cloudSpaceShader->Start();

	cloudSpaceShader->LoadfTime_Uniform(MyOpenGL::fTime* timeMultiplier +0000.0f);
	cloudSpaceShader->LoadfCloudFactor_Uniform(fCloudFactor);
	cloudSpaceShader->LoadfEarthRadius_Uniform(EarthRadius);//fEarthRadius
	cloudSpaceShader->LoadfCloudVolumetricCoverage_Uniform(VolumetricCoverage);
	cloudSpaceShader->LoadfCloudHighAltitudeCoverage_Uniform(HighAltitudeCoverage);

	cloudSpaceShader->LoadSampler3D_PerlinWorleyUniform(GL_TEXTURE0, 0, TextureID_PerlinWorley);
	cloudSpaceShader->LoadSampler3D_Worley32Uniform(GL_TEXTURE1, 1, TextureID_Worley32);
	cloudSpaceShader->LoadSampler2D_WeatherMapUniform(GL_TEXTURE2, 2, TextureID_WeatherMap);
	cloudSpaceShader->LoadSampler2D_BlueNoiseUniform(GL_TEXTURE3, 3, TextureID_BlueNoise);
	cloudSpaceShader->LoadSampler2D_CurlNoiseUniform(GL_TEXTURE4, 4, TextureID_CurlNoise);
	cloudSpaceShader->LoadSampler2D_CirrusCloudUniform(GL_TEXTURE5, 5, TextureID_CirrusCloud);
	cloudSpaceShader->LoadSampler2D_CirroStratusCloudUniform(GL_TEXTURE6, 6, TextureID_CirroStratusCloud);
	cloudSpaceShader->LoadSampler2D_CirroCumulusCloudUniform(GL_TEXTURE7, 7, TextureID_CirroCumulusCloud);

	SetProgramUniformsAtmoTextures(cloudSpaceShader->GetShaderProgram(), 8, 9, 10, AtmoTextures);

	cloudSpaceShader->LoadSampler2D_BeerShadowMapUniform(GL_TEXTURE11, 11, this->CloudPrevShadowMapTexture);

	cloudSpaceShader->Loadm4LightViewProjX_Uniform(MyWin32::gLightViewProjectionMatrix);
	cloudSpaceShader->Loadm4InvViewX_Uniform(vmath::inverse(CameraViewMatrix));
	cloudSpaceShader->Loadm4PrevViewX_Uniform(MyWin32::gPreviousViewMatrix);
	cloudSpaceShader->Loadm4ProjX_Uniform(MyWin32::gProjectionMatrix);
	cloudSpaceShader->Loadm4InvProjX_Uniform(vmath::inverse(MyWin32::gProjectionMatrix));

	cloudSpaceShader->Loadv3CameraPos_Uniform(CameraPos);
	cloudSpaceShader->Loadv3SunPos_Uniform(SunPos);

	GLdouble SunTheta = vmath::dot(vmath::normalize(SunPos), vmath::vec3(0.0f, 1.0f, 0.0f));
	//cloudSpaceShader.Loadv3SunColor_Uniform(getLightColor(SunTheta));
	//cloudSpaceShader.Loadv3SunColor_Uniform(v3SunColor);
	cloudSpaceShader->Loadv3SunColor_Uniform( v3SunColor);//sunIrradiance without transmittance  //vmath::vec3(1.4740000f, 1.85040000f, 1.85040000f) *

	cloudSpaceShader->Loadv2Resolution_Uniform(resolution);
	cloudSpaceShader->Loadv2InnerOuterRadius_Uniform(vmath::vec2(1.0f, 2.0f));

	cloudSpaceShader->LoadiPixelID_Uniform(MyWin32::iBlockPixelID);
	cloudSpaceShader->LoadiFrameID_Uniform(MyWin32::iFrameID);

	glBindImageTexture(0, CloudCurrFrameNearestTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glBindImageTexture(1, CloudCurrFrameLinearTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glBindImageTexture(2, CloudVelocityTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG16F);
	glBindImageTexture(3, CloudFarCurrFrameTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glDispatchCompute(INT_CEIL(resolution[0], WorkGroupSize), INT_CEIL(resolution[1], WorkGroupSize), 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	cloudSpaceShader->Stop();
}

void Clouds::RenderComputeClouds(GLuint TextureID_PerlinWorley, GLuint TextureID_Worley32, GLuint TextureID_WeatherMap, GLuint TextureID_BlueNoise, vmath::mat4 CameraViewMatrix, vmath::vec3 CameraPos, vmath::vec3 SunPos, GLfloat VolumetricCoverage,GLfloat HighAltitudeCoverage,GLfloat timeMultiplier,GLfloat EarthRadius)
{
	GLushort WorkGroupSize = 32;
	vmath::vec2 resolution = vmath::vec2(3840, 2160);//quarter resolution
	cloudShader->Start();

	cloudShader->LoadfTime_Uniform(MyOpenGL::fTime* timeMultiplier +0000.0f);
	cloudShader->LoadfCloudFactor_Uniform(fCloudFactor);
	cloudShader->LoadfEarthRadius_Uniform(EarthRadius);//fEarthRadius
	cloudShader->LoadfCloudVolumetricCoverage_Uniform(VolumetricCoverage);
	cloudShader->LoadfCloudHighAltitudeCoverage_Uniform(HighAltitudeCoverage);

	cloudShader->LoadSampler3D_PerlinWorleyUniform(GL_TEXTURE0, 0, TextureID_PerlinWorley);
	cloudShader->LoadSampler3D_Worley32Uniform(GL_TEXTURE1, 1, TextureID_Worley32);
	cloudShader->LoadSampler2D_WeatherMapUniform(GL_TEXTURE2, 2, TextureID_WeatherMap);
	cloudShader->LoadSampler2D_BlueNoiseUniform(GL_TEXTURE3, 3, TextureID_BlueNoise);
	cloudShader->LoadSampler2D_CurlNoiseUniform(GL_TEXTURE4, 4, TextureID_CurlNoise);
	cloudShader->LoadSampler2D_CirrusCloudUniform(GL_TEXTURE5, 5, TextureID_CirrusCloud);
	cloudShader->LoadSampler2D_CirroStratusCloudUniform(GL_TEXTURE6, 6, TextureID_CirroStratusCloud);
	cloudShader->LoadSampler2D_CirroCumulusCloudUniform(GL_TEXTURE7, 7, TextureID_CirroCumulusCloud);
	cloudShader->LoadSampler2D_WeatherMapTilableUniform(GL_TEXTURE12, 12, TextureID_WeatherMapTileable);

	SetProgramUniformsAtmoTextures(cloudShader->GetShaderProgram(), 8, 9, 10, AtmoTextures);

	cloudShader->LoadSampler2D_BeerShadowMapUniform(GL_TEXTURE11, 11,this->GetCloudShadowMapTexture() );//this->CloudPrevShadowMapTexture //for realtime

	cloudShader->Loadm4LightViewProjX_Uniform(MyWin32::gLightViewProjectionMatrix);
	cloudShader->Loadm4InvViewX_Uniform(vmath::inverse(CameraViewMatrix));
	cloudShader->Loadm4PrevViewX_Uniform(MyWin32::gPreviousViewMatrix);
	cloudShader->Loadm4ProjX_Uniform(MyWin32::gProjectionMatrix);
	cloudShader->Loadm4InvProjX_Uniform(vmath::inverse(MyWin32::gProjectionMatrix));

	cloudShader->Loadv3CameraPos_Uniform(CameraPos);
	cloudShader->Loadv3SunPos_Uniform(SunPos);

	GLdouble SunTheta = vmath::dot(vmath::normalize(SunPos), vmath::vec3(0.0f, 1.0f, 0.0f));
	//cloudShader.Loadv3SunColor_Uniform(getLightColor(SunTheta));
	//cloudShader.Loadv3SunColor_Uniform(v3SunColor);
	cloudShader->Loadv3SunColor_Uniform( v3SunColor);//sunIrradiance without transmittance  //vmath::vec3(1.4740000f, 1.85040000f, 1.85040000f) *

	cloudShader->Loadv2Resolution_Uniform(resolution);
	cloudShader->Loadv2InnerOuterRadius_Uniform(vmath::vec2(1.0f, 1.79f));

	cloudShader->LoadiPixelID_Uniform(MyWin32::iBlockPixelID);
	cloudShader->LoadiFrameID_Uniform(MyWin32::iFrameID);

	glBindImageTexture(0, CloudCurrFrameNearestTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glBindImageTexture(1, CloudCurrFrameLinearTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glBindImageTexture(2, CloudVelocityTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG16F);
	glBindImageTexture(3, CloudFarCurrFrameTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glDispatchCompute(INT_CEIL(resolution[0], WorkGroupSize), INT_CEIL(resolution[1], WorkGroupSize), 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	cloudShader->Stop();
}

void Clouds::RenderTemporalUpscaleQuad(vmath::mat4 ViewMatrix,vmath::vec3 CameraPos)
{

	glBindFramebuffer(GL_FRAMEBUFFER,Fbo_TemporalUpscale->GetFbo());
	glViewport(0, 0, 1920, 1080);
	glClear(GL_COLOR_BUFFER_BIT);

	temporalPassShader.Start();

	temporalPassShader.LoadSampler2DCurrFrameNearest_Uniform(GL_TEXTURE0, 0, CloudCurrFrameNearestTexture);//quarter res
	temporalPassShader.LoadSampler2DCurrFrameLinear_Uniform(GL_TEXTURE1, 1, CloudCurrFrameLinearTexture);//quarter res

	temporalPassShader.LoadSampler2DPrevFrame_Uniform(GL_TEXTURE2, 2, CloudPreviousFrameTexture);//full res
	temporalPassShader.LoadSampler2DPrevFrameExtra_Uniform(GL_TEXTURE3, 3, CloudFarPreviousFrameTexture);//full res

	temporalPassShader.LoadSampler2DCurrVelocity_Uniform(GL_TEXTURE4, 4, CloudVelocityTexture);//quarter res
	temporalPassShader.LoadSampler2DCurrFrameExtra_Uniform(GL_TEXTURE5, 5, CloudFarCurrFrameTexture);//quarter res

	temporalPassShader.Loadv3CameraPosition_Uniform(CameraPos);

	temporalPassShader.Loadm4ProjX_Uniform(MyWin32::gProjectionMatrix);
	temporalPassShader.Loadm4PrevViewX_Uniform(MyWin32::gPreviousViewMatrix);
	temporalPassShader.Loadm4ViewX_Uniform(ViewMatrix);

	temporalPassShader.LoadfTime_Uniform(MyOpenGL::fTime);
	temporalPassShader.LoadiBlockPixelID_Uniform(MyWin32::iBlockPixelID);

	glDrawArrays(GL_QUADS, 0, 4);

	temporalPassShader.Stop();

	ResolveToPreviousFrame();
}

void Clouds::RenderTemporalUpscaleShadowMapQuad(vmath::mat4 ViewMatrix, vmath::vec3 CameraPos)
{

	glBindFramebuffer(GL_FRAMEBUFFER, Fbo_TemporalShadowUpscale->GetFbo());
	vmath::uvec2 res = Fbo_TemporalShadowUpscale->GetFboResolution();
	glViewport(0, 0, res[0], res[1]);
	glClear(GL_COLOR_BUFFER_BIT);

	temporalShadowShader.Start();

	temporalShadowShader.LoadSampler2DCurrFrameNearest_Uniform(GL_TEXTURE0, 0, CloudShadowMapTexture);//quarter res
	temporalShadowShader.LoadSampler2DPrevFrame_Uniform(GL_TEXTURE2, 2, CloudPrevShadowMapTexture);//full res
	temporalShadowShader.LoadSampler2DCurrVelocity_Uniform(GL_TEXTURE4, 4,0 );//quarter res

	temporalShadowShader.Loadv3CameraPosition_Uniform(CameraPos);

	temporalShadowShader.Loadm4ProjX_Uniform(MyWin32::gProjectionMatrix);
	temporalShadowShader.Loadm4PrevViewX_Uniform(MyWin32::gPreviousViewMatrix);
	temporalShadowShader.Loadm4ViewX_Uniform(ViewMatrix);

	temporalShadowShader.LoadfTime_Uniform(MyOpenGL::fTime);
	temporalShadowShader.LoadiBlockPixelID_Uniform(MyWin32::iFrameID%256);

	glDrawArrays(GL_QUADS, 0, 4);

	temporalShadowShader.Stop();

	//resolve previous frame  shadowMap 
	glCopyImageSubData(Fbo_TemporalShadowUpscale->GetFbo_Color_Texture(0),
		GL_TEXTURE_2D, 0, 0, 0, 0,
		CloudPrevShadowMapTexture,
		GL_TEXTURE_2D, 0, 0, 0, 0,
		res[0], res[1], 1);
}

void Clouds::ResolveToPreviousFrame(void)
{
	//glCopyImageSubData(Fbo_Temporal->GetFbo_Color0_Texture(), GL_TEXTURE_2D, 0, 0, 0, 0, CloudPreviousFrameTexture, GL_TEXTURE_2D, 0, 0, 0, 0, MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight, 1);

	//previous frame
	glCopyImageSubData(Fbo_TemporalUpscale->GetFbo_Color_Texture(0),
		GL_TEXTURE_2D, 0, 0, 0, 0,
		CloudPreviousFrameTexture,
		GL_TEXTURE_2D, 0, 0, 0, 0,
		MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight, 1);

	//previous depth
	glCopyImageSubData(Fbo_TemporalUpscale->GetFbo_Color_Texture(1),
		GL_TEXTURE_2D, 0, 0, 0, 0,
		CloudFarPreviousFrameTexture,
		GL_TEXTURE_2D, 0, 0, 0, 0,
		MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight, 1);
}

vmath::vec3 Clouds::getLightColor(GLdouble sunTheta)
{
	GLdouble sunTheta2 = sunTheta * sunTheta;
	GLdouble sunTheta3 = sunTheta2 * sunTheta;

	vmath::vec3 first = vmath::mix(vmath::vec3(1.0f, 0.008f, 0.0f), vmath::vec3(1.0f, 0.027f, 0.0f), (float)smoothstep(0.0f, 0.0613f, sunTheta));//sunTheta

	vmath::vec3 second = vmath::mix(first, vmath::vec3(0.725f, 0.070f, 0.007f), (float)smoothstep(0.0613f, 0.074711f, sunTheta2));

	vmath::vec3 third = vmath::mix(second, vmath::vec3(0.858f, 0.317f, 0.122f), (float)smoothstep(0.074711f, 0.101533f, sunTheta2));

	vmath::vec3 fourth = vmath::mix(third, vmath::vec3(0.858f, 0.317f, 0.122f), (float)smoothstep(0.101533f, 0.128355f, pow(sunTheta,1.07f)));

	vmath::vec3 last = vmath::mix(fourth, vmath::vec3(1.0f), (float)smoothstep(0.128355f, 0.4f, sunTheta2));

	return last;
}
