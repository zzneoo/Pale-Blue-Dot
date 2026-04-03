#include "TemporalAntiAliasingShaderLoader.h"
#include"MyOpenGL.h"

TemporalAntiAliasingShaderLoader::TemporalAntiAliasingShaderLoader()
{
	Sampler2DCurrFrameNearest_Uniform = 0;
	//Sampler2DCurrFrameLinear_Uniform = 0;
	Sampler2DPrevFrame_Uniform = 0;
	//Sampler2DPrevDepth_Uniform = 0;
	Sampler2DCurrDepth_Uniform = 0;
	Sampler2DCurrVelocity_Uniform = 0;


	m4ViewX_Uniform = 0;
	m4ProjX_Uniform = 0;
	m4PrevViewX_Uniform = 0;
	v3CameraPosition_Uniform = 0;

	iBlockPixelID_Uniform = 0;
	v4Halton23_Uniform = 0;
	fTime_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}

TemporalAntiAliasingShaderLoader::~TemporalAntiAliasingShaderLoader()
{

}
//uniform samplers
void TemporalAntiAliasingShaderLoader::LoadSampler2DCurrFrameNearest_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DCurrFrameNearest_Uniform, TEXTURE, tNo, textureID);
}

//prev
void TemporalAntiAliasingShaderLoader::LoadSampler2DPrevFrame_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DPrevFrame_Uniform, TEXTURE, tNo, textureID);
}

//velocity
void TemporalAntiAliasingShaderLoader::LoadSampler2DCurrVelocity_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DCurrVelocity_Uniform, TEXTURE, tNo, textureID);
}
//depth
void TemporalAntiAliasingShaderLoader::LoadSampler2DCurrDepth_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DCurrDepth_Uniform, TEXTURE, tNo, textureID);
}

//uniform floats
void TemporalAntiAliasingShaderLoader::LoadfTime_Uniform(GLfloat time)
{
	UniformLoad1f(fTime_Uniform, time);
}

//bool
void TemporalAntiAliasingShaderLoader::LoadbIsTAA_Uniform(bool isTAA)
{
	UniformLoad1ui(bIsTAA_Uniform, isTAA);
}
//vec2
void TemporalAntiAliasingShaderLoader::Loadv4Halton23_Uniform(vmath::vec4 CurrPrevHalton23)
{
	UniformLoadVec4(v4Halton23_Uniform, CurrPrevHalton23);
}

void TemporalAntiAliasingShaderLoader::LoadiBlockPixelID_Uniform(GLuint iBlockPixelID)
{
	UniformLoad1i(iBlockPixelID_Uniform, iBlockPixelID);
}

//uniform matrices
void TemporalAntiAliasingShaderLoader::Loadm4ProjX_Uniform(vmath::mat4 ProjX)
{
	UniformLoadMatrix(m4ProjX_Uniform, ProjX);
}

void TemporalAntiAliasingShaderLoader::Loadm4PrevViewX_Uniform(vmath::mat4 PrevViewX)
{
	UniformLoadMatrix(m4PrevViewX_Uniform, PrevViewX);
}

void TemporalAntiAliasingShaderLoader::Loadm4ViewX_Uniform(vmath::mat4 ViewX)
{
	UniformLoadMatrix(m4ViewX_Uniform, ViewX);
}

//uniform vec3
void TemporalAntiAliasingShaderLoader::Loadv3CameraPosition_Uniform(vmath::vec3 CameraPosition)
{
	UniformLoadVec3(v3CameraPosition_Uniform, CameraPosition);
}


bool TemporalAntiAliasingShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs,filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void TemporalAntiAliasingShaderLoader::GetAllUniformLocations(void)
{
	Sampler2DCurrFrameNearest_Uniform = GetUniformLocation("tSamplerCurrFrameNearest");
//	Sampler2DCurrFrameLinear_Uniform = GetUniformLocation("tSamplerCurrFrameLinear");
	Sampler2DPrevFrame_Uniform = GetUniformLocation("tSamplerPrevFrame");
//	Sampler2DPrevDepth_Uniform = GetUniformLocation("tSamplerPrevDepth");
	Sampler2DCurrVelocity_Uniform = GetUniformLocation("tSamplerCurrVelocity");
	Sampler2DCurrDepth_Uniform = GetUniformLocation("tSamplerCurrDepth");

	v3CameraPosition_Uniform = GetUniformLocation("v3CameraPosition");

	m4ProjX_Uniform = GetUniformLocation("m4ProjX");
	m4PrevViewX_Uniform = GetUniformLocation("m4PrevViewX");
	m4ViewX_Uniform = GetUniformLocation("m4ViewX");

	bIsTAA_Uniform = GetUniformLocation("bIsTAA");
	fTime_Uniform = GetUniformLocation("fTime");
	v4Halton23_Uniform = GetUniformLocation("v4Halton23");
	iBlockPixelID_Uniform = GetUniformLocation("iBlockPixelID");
}
