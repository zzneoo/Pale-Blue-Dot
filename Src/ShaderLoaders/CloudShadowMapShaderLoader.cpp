#include "CloudShadowMapShaderLoader.h"
#include"MyOpenGL.h"

CloudShadowMapShaderLoader::CloudShadowMapShaderLoader()
{
	Sampler3D_PerlinWorley3DUniform = 0;
//	Sampler3D_Worley32Uniform = 0;
	Sampler2D_WeatherMapUniform = 0;
	Sampler2D_WeatherMapTilableUniform = 0;
//	Sampler2D_BlueNoiseUniform = 0;
//	Sampler2D_CurlNoiseUniform = 0;

//	Sampler2D_CirrusCloudUniform = 0;
//	Sampler2D_CirroStratusCloudUniform = 0;
//	Sampler2D_CirroCumulusCloudUniform = 0;

//	v3SunPos_Uniform = 0;
	v3CameraPos_Uniform = 0;
	v2InnerOuterRadius_Uniform = 0;
//	v3SunColor_Uniform = 0;

	m4InvViewX_Uniform = 0;
	m4LightViewProjX_Uniform = 0;
	m4InvProjX_Uniform = 0;
	m4ProjX_Uniform = 0;
	m4PrevViewX_Uniform = 0;

	fEarthRadius_Uniform = 0;
	fShadowFactor_Uniform = 0;
//	v2Resolution_Uniform = 0;

//	fTime_Uniform = 0;
//	fCloudFactor_Uniform = 0;
//	fCloudVolumetricCoverage_Uniform = 0;
//	fCloudHighAltitudeCoverage_Uniform = 0;

	iPixelID_Uniform = 0;
//	iFrameID_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}
CloudShadowMapShaderLoader::~CloudShadowMapShaderLoader()
{

}
//Samplers

void CloudShadowMapShaderLoader::LoadSampler3D_PerlinWorleyUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture3D(Sampler3D_PerlinWorley3DUniform, TEXTURE, tNo, textureID);
}
//void CloudShadowMapShaderLoader::LoadSampler3D_Worley32Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
//{
//	UniformLoadTexture3D(Sampler3D_Worley32Uniform, TEXTURE, tNo, textureID);
//}
void CloudShadowMapShaderLoader::LoadSampler2D_WeatherMapUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_WeatherMapUniform, TEXTURE, tNo, textureID);
}
void CloudShadowMapShaderLoader::LoadSampler2D_WeatherMapTilableUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_WeatherMapTilableUniform, TEXTURE, tNo, textureID);
}

//int
void CloudShadowMapShaderLoader::LoadiPixelID_Uniform(GLint PixelID)
{
	UniformLoad1i(iPixelID_Uniform, PixelID);
}

/////////////////////////////////////////////////////Matrices/////////////////////////////////////////////////////////////////////

void CloudShadowMapShaderLoader::Loadm4InvViewX_Uniform(vmath::mat4 InvViewMatrix)
{
	UniformLoadMatrix(m4InvViewX_Uniform, InvViewMatrix);
}

void CloudShadowMapShaderLoader::Loadm4LightViewProjX_Uniform(vmath::mat4 LightViewProjMatrix)
{
	UniformLoadMatrix(m4LightViewProjX_Uniform, LightViewProjMatrix);
}

void CloudShadowMapShaderLoader::Loadm4PrevViewX_Uniform(vmath::mat4 PrevViewMatrix)
{
	UniformLoadMatrix(m4PrevViewX_Uniform, PrevViewMatrix);
}

void CloudShadowMapShaderLoader::Loadm4ProjX_Uniform(vmath::mat4 ProjMatrix)
{
	UniformLoadMatrix(m4ProjX_Uniform, ProjMatrix);
}

void CloudShadowMapShaderLoader::Loadm4InvProjX_Uniform(vmath::mat4 InvProjMatrix)
{

	UniformLoadMatrix(m4InvProjX_Uniform, InvProjMatrix);
}

////////////////////////////////////////////////////////Vec3/////////////////////////////////////////////////////////////////////
void CloudShadowMapShaderLoader::Loadv3CameraPos_Uniform(vmath::vec3 CameraPos)
{
	UniformLoadVec3(v3CameraPos_Uniform, CameraPos);
}

void CloudShadowMapShaderLoader::LoadfEarthRadius_Uniform(GLfloat EarthRadius)
{
	UniformLoad1f(fEarthRadius_Uniform, EarthRadius);
}

void CloudShadowMapShaderLoader::Loadv2InnerOuterRadius_Uniform(vmath::vec2 InnerOuterRadius)
{
	UniformLoadVec2(v2InnerOuterRadius_Uniform, InnerOuterRadius);
}

void CloudShadowMapShaderLoader::LoadfShadowFactor_Uniform(GLfloat fFactor)
{
	UniformLoad1f(fShadowFactor_Uniform, fFactor);
}
//void CloudShaderLoader::Loadv3SunPos_Uniform(vmath::vec3 SunPos)
//{
//	UniformLoadVec3(v3SunPos_Uniform, SunPos);
//}



bool CloudShadowMapShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_cs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void CloudShadowMapShaderLoader::GetAllUniformLocations(void)
{
	Sampler3D_PerlinWorley3DUniform = GetUniformLocation("tSampler3D_PerlinWorley");
//	Sampler3D_Worley32Uniform = GetUniformLocation("tSampler3D_Worley32");
	Sampler2D_WeatherMapUniform= GetUniformLocation("tSampler2D_WeatherMap");
	Sampler2D_WeatherMapTilableUniform = GetUniformLocation("tSampler2D_WeatherMapTilable");
//	Sampler2D_BlueNoiseUniform = GetUniformLocation("tSampler2D_BlueNoise");
//	Sampler2D_CurlNoiseUniform = GetUniformLocation("tSampler2D_CurlNoise");

//	Sampler2D_CirrusCloudUniform = GetUniformLocation("tSampler2D_Cirrus");
//	Sampler2D_CirroStratusCloudUniform = GetUniformLocation("tSampler2D_CirroStratus");
//	Sampler2D_CirroCumulusCloudUniform = GetUniformLocation("tSampler2D_CirroCumulus");

	m4InvViewX_Uniform = GetUniformLocation("m4InvViewX");
	m4LightViewProjX_Uniform = GetUniformLocation("m4LightViewProjX");
	m4PrevViewX_Uniform= GetUniformLocation("m4PrevViewX");
	m4InvProjX_Uniform = GetUniformLocation("m4InvProjX");
	m4ProjX_Uniform = GetUniformLocation("m4ProjX");

	v3CameraPos_Uniform = GetUniformLocation("v3CameraPosition");
	fEarthRadius_Uniform = GetUniformLocation("fEarthRadius");
	fShadowFactor_Uniform = GetUniformLocation("fShadowFactor");

	v2InnerOuterRadius_Uniform = GetUniformLocation("v2InnerOuterRadius");

//	v3SunPos_Uniform = GetUniformLocation("v3SunPos");
//	v3SunColor_Uniform = GetUniformLocation("v3SunColor");

//	v2Resolution_Uniform = GetUniformLocation("v2Resolution");

//	fTime_Uniform = GetUniformLocation("fTime");
//	fCloudFactor_Uniform = GetUniformLocation("fCloudFactor");
//	fCloudVolumetricCoverage_Uniform = GetUniformLocation("fCloudVolumetricCoverage");
//	fCloudHighAltitudeCoverage_Uniform = GetUniformLocation("fCloudHighAltitudeCoverage");

	iPixelID_Uniform=GetUniformLocation("iPixelID");
//	iFrameID_Uniform= GetUniformLocation("iFrameID");
}
