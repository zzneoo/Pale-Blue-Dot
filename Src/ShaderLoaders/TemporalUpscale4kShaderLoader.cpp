#include "TemporalUpscale4kShaderLoader.h"
#include"MyOpenGL.h"

TemporalUpscale4kShaderLoader::TemporalUpscale4kShaderLoader()
{

	Sampler2DCurrFrameLinear_Uniform = 0;
	Sampler2DPrevFrame_Uniform = 0;
	Sampler2DCurrVelocity_Uniform = 0;

	iBlockPixelID_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}

TemporalUpscale4kShaderLoader::~TemporalUpscale4kShaderLoader()
{

}
//uniform samplers

void TemporalUpscale4kShaderLoader::LoadSampler2DCurrFrameLinear_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DCurrFrameLinear_Uniform, TEXTURE, tNo, textureID);
}
//prev
void TemporalUpscale4kShaderLoader::LoadSampler2DPrevFrame_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DPrevFrame_Uniform, TEXTURE, tNo, textureID);
}

//velocity
void TemporalUpscale4kShaderLoader::LoadSampler2DCurrVelocity_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DCurrVelocity_Uniform, TEXTURE, tNo, textureID);
}

void TemporalUpscale4kShaderLoader::LoadiBlockPixelID_Uniform(GLuint iBlockPixelID)
{
	UniformLoad1i(iBlockPixelID_Uniform, iBlockPixelID);
}
void TemporalUpscale4kShaderLoader::LoadiBlockWidth_Uniform(GLuint iBlockWidth)
{
	UniformLoad1i(iBlockWidth_Uniform, iBlockWidth);
}
void TemporalUpscale4kShaderLoader::Loadv2Resolution_Uniform(vmath::vec2 Resolution)
{
	UniformLoadVec2(v2Resolution_Uniform, Resolution);
}



bool TemporalUpscale4kShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs,filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void TemporalUpscale4kShaderLoader::GetAllUniformLocations(void)
{
	//Sampler2DCurrFrameNearest_Uniform = GetUniformLocation("tSamplerCurrFrameNearest");
	Sampler2DCurrFrameLinear_Uniform = GetUniformLocation("tSamplerCurrFrameLinear");
	Sampler2DPrevFrame_Uniform = GetUniformLocation("tSamplerPrevFrame");
	//Sampler2DPrevDepth_Uniform = GetUniformLocation("tSamplerPrevDepth");
	Sampler2DCurrVelocity_Uniform = GetUniformLocation("tSamplerCurrVelocity");
	//Sampler2DCurrDepth_Uniform = GetUniformLocation("tSamplerCurrDepth");

	//v3CameraPosition_Uniform = GetUniformLocation("v3CameraPosition");

	//m4ProjX_Uniform = GetUniformLocation("m4ProjX");
	//m4PrevViewX_Uniform = GetUniformLocation("m4PrevViewX");
	//m4ViewX_Uniform = GetUniformLocation("m4ViewX");

	//fTime_Uniform = GetUniformLocation("fTime");
	iBlockPixelID_Uniform = GetUniformLocation("iBlockPixelID");
	iBlockWidth_Uniform = GetUniformLocation("iBlockWidth");
	v2Resolution_Uniform = GetUniformLocation("v2Resolution");
}
