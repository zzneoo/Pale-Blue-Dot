#include "SkyDomeShaderLoader.h"
#include "MyOpenGL.h"

SkyDomeShaderLoader::SkyDomeShaderLoader()
{
	//uniforms
	m4ModelViewProjX_Uniform = 0;
	v3CameraPos_Uniform = 0;

	fOuterRadius_Uniform = 0;
	fInnerRadius_Uniform = 0;

	fScale_Uniform = 0;
	fScaleDepth_Uniform = 0;
	fScaleOverScaleDepth_Uniform = 0;

	fKrESun_Uniform = 0;
	fKmESun_Uniform = 0;

	fKr4PI_Uniform = 0;
	fKm4PI_Uniform = 0;
	v3InvWavelength_Uniform = 0;

	//Varying Scattering Uniforms
	v3LightPos_Uniform = 0;
	fSkyFactor_Uniform = 0;
	iFrameID_Uniform = 0;
	//sampler uniform
	tSamplerBlueNoise = 0;

	
	//Initialize

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}


SkyDomeShaderLoader::~SkyDomeShaderLoader()
{
}


bool SkyDomeShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();

	return true;
}

void SkyDomeShaderLoader::LoadModelViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelViewProjX_Uniform, matrix);
}

void SkyDomeShaderLoader::LoadCameraPosUniform(vmath::vec3 cameraPos)
{
	UniformLoadVec3(v3CameraPos_Uniform, cameraPos);
}

void SkyDomeShaderLoader::LoadLightPosUniform(vmath::vec3 lightPos)
{
	UniformLoadVec3(v3LightPos_Uniform, lightPos);
}

void  SkyDomeShaderLoader::LoadiFrameID_Uniform(GLuint iFrameID)
{
	UniformLoad1ui(iFrameID_Uniform, iFrameID);
}

void SkyDomeShaderLoader::LoadfSkyFactor_Uniform(GLfloat fSkyFactor)
{
	UniformLoad1f(fSkyFactor_Uniform, fSkyFactor);
}

void SkyDomeShaderLoader::LoadInverseWavelengthUniform(vmath::vec3 inverseWavelength)
{
	UniformLoadVec3(v3InvWavelength_Uniform, inverseWavelength);
}

void SkyDomeShaderLoader::LoadOuterRadiusUniform(GLfloat OuterRadius)
{
	UniformLoad1f(fOuterRadius_Uniform, OuterRadius);
}

void SkyDomeShaderLoader::LoadInnerRadiusUniform(GLfloat InnerRadius)
{
	UniformLoad1f(fInnerRadius_Uniform, InnerRadius);
}

void  SkyDomeShaderLoader::LoadScaleUniform(GLfloat Scale)
{
	UniformLoad1f(fScale_Uniform, Scale);
}
void  SkyDomeShaderLoader::LoadScaleDepthUniform(GLfloat ScaleDepth)
{
	UniformLoad1f(fScaleDepth_Uniform, ScaleDepth);
}
void  SkyDomeShaderLoader::LoadScaleOverScaleDepthUniform(GLfloat ScaleOverScaleDepth)
{
	UniformLoad1f(fScaleOverScaleDepth_Uniform, ScaleOverScaleDepth);
}
void  SkyDomeShaderLoader::LoadKrESunUniform(GLfloat KrESun)
{
	UniformLoad1f(fKrESun_Uniform, KrESun);
}
void  SkyDomeShaderLoader::LoadKmESunUniform(GLfloat KmESun)
{
	UniformLoad1f(fKmESun_Uniform, KmESun);
}

void  SkyDomeShaderLoader::LoadKr4PIUniform(GLfloat Kr4PI)
{
	UniformLoad1f(fKr4PI_Uniform, Kr4PI);
}
void  SkyDomeShaderLoader::LoadKm4PIUniform(GLfloat Km4PI)
{
	UniformLoad1f(fKm4PI_Uniform, Km4PI);
}

//Texture Samplers
void  SkyDomeShaderLoader::LoadTextureID_BlueNoiseUniform(GLenum TEXTURE, GLuint tNo, GLuint  TextureID_BlueNoise)
{
	UniformLoadTexture2D(tSamplerBlueNoise, TEXTURE, tNo, TextureID_BlueNoise);
}



void SkyDomeShaderLoader::GetAllUniformLocations(void)
{
	//Varying Scattering Uniforms
	v3LightPos_Uniform = GetUniformLocation("v3LightPos");
	m4ModelViewProjX_Uniform = GetUniformLocation("m4ModelViewProjX");
	v3CameraPos_Uniform = GetUniformLocation("v3CameraPos");
	v3InvWavelength_Uniform = GetUniformLocation("v3InvWavelength");

	iFrameID_Uniform = GetUniformLocation("iFrameID");
	fSkyFactor_Uniform= GetUniformLocation("fSkyFactor");

	//const uniforms
	fOuterRadius_Uniform = GetUniformLocation("fOuterRadius");
	fInnerRadius_Uniform = GetUniformLocation("fInnerRadius");

	fScale_Uniform = GetUniformLocation("fScale");
	fScaleDepth_Uniform = GetUniformLocation("fScaleDepth");
	fScaleOverScaleDepth_Uniform = GetUniformLocation("fScaleOverScaleDepth");

	fKrESun_Uniform = GetUniformLocation("fKrESun");
	fKmESun_Uniform = GetUniformLocation("fKmESun");

	fKr4PI_Uniform = GetUniformLocation("fKr4PI");
	fKm4PI_Uniform = GetUniformLocation("fKm4PI");

	tSamplerBlueNoise= GetUniformLocation("tSampler2D_BlueNoise");
}