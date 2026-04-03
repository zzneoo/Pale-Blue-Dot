#include "CloudShaderLoader.h"
#include"MyOpenGL.h"

CloudShaderLoader::CloudShaderLoader(GLuint AtmoShader)
{
	Sampler3D_PerlinWorley3DUniform = 0;
	Sampler3D_Worley32Uniform = 0;
	Sampler2D_WeatherMapUniform = 0;
	Sampler2D_WeatherMapTilableUniform = 0;
	Sampler2D_BlueNoiseUniform = 0;
	Sampler2D_CurlNoiseUniform = 0;
	Sampler2D_BeerShadowMapUniform = 0;

	Sampler2D_CirrusCloudUniform = 0;
	Sampler2D_CirroStratusCloudUniform = 0;
	Sampler2D_CirroCumulusCloudUniform = 0;

	v3SunPos_Uniform = 0;
	v3CameraPos_Uniform = 0;
	v3SunColor_Uniform = 0;

	m4InvViewX_Uniform = 0;
	m4LightViewProjX_Uniform = 0;
	m4InvProjX_Uniform = 0;
	m4ProjX_Uniform = 0;
	m4PrevViewX_Uniform = 0;

	v2Resolution_Uniform = 0;
	v2InnerOuterRadius_Uniform = 0;

	fTime_Uniform = 0;
	fCloudFactor_Uniform = 0;
	fCloudVolumetricCoverage_Uniform = 0;
	fCloudHighAltitudeCoverage_Uniform = 0;
	fEarthRadius_Uniform=0;

	iPixelID_Uniform = 0;
	iFrameID_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		SetAtmosphereShaderObject(AtmoShader);
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}
CloudShaderLoader::~CloudShaderLoader()
{

}
//Samplers

void CloudShaderLoader::LoadSampler3D_PerlinWorleyUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture3D(Sampler3D_PerlinWorley3DUniform, TEXTURE, tNo, textureID);
}
void CloudShaderLoader::LoadSampler3D_Worley32Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture3D(Sampler3D_Worley32Uniform, TEXTURE, tNo, textureID);
}
void CloudShaderLoader::LoadSampler2D_WeatherMapUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_WeatherMapUniform, TEXTURE, tNo, textureID);
}
void CloudShaderLoader::LoadSampler2D_WeatherMapTilableUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_WeatherMapTilableUniform, TEXTURE, tNo, textureID);
}

void CloudShaderLoader::LoadSampler2D_BlueNoiseUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_BlueNoiseUniform, TEXTURE, tNo, textureID);
}

void CloudShaderLoader::LoadSampler2D_CurlNoiseUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_CurlNoiseUniform, TEXTURE, tNo, textureID);
}

void CloudShaderLoader::LoadSampler2D_BeerShadowMapUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_BeerShadowMapUniform, TEXTURE, tNo, textureID);
}

//stock
void CloudShaderLoader::LoadSampler2D_CirrusCloudUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_CirrusCloudUniform, TEXTURE, tNo, textureID);
}
void CloudShaderLoader::LoadSampler2D_CirroStratusCloudUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_CirroStratusCloudUniform, TEXTURE, tNo, textureID);
}
void CloudShaderLoader::LoadSampler2D_CirroCumulusCloudUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_CirroCumulusCloudUniform, TEXTURE, tNo, textureID);
}

/////////////////////////////////////////////////////Matrices/////////////////////////////////////////////////////////////////////

void CloudShaderLoader::Loadm4InvViewX_Uniform(vmath::mat4 InvViewMatrix)
{
	UniformLoadMatrix(m4InvViewX_Uniform, InvViewMatrix);
}

void CloudShaderLoader::Loadm4LightViewProjX_Uniform(vmath::mat4 LightViewProjMatrix)
{
	UniformLoadMatrix(m4LightViewProjX_Uniform, LightViewProjMatrix);
}

void CloudShaderLoader::Loadm4PrevViewX_Uniform(vmath::mat4 PrevViewMatrix)
{
	UniformLoadMatrix(m4PrevViewX_Uniform, PrevViewMatrix);
}

void CloudShaderLoader::Loadm4ProjX_Uniform(vmath::mat4 ProjMatrix)
{
	UniformLoadMatrix(m4ProjX_Uniform, ProjMatrix);
}

void CloudShaderLoader::Loadm4InvProjX_Uniform(vmath::mat4 InvProjMatrix)
{

	UniformLoadMatrix(m4InvProjX_Uniform, InvProjMatrix);
}


///////////////////////////////////////////////////////Float//////////////////////////////////////////////////////////////////////////////

void CloudShaderLoader::LoadfTime_Uniform(GLfloat fTime)
{
	UniformLoad1f(fTime_Uniform, fTime);
}

void CloudShaderLoader::LoadfCloudFactor_Uniform(GLfloat fCloudScale)
{
	UniformLoad1f(fCloudFactor_Uniform, fCloudScale);
}
void CloudShaderLoader::LoadfEarthRadius_Uniform(GLfloat EarthRadius)
{
	UniformLoad1f(fEarthRadius_Uniform, EarthRadius);
}
void CloudShaderLoader::LoadfCloudVolumetricCoverage_Uniform(GLfloat fCloudVolumetricCoverage)
{
	UniformLoad1f(fCloudVolumetricCoverage_Uniform, fCloudVolumetricCoverage);
}

void CloudShaderLoader::LoadfCloudHighAltitudeCoverage_Uniform(GLfloat fCloudHighAltitudeCoverage)
{
	UniformLoad1f(fCloudHighAltitudeCoverage_Uniform, fCloudHighAltitudeCoverage);
}
////////////////////////////////////////////////////////Vec3/////////////////////////////////////////////////////////////////////
void CloudShaderLoader::Loadv3CameraPos_Uniform(vmath::vec3 CameraPos)
{
	UniformLoadVec3(v3CameraPos_Uniform, CameraPos);
}
void CloudShaderLoader::Loadv3SunPos_Uniform(vmath::vec3 SunPos)
{
	UniformLoadVec3(v3SunPos_Uniform, SunPos);
}

void CloudShaderLoader::Loadv3SunColor_Uniform(vmath::vec3 SunColor)
{
	UniformLoadVec3(v3SunColor_Uniform, SunColor);
}

void CloudShaderLoader::Loadv2InnerOuterRadius_Uniform(vmath::vec2 InnerOuterRadius)
{
	UniformLoadVec2(v2InnerOuterRadius_Uniform, InnerOuterRadius);
}

void CloudShaderLoader::Loadv2Resolution_Uniform(vmath::vec2 Resolution)
{
	UniformLoadVec2(v2Resolution_Uniform, Resolution);
}

void CloudShaderLoader::LoadiPixelID_Uniform(GLint PixelID)
{
	UniformLoad1i(iPixelID_Uniform, PixelID);
}

void CloudShaderLoader::LoadiFrameID_Uniform(GLuint iFrameID)
{
	UniformLoad1ui(iFrameID_Uniform, iFrameID);
}

bool CloudShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_cs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void CloudShaderLoader::GetAllUniformLocations(void)
{
	Sampler3D_PerlinWorley3DUniform = GetUniformLocation("tSampler3D_PerlinWorley3D");
	Sampler3D_Worley32Uniform = GetUniformLocation("tSampler3D_Worley32");
	Sampler2D_WeatherMapUniform= GetUniformLocation("tSampler2D_WeatherMap");
	Sampler2D_WeatherMapTilableUniform = GetUniformLocation("tSampler2D_WeatherMapTilable");
	Sampler2D_BlueNoiseUniform = GetUniformLocation("tSampler2D_BlueNoise");
	Sampler2D_CurlNoiseUniform = GetUniformLocation("tSampler2D_CurlNoise");
	Sampler2D_BeerShadowMapUniform = GetUniformLocation("tSampler2D_BeerShadowMap");

	Sampler2D_CirrusCloudUniform = GetUniformLocation("tSampler2D_Cirrus");
	Sampler2D_CirroStratusCloudUniform = GetUniformLocation("tSampler2D_CirroStratus");
	Sampler2D_CirroCumulusCloudUniform = GetUniformLocation("tSampler2D_CirroCumulus");

	m4InvViewX_Uniform = GetUniformLocation("m4InvViewX");
	m4LightViewProjX_Uniform = GetUniformLocation("m4LightViewProjX");
	m4PrevViewX_Uniform= GetUniformLocation("m4PrevViewX");
	m4InvProjX_Uniform = GetUniformLocation("m4InvProjX");
	m4ProjX_Uniform = GetUniformLocation("m4ProjX");

	v3CameraPos_Uniform = GetUniformLocation("v3CameraPosition");
	v3SunPos_Uniform = GetUniformLocation("v3SunPos");
	v3SunColor_Uniform = GetUniformLocation("v3SunColor");

	v2Resolution_Uniform = GetUniformLocation("v2Resolution");
	v2InnerOuterRadius_Uniform = GetUniformLocation("v2InnerOuterRadius");

	fTime_Uniform = GetUniformLocation("fTime");
	fCloudFactor_Uniform = GetUniformLocation("fCloudFactor");
	fEarthRadius_Uniform = GetUniformLocation("fEarthRadius");
	fCloudVolumetricCoverage_Uniform = GetUniformLocation("fCloudVolumetricCoverage");
	fCloudHighAltitudeCoverage_Uniform = GetUniformLocation("fCloudHighAltitudeCoverage");

	iPixelID_Uniform=GetUniformLocation("iPixelID");
	iFrameID_Uniform= GetUniformLocation("iFrameID");
}

void CloudShaderLoader::SetAtmosphereShaderObject(GLuint AtmoshpereShaderID)
{
	SetExtraShader(AtmoshpereShaderID);
}