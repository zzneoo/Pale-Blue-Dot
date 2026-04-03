#include "TemporalPassShaderLoader.h"
#include"MyOpenGL.h"

TemporalPassShaderLoader::TemporalPassShaderLoader()
{
	Sampler2DCurrFrameNearest_Uniform = 0;
	Sampler2DCurrFrameLinear_Uniform = 0;
	Sampler2DPrevFrame_Uniform = 0;
	Sampler2DPrevFrameExtra_Uniform = 0;
	Sampler2DCurrFrameExtra_Uniform = 0;
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

TemporalPassShaderLoader::~TemporalPassShaderLoader()
{

}
//uniform samplers
void TemporalPassShaderLoader::LoadSampler2DCurrFrameNearest_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DCurrFrameNearest_Uniform, TEXTURE, tNo, textureID);
}
void TemporalPassShaderLoader::LoadSampler2DCurrFrameLinear_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DCurrFrameLinear_Uniform, TEXTURE, tNo, textureID);
}
//prev
void TemporalPassShaderLoader::LoadSampler2DPrevFrame_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DPrevFrame_Uniform, TEXTURE, tNo, textureID);
}
void TemporalPassShaderLoader::LoadSampler2DPrevFrameExtra_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DPrevFrameExtra_Uniform, TEXTURE, tNo, textureID);
}
//velocity
void TemporalPassShaderLoader::LoadSampler2DCurrVelocity_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DCurrVelocity_Uniform, TEXTURE, tNo, textureID);
}
//depth
void TemporalPassShaderLoader::LoadSampler2DCurrFrameExtra_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DCurrFrameExtra_Uniform, TEXTURE, tNo, textureID);
}

//uniform floats
void TemporalPassShaderLoader::LoadfTime_Uniform(GLfloat time)
{
	UniformLoad1f(fTime_Uniform, time);
}

void TemporalPassShaderLoader::LoadiBlockPixelID_Uniform(GLuint iBlockPixelID)
{
	UniformLoad1i(iBlockPixelID_Uniform, iBlockPixelID);
}

//uniform matrices
void TemporalPassShaderLoader::Loadm4ProjX_Uniform(vmath::mat4 ProjX)
{
	UniformLoadMatrix(m4ProjX_Uniform, ProjX);
}

void TemporalPassShaderLoader::Loadm4PrevViewX_Uniform(vmath::mat4 PrevViewX)
{
	UniformLoadMatrix(m4PrevViewX_Uniform, PrevViewX);
}

void TemporalPassShaderLoader::Loadm4ViewX_Uniform(vmath::mat4 ViewX)
{
	UniformLoadMatrix(m4ViewX_Uniform, ViewX);
}

//uniform vec3
void TemporalPassShaderLoader::Loadv3CameraPosition_Uniform(vmath::vec3 CameraPosition)
{
	UniformLoadVec3(v3CameraPosition_Uniform, CameraPosition);
}


bool TemporalPassShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs,filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void TemporalPassShaderLoader::GetAllUniformLocations(void)
{
	Sampler2DCurrFrameNearest_Uniform = GetUniformLocation("tSamplerCurrFrameNearest");
	Sampler2DCurrFrameLinear_Uniform = GetUniformLocation("tSamplerCurrFrameLinear");
	Sampler2DPrevFrame_Uniform = GetUniformLocation("tSamplerPrevFrame");
	Sampler2DPrevFrameExtra_Uniform = GetUniformLocation("tSamplerPrevExtra");
	Sampler2DCurrVelocity_Uniform = GetUniformLocation("tSamplerCurrVelocity");
	Sampler2DCurrFrameExtra_Uniform = GetUniformLocation("tSamplerCurrExtra");

	v3CameraPosition_Uniform = GetUniformLocation("v3CameraPosition");

	m4ProjX_Uniform = GetUniformLocation("m4ProjX");
	m4PrevViewX_Uniform = GetUniformLocation("m4PrevViewX");
	m4ViewX_Uniform = GetUniformLocation("m4ViewX");

	fTime_Uniform = GetUniformLocation("fTime");
	iBlockPixelID_Uniform = GetUniformLocation("iBlockPixelID");
}
