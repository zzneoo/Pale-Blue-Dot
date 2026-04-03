#include "BasicSkyPBR_ShaderLoader.h"
#include"MyOpenGL.h"

BasicSkyPBR_ShaderLoader::BasicSkyPBR_ShaderLoader(GLuint AtmosphereFragShaderID)
{
	//uniform locations
	//floats
	fArtistFactor_Uniform = 0;
	fExposure_Uniform = 0;
	fTime_Uniform = 0;

	//vectors
	v2SunSize_Uniform = 0;
	v3SunPos_Uniform = 0;
	v3ColorPicker_Uniform = 0;
	v3EyePos_Uniform = 0;
	//Samplers
	Sampler2D_WorldSpacePosition = 0;
	Sampler2D_WorldSpaceNormal = 0;
	Sampler2D_AlbedoAO = 0;
	Sampler2D_SpecRoughAlphaDither = 0;
	Sampler2D_DefferedVelocity = 0;

	//PostProcess
	Sampler2D_CloudShadowMap = 0;
	Sampler2D_CloudsTexture_Uniform = 0;
	Sampler2D_BlueNoiseTexture_Uniform = 0;
	Sampler3D_FroxelTransmittance_Uniform = 0;
	Sampler3D_FroxelInScatter_Uniform = 0;
	//Matrices
	m4InvProjX_Uniform = 0;
	m4InvViewX_Uniform = 0;
	m4LightViewProjX_Uniform = 0;
	m4PrevViewProjX_Uniform = 0;
	v3EarthCenter_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		SetAtmosphereFragShaderObject(AtmosphereFragShaderID);
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}
BasicSkyPBR_ShaderLoader::~BasicSkyPBR_ShaderLoader()
{
}

//floats
void BasicSkyPBR_ShaderLoader::LoadArtistFactor_Uniform(GLfloat factor)
{
	UniformLoad1f(fArtistFactor_Uniform, factor);
}

void BasicSkyPBR_ShaderLoader::LoadExposure_Uniform(GLfloat Exposure)
{
	UniformLoad1f(fExposure_Uniform, Exposure);
}

void BasicSkyPBR_ShaderLoader::LoadTime_Uniform(GLfloat fTime)
{
	UniformLoad1f(fTime_Uniform, fTime);
}


//vectors-------------------------------------------------------------------------------------------------

void BasicSkyPBR_ShaderLoader::LoadSunPosition_Uniform(vmath::vec3 sunPos)
{
	UniformLoadVec3(v3SunPos_Uniform,sunPos);
}
void BasicSkyPBR_ShaderLoader::LoadEyePosition_Uniform(vmath::vec3 eyePos)
{
	UniformLoadVec3(v3EyePos_Uniform,eyePos);
}
void BasicSkyPBR_ShaderLoader::LoadEarthCenter_Uniform(vmath::vec3 EarthCenter)
{
	UniformLoadVec3(v3EarthCenter_Uniform, EarthCenter);
}

void BasicSkyPBR_ShaderLoader::LoadSunSize_Uniform(vmath::vec2 sunSize) 
{
	UniformLoadVec2(v2SunSize_Uniform, sunSize);
}

void BasicSkyPBR_ShaderLoader::LoadColorPicker_Uniform(vmath::vec3 CamRight)
{
	UniformLoadVec3(v3ColorPicker_Uniform, CamRight);
}

//Matrices
void BasicSkyPBR_ShaderLoader::LoadInvProjX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4InvProjX_Uniform, matrix);
}
void BasicSkyPBR_ShaderLoader::LoadPrevViewProjX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4PrevViewProjX_Uniform, matrix);
}
void BasicSkyPBR_ShaderLoader::Loadm4LightViewProjX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4LightViewProjX_Uniform, matrix);
}

void BasicSkyPBR_ShaderLoader::LoadInvViewX_Uniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4InvViewX_Uniform, matrix);
}


//samplers------------------------------------------------------------------------------------------------
void BasicSkyPBR_ShaderLoader::LoadWorldSpacePositionTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_WorldSpacePosition, TEXTURE, tNo, textureID);
}
void BasicSkyPBR_ShaderLoader::LoadWorldSpaceNormalTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_WorldSpaceNormal, TEXTURE, tNo, textureID);
}
void BasicSkyPBR_ShaderLoader::LoadAlbedoAOTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_AlbedoAO, TEXTURE, tNo, textureID);
}
void BasicSkyPBR_ShaderLoader::LoadSpecRoughAlphaDitherTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_SpecRoughAlphaDither, TEXTURE, tNo, textureID);
}
void BasicSkyPBR_ShaderLoader::LoadDefferedVelocityTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_DefferedVelocity, TEXTURE, tNo, textureID);
}

void BasicSkyPBR_ShaderLoader::LoadCloudsTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_CloudsTexture_Uniform, TEXTURE, tNo, textureID);
}

void BasicSkyPBR_ShaderLoader::LoadBlueNoiseTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_BlueNoiseTexture_Uniform, TEXTURE, tNo, textureID);
}

void BasicSkyPBR_ShaderLoader::LoadCloudShadowMap_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_CloudShadowMap, TEXTURE, tNo, textureID);
}

//3D
void BasicSkyPBR_ShaderLoader::LoadFroxelTransmittance_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture3D(Sampler3D_FroxelTransmittance_Uniform, TEXTURE, tNo, textureID);
}

void BasicSkyPBR_ShaderLoader::LoadFroxelInScatter_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture3D(Sampler3D_FroxelInScatter_Uniform, TEXTURE, tNo, textureID);
}

//----------------------------------------------------------------------------------------------------------

bool BasicSkyPBR_ShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void BasicSkyPBR_ShaderLoader::GetAllUniformLocations(void)
{

	//floats
	fArtistFactor_Uniform = GetUniformLocation("fArtistFactor");
	fExposure_Uniform = GetUniformLocation("fExposure");
	fTime_Uniform = GetUniformLocation("fTime");
	//Matrices
	m4InvProjX_Uniform = GetUniformLocation("m4InvProjX");
	m4InvViewX_Uniform = GetUniformLocation("m4InvViewX");
	m4LightViewProjX_Uniform = GetUniformLocation("m4LightViewProjX");
	m4PrevViewProjX_Uniform = GetUniformLocation("m4PrevViewProjX");

	//vectors
	v2SunSize_Uniform = GetUniformLocation("v2SunSize"); 
	v3SunPos_Uniform = GetUniformLocation("v3SunPos");
	v3ColorPicker_Uniform = GetUniformLocation("v3ColorPicker");
	v3EyePos_Uniform = GetUniformLocation("v3EyePos");
	v3EarthCenter_Uniform = GetUniformLocation("v3EarthCenter");
	//samplers
	Sampler2D_WorldSpacePosition = GetUniformLocation("Sampler2D_WorldSpacePosition");
	Sampler2D_WorldSpaceNormal = GetUniformLocation("Sampler2D_WorldSpaceNormal");
	Sampler2D_AlbedoAO = GetUniformLocation("Sampler2D_AlbedoAO");
	Sampler2D_SpecRoughAlphaDither = GetUniformLocation("Sampler2D_SpecRoughAlphaDither");
	Sampler2D_DefferedVelocity = GetUniformLocation("Sampler2D_DefferedVelocity");

	//PostProcess
	Sampler2D_CloudShadowMap = GetUniformLocation("Sampler2D_CloudShadowMap");
	Sampler2D_CloudsTexture_Uniform = GetUniformLocation("Sampler2D_Clouds");
	Sampler2D_BlueNoiseTexture_Uniform = GetUniformLocation("Sampler2D_BlueNoise");
	Sampler3D_FroxelTransmittance_Uniform = GetUniformLocation("Sampler3D_FroxelTransmittance");
	Sampler3D_FroxelInScatter_Uniform = GetUniformLocation("Sampler3D_FroxelInScatter");


}

void BasicSkyPBR_ShaderLoader::SetAtmosphereFragShaderObject(GLuint AtmoshpereShaderID)
{
	SetExtraShader(AtmoshpereShaderID);
}
