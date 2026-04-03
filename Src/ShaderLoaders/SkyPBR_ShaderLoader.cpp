#include "SkyPBR_ShaderLoader.h"
#include"MyOpenGL.h"

SkyPBR_ShaderLoader::SkyPBR_ShaderLoader(GLuint AtmosphereFragShaderID)
{
	//uniform locations
	//floats
	fFactorUniform = 0;
	fTimeUniform = 0;
	fExposureUniform = 0;

	//vectors
	v2SunSizeUniform = 0;
	v2CloudScaleVisibilityUniform = 0;
	v3SunPosUniform = 0;
	v3ColorPickerUniform = 0;
	v3WaterTintUniform = 0;
	v3EyePosUniform = 0;
	//Samplers
	Sampler2D_WorldSpacePosition = 0;
	Sampler2D_WorldSpaceNormal = 0;
	Sampler2D_AlbedoAO = 0;
	Sampler2D_SpecRoughAlphaDither = 0;
	Sampler2D_EarthAlbedoSpec = 0;
	Sampler2D_EarthLodBlendMap = 0;
	Sampler2D_EarthWaterNormalMap = 0;
	Sampler2D_BlueNoiseTextureUniform = 0;
	Sampler2D_EarthClouds = 0;
	Sampler2D_EarthNormal = 0;
	Sampler2D_WhirlNoiseTextureUniform = 0;
	Sampler2D_WhirlTextureUniform = 0;
	Sampler2D_DefferedVelocity = 0;
	Sampler2D_CloudShadowMap = 0;
	//PostProcess
	Sampler2D_SunGlareTextureUniform = 0;
	Sampler2D_CloudsTextureUniform = 0;

	//Matrices
	m4InvProjX_Uniform = 0;
	m4InvViewX_Uniform = 0;
	m4LightViewProjX_Uniform = 0;
	m4EarthRotationX_Uniform = 0;
	m4PrevViewProjX_Uniform = 0;
	m4EarthCloudRotationX_Uniform = 0;
	v3EarthCenterUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		SetAtmosphereFragShaderObject(AtmosphereFragShaderID);
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}
SkyPBR_ShaderLoader::~SkyPBR_ShaderLoader()
{
}

//floats
void SkyPBR_ShaderLoader::LoadFactorUniform(GLfloat factor)
{
	UniformLoad1f(fFactorUniform, factor);
}

void SkyPBR_ShaderLoader::LoadTimeUniform(GLfloat fTime)
{
	UniformLoad1f(fTimeUniform, fTime);
}

void SkyPBR_ShaderLoader::LoadExposureUniform(GLfloat Exposure)
{
	UniformLoad1f(fExposureUniform, Exposure);
}


//vectors-------------------------------------------------------------------------------------------------

void SkyPBR_ShaderLoader::LoadSunPositionUniform(vmath::vec3 sunPos)
{
	UniformLoadVec3(v3SunPosUniform,sunPos);
}
void SkyPBR_ShaderLoader::LoadEyePositionUniform(vmath::vec3 eyePos)
{
	UniformLoadVec3(v3EyePosUniform,eyePos);
}
void SkyPBR_ShaderLoader::LoadEarthCenterUniform(vmath::vec3 EarthCenter)
{
	UniformLoadVec3(v3EarthCenterUniform, EarthCenter);
}

void SkyPBR_ShaderLoader::LoadCloudScaleVisibilityUniform(vmath::vec2 CloudScale_Visibility)
{
	UniformLoadVec2(v2CloudScaleVisibilityUniform, CloudScale_Visibility);
}

void SkyPBR_ShaderLoader::LoadSunSizeUniform(vmath::vec2 sunSize) 
{
	UniformLoadVec2(v2SunSizeUniform, sunSize);
}

void SkyPBR_ShaderLoader::LoadColorPickerUniform(vmath::vec3 ColorPicker)
{
	UniformLoadVec3(v3ColorPickerUniform, ColorPicker);
}

void SkyPBR_ShaderLoader::LoadWaterTintUniform(vmath::vec3 WaterTint)
{
	UniformLoadVec3(v3WaterTintUniform, WaterTint);
}

//Matrices
void SkyPBR_ShaderLoader::LoadInvProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4InvProjX_Uniform, matrix);
}
void SkyPBR_ShaderLoader::LoadPrevViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4PrevViewProjX_Uniform, matrix);
}
void SkyPBR_ShaderLoader::Loadm4LightViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4LightViewProjX_Uniform, matrix);
}

void SkyPBR_ShaderLoader::LoadInvViewXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4InvViewX_Uniform, matrix);
}

void SkyPBR_ShaderLoader::LoadEarthRotationXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4EarthRotationX_Uniform, matrix);
}
void SkyPBR_ShaderLoader::LoadEarthCloudRotationXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4EarthCloudRotationX_Uniform, matrix);
}


//samplers------------------------------------------------------------------------------------------------
void SkyPBR_ShaderLoader::LoadWorldSpacePositionTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_WorldSpacePosition, TEXTURE, tNo, textureID);
}
void SkyPBR_ShaderLoader::LoadWorldSpaceNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_WorldSpaceNormal, TEXTURE, tNo, textureID);
}
void SkyPBR_ShaderLoader::LoadAlbedoAOTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_AlbedoAO, TEXTURE, tNo, textureID);
}
void SkyPBR_ShaderLoader::LoadSpecRoughAlphaDitherTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_SpecRoughAlphaDither, TEXTURE, tNo, textureID);
}
void SkyPBR_ShaderLoader::LoadDefferedVelocityTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_DefferedVelocity, TEXTURE, tNo, textureID);
}
void SkyPBR_ShaderLoader::LoadEarthAlbedoSpecTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_EarthAlbedoSpec, TEXTURE, tNo, textureID);
}
void SkyPBR_ShaderLoader::LoadEarthNormalTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_EarthNormal, TEXTURE, tNo, textureID);
}
void SkyPBR_ShaderLoader::LoadEarthCloudsTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_EarthClouds, TEXTURE, tNo, textureID);
}

void SkyPBR_ShaderLoader::LoadEarthLodBlendMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_EarthLodBlendMap, TEXTURE, tNo, textureID);
}
void SkyPBR_ShaderLoader::LoadEarthWaterNormalUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_EarthWaterNormalMap, TEXTURE, tNo, textureID);
}
void SkyPBR_ShaderLoader::LoadSunGlareTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_SunGlareTextureUniform, TEXTURE, tNo, textureID);
}

void SkyPBR_ShaderLoader::LoadCloudsTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_CloudsTextureUniform, TEXTURE, tNo, textureID);
}
void SkyPBR_ShaderLoader::LoadWhirlNoiseTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_WhirlNoiseTextureUniform, TEXTURE, tNo, textureID);
}
void SkyPBR_ShaderLoader::LoadWhirlTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_WhirlTextureUniform, TEXTURE, tNo, textureID);
}
void SkyPBR_ShaderLoader::LoadBlueNoiseTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_BlueNoiseTextureUniform, TEXTURE, tNo, textureID);
}

void SkyPBR_ShaderLoader::LoadCloudShadowMapUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_CloudShadowMap, TEXTURE, tNo, textureID);
}

//----------------------------------------------------------------------------------------------------------

bool SkyPBR_ShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void SkyPBR_ShaderLoader::GetAllUniformLocations(void)
{

	//floats
	fFactorUniform = GetUniformLocation("fFactor");
	fTimeUniform = GetUniformLocation("fTime");
	fExposureUniform = GetUniformLocation("fExposure");
	//Matrices
	m4InvProjX_Uniform = GetUniformLocation("m4InvProjX");
	m4InvViewX_Uniform = GetUniformLocation("m4InvViewX");
	m4LightViewProjX_Uniform = GetUniformLocation("m4LightViewProjX");
	m4PrevViewProjX_Uniform = GetUniformLocation("m4PrevViewProjX");
	m4EarthRotationX_Uniform = GetUniformLocation("m4EarthRotationX");
	m4EarthCloudRotationX_Uniform = GetUniformLocation("m4EarthCloudRotationX");
	//vectors
	v2SunSizeUniform = GetUniformLocation("v2SunSize"); 
	v2CloudScaleVisibilityUniform = GetUniformLocation("v2CloudScaleVisibility");
	v3SunPosUniform = GetUniformLocation("v3SunPos");
	v3ColorPickerUniform = GetUniformLocation("v3ColorPicker");
	v3WaterTintUniform = GetUniformLocation("v3WaterTint");
	v3EyePosUniform = GetUniformLocation("v3EyePos");
	v3EarthCenterUniform = GetUniformLocation("v3EarthCenter");
	//samplers
	Sampler2D_WorldSpacePosition = GetUniformLocation("Sampler2D_WorldSpacePosition");
	Sampler2D_WorldSpaceNormal = GetUniformLocation("Sampler2D_WorldSpaceNormal");
	Sampler2D_AlbedoAO = GetUniformLocation("Sampler2D_AlbedoAO");
	Sampler2D_SpecRoughAlphaDither = GetUniformLocation("Sampler2D_SpecRoughAlphaDither");
	Sampler2D_DefferedVelocity = GetUniformLocation("Sampler2D_DefferedVelocity");

	Sampler2D_CloudShadowMap = GetUniformLocation("Sampler2D_CloudShadowMap");

	Sampler2D_EarthAlbedoSpec = GetUniformLocation("Sampler2D_EarthAlbedoSpec");
	Sampler2D_EarthNormal = GetUniformLocation("Sampler2D_EarthNormal");
	Sampler2D_EarthClouds = GetUniformLocation("Sampler2D_EarthClouds");

	Sampler2D_EarthLodBlendMap = GetUniformLocation("Sampler2D_EarthLodBlendMap");
	Sampler2D_EarthWaterNormalMap = GetUniformLocation("Sampler2D_EarthWaterNormal");
	//PostProcess
	Sampler2D_SunGlareTextureUniform = GetUniformLocation("Sampler2D_SunGlare");
	Sampler2D_CloudsTextureUniform = GetUniformLocation("Sampler2D_Clouds");
	Sampler2D_WhirlNoiseTextureUniform = GetUniformLocation("Sampler2D_WhirlNoise");
	Sampler2D_BlueNoiseTextureUniform = GetUniformLocation("Sampler2D_BlueNoise");
	Sampler2D_WhirlTextureUniform = GetUniformLocation("Sampler2D_WhirlTexture");

}

void SkyPBR_ShaderLoader::SetAtmosphereFragShaderObject(GLuint AtmoshpereShaderID)
{
	SetExtraShader(AtmoshpereShaderID);
}
