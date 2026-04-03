#include "SkyDistant_ShaderLoader.h"
#include"MyOpenGL.h"

SkyDistant_ShaderLoader::SkyDistant_ShaderLoader(GLuint AtmosphereFragShaderID)
{
	//uniform locations
	//floats
	fArtistFactor_Uniform = 0;
	fExposure_Uniform = 0;

	//vectors
	v2SunSize_Uniform = 0;
	v3SunPos_Uniform = 0;
	v3EyePos_Uniform = 0;
	v2NearFar_Uniform = 0;

	//PostProcess
	Sampler2D_BlueNoiseTexture_Uniform = 0;
	//Matrices
	m4InvProjX_Uniform = 0;
	m4InvViewX_Uniform = 0;
	v3EarthCenter_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		SetAtmosphereFragShaderObject(AtmosphereFragShaderID);
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}
SkyDistant_ShaderLoader::~SkyDistant_ShaderLoader()
{
}

//floats
void SkyDistant_ShaderLoader::LoadArtistFactor_Uniform(GLfloat factor)
{
	UniformLoad1f(fArtistFactor_Uniform, factor);
}
void SkyDistant_ShaderLoader::LoadExposure_Uniform(GLfloat Exposure)
{
	UniformLoad1f(fExposure_Uniform, Exposure);
}


//vectors-------------------------------------------------------------------------------------------------

void SkyDistant_ShaderLoader::LoadSunPosition_Uniform(vmath::vec3 sunPos)
{
	UniformLoadVec3(v3SunPos_Uniform,sunPos);
}
void SkyDistant_ShaderLoader::LoadEyePosition_Uniform(vmath::vec3 eyePos)
{
	UniformLoadVec3(v3EyePos_Uniform,eyePos);
}
void SkyDistant_ShaderLoader::LoadEarthCenter_Uniform(vmath::vec3 EarthCenter)
{
	UniformLoadVec3(v3EarthCenter_Uniform, EarthCenter);
}

void SkyDistant_ShaderLoader::LoadSunSize_Uniform(vmath::vec2 sunSize)
{
	UniformLoadVec2(v2SunSize_Uniform, sunSize);
}

void SkyDistant_ShaderLoader::LoadNearFar_Uniform(vmath::vec2 NearFar)
{
	UniformLoadVec2(v2NearFar_Uniform, NearFar);
}


//Matrices
void SkyDistant_ShaderLoader::LoadInvProjX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4InvProjX_Uniform, matrix);
}


void SkyDistant_ShaderLoader::LoadInvViewX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4InvViewX_Uniform, matrix);
}


void SkyDistant_ShaderLoader::LoadBlueNoiseTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_BlueNoiseTexture_Uniform, TEXTURE, tNo, textureID);
}

//----------------------------------------------------------------------------------------------------------

bool SkyDistant_ShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs,filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void SkyDistant_ShaderLoader::GetAllUniformLocations(void)
{

	//floats
	fArtistFactor_Uniform = GetUniformLocation("fArtistFactor");
	fExposure_Uniform = GetUniformLocation("fExposure");
	//Matrices
	m4InvProjX_Uniform = GetUniformLocation("m4InvProjX");
	m4InvViewX_Uniform = GetUniformLocation("m4InvViewX");

	//vectors
	v2SunSize_Uniform = GetUniformLocation("v2SunSize"); 
	v2NearFar_Uniform = GetUniformLocation("v2NearFar");

	v3SunPos_Uniform = GetUniformLocation("v3SunPos");
	v3EyePos_Uniform = GetUniformLocation("v3EyePos");
	v3EarthCenter_Uniform = GetUniformLocation("v3EarthCenter");

	//PostProcess
	Sampler2D_BlueNoiseTexture_Uniform = GetUniformLocation("Sampler2D_BlueNoise");


}

void SkyDistant_ShaderLoader::SetAtmosphereFragShaderObject(GLuint AtmoshpereShaderID)
{
	SetExtraShader(AtmoshpereShaderID);
}
