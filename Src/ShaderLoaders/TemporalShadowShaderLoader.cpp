#include "TemporalShadowShaderLoader.h"
#include"MyOpenGL.h"

TemporalShadowShaderLoader::TemporalShadowShaderLoader()
{
	Sampler2DCurrFrameNearest_Uniform = 0;
	Sampler2DPrevFrame_Uniform = 0;
	Sampler2DCurrVelocity_Uniform = 0;


	m4ViewX_Uniform = 0;
	m4ProjX_Uniform = 0;
	m4PrevViewX_Uniform = 0;
	v3CameraPosition_Uniform = 0;

	iBlockPixelID_Uniform = 0;
	fTime_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}

TemporalShadowShaderLoader::~TemporalShadowShaderLoader()
{

}
//uniform samplers
void TemporalShadowShaderLoader::LoadSampler2DCurrFrameNearest_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DCurrFrameNearest_Uniform, TEXTURE, tNo, textureID);
}

//prev
void TemporalShadowShaderLoader::LoadSampler2DPrevFrame_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DPrevFrame_Uniform, TEXTURE, tNo, textureID);
}

//velocity
void TemporalShadowShaderLoader::LoadSampler2DCurrVelocity_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DCurrVelocity_Uniform, TEXTURE, tNo, textureID);
}

//uniform floats
void TemporalShadowShaderLoader::LoadfTime_Uniform(GLfloat time)
{
	UniformLoad1f(fTime_Uniform, time);
}

void TemporalShadowShaderLoader::LoadiBlockPixelID_Uniform(GLuint iBlockPixelID)
{
	UniformLoad1i(iBlockPixelID_Uniform, iBlockPixelID);
}

//uniform matrices
void TemporalShadowShaderLoader::Loadm4ProjX_Uniform(vmath::mat4 ProjX)
{
	UniformLoadMatrix(m4ProjX_Uniform, ProjX);
}

void TemporalShadowShaderLoader::Loadm4PrevViewX_Uniform(vmath::mat4 PrevViewX)
{
	UniformLoadMatrix(m4PrevViewX_Uniform, PrevViewX);
}

void TemporalShadowShaderLoader::Loadm4ViewX_Uniform(vmath::mat4 ViewX)
{
	UniformLoadMatrix(m4ViewX_Uniform, ViewX);
}

//uniform vec3
void TemporalShadowShaderLoader::Loadv3CameraPosition_Uniform(vmath::vec3 CameraPosition)
{
	UniformLoadVec3(v3CameraPosition_Uniform, CameraPosition);
}


bool TemporalShadowShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs,filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void TemporalShadowShaderLoader::GetAllUniformLocations(void)
{
	Sampler2DCurrFrameNearest_Uniform = GetUniformLocation("tSamplerCurrFrameNearest");
	Sampler2DPrevFrame_Uniform = GetUniformLocation("tSamplerPrevFrame");
	Sampler2DCurrVelocity_Uniform = GetUniformLocation("tSamplerCurrVelocity");


	v3CameraPosition_Uniform = GetUniformLocation("v3CameraPosition");

	m4ProjX_Uniform = GetUniformLocation("m4ProjX");
	m4PrevViewX_Uniform = GetUniformLocation("m4PrevViewX");
	m4ViewX_Uniform = GetUniformLocation("m4ViewX");

	fTime_Uniform = GetUniformLocation("fTime");
	iBlockPixelID_Uniform = GetUniformLocation("iBlockPixelID");
}
