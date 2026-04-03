#include "GodRaysShaderLoader.h"
#include"MyOpenGL.h"

GodRaysShaderLoader::GodRaysShaderLoader()
{
	//samplers
	Sampler2D_LightAreaUniform = 0;
	Sampler2D_BlueNoiseUniform = 0;
	//vectors
	v2Resolution_Uniform = 0;
	v2ScreenSunPos_Uniform = 0;
	//float
	fTime_Uniform = 0;
	//uint
	iFrameID_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}

GodRaysShaderLoader::~GodRaysShaderLoader()
{

}

bool GodRaysShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_cs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

//samplers
void GodRaysShaderLoader::LoadSampler2D_LightAreaUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_LightAreaUniform, TEXTURE, tNo, textureID);
}
void GodRaysShaderLoader::LoadSampler2D_BlueNoiseUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_BlueNoiseUniform, TEXTURE, tNo, textureID);
}

//vectors
void GodRaysShaderLoader::Loadv2Resolution_Uniform(vmath::vec2 resolution)
{
	UniformLoadVec2(v2Resolution_Uniform, resolution);
}
void GodRaysShaderLoader::Loadv2ScreenSunPos_Uniform(vmath::vec2 ScreenSunPos)
{
	UniformLoadVec2(v2ScreenSunPos_Uniform, ScreenSunPos);
}

//float
void GodRaysShaderLoader::LoadfTime_Uniform(GLfloat fTime)
{
	UniformLoad1f(fTime_Uniform, fTime);
}
//uint
void GodRaysShaderLoader::LoadiFrameID_Uniform(GLuint iFrameID)
{
	UniformLoad1ui(iFrameID_Uniform, iFrameID);
}

void GodRaysShaderLoader::GetAllUniformLocations(void)
{
	//samplers
	Sampler2D_LightAreaUniform = GetUniformLocation("tSampler2D_LightArea");
	Sampler2D_BlueNoiseUniform = GetUniformLocation("tSampler2D_BlueNoise");
	//vectors
	v2Resolution_Uniform = GetUniformLocation("v2Resolution");
	v2ScreenSunPos_Uniform = GetUniformLocation("v2ScreenSunPos");
	//float
	fTime_Uniform = GetUniformLocation("fTime");
	iFrameID_Uniform = GetUniformLocation("iFrameID");
}
