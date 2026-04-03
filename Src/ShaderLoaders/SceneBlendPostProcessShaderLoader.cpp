#include "SceneBlendPostProcessShaderLoader.h"
#include"MyOpenGL.h"

SceneBlendPostProcessShaderLoader::SceneBlendPostProcessShaderLoader()
{
	//extra
	v3SunPos_Uniform = 0;
	v3EyePos_Uniform = 0;
	v3EyeDir_Uniform = 0;
	//matrices
	m4InvProjX_Uniform = 0;
	m4InvViewX_Uniform = 0;
	m4LightSpaceX_Uniform = 0;
	//scene
	Sampler2D_CloudTextureUniform = 0;
	Sampler2D_SkyTextureUniform = 0;
	Sampler2D_SceneTextureUniform = 0;
	Sampler2D_SceneMaskTextureUniform = 0;
	//postProcess
	Sampler2D_GodRaysUniform = 0;
	//extra
	Sampler2D_ShadowMapTextureUniform = 0;
	Sampler2D_DepthTextureUniform = 0;
	//float 
	fVignetteUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}
SceneBlendPostProcessShaderLoader::~SceneBlendPostProcessShaderLoader()
{

}

//extra
void SceneBlendPostProcessShaderLoader::Loadv3SunPos_Uniform(vmath::vec3 sunPos)
{
	UniformLoadVec3(v3SunPos_Uniform, sunPos);
}
void SceneBlendPostProcessShaderLoader::Loadv3EyePos_Uniform(vmath::vec3 eyePos)
{
	UniformLoadVec3(v3EyePos_Uniform, eyePos);
}
void SceneBlendPostProcessShaderLoader::Loadv3EyeDir_Uniform(vmath::vec3 eyeDir)
{
	UniformLoadVec3(v3EyeDir_Uniform, eyeDir);
}

void SceneBlendPostProcessShaderLoader::LoadfVignette_Uniform(GLfloat fVignette)
{
	UniformLoad1f(fVignetteUniform, fVignette);
}

//matrices
void SceneBlendPostProcessShaderLoader::Loadm4InvProjX_Uniform(vmath::mat4 InvProjMatrix)
{
	UniformLoadMatrix(m4InvProjX_Uniform, InvProjMatrix);
}
void SceneBlendPostProcessShaderLoader::Loadm4InvViewX_Uniform(vmath::mat4 InvViewMatrix)
{
	UniformLoadMatrix(m4InvViewX_Uniform, InvViewMatrix);
}

void SceneBlendPostProcessShaderLoader::Loadm4LightSpaceX_Uniform(vmath::mat4 LightSpaceMatrix)
{
	UniformLoadMatrix(m4LightSpaceX_Uniform, LightSpaceMatrix);
}

//Samplers
//scene
void SceneBlendPostProcessShaderLoader::LoadSampler2D_CloudTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_CloudTextureUniform, TEXTURE, tNo, textureID);
}
void SceneBlendPostProcessShaderLoader::LoadSampler2D_SkyTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_SkyTextureUniform, TEXTURE, tNo, textureID);
}
void SceneBlendPostProcessShaderLoader::LoadSampler2D_SceneTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_SceneTextureUniform, TEXTURE, tNo, textureID);
}
void SceneBlendPostProcessShaderLoader::LoadSampler2D_SceneMaskTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_SceneMaskTextureUniform, TEXTURE, tNo, textureID);
}
//postProcess
void SceneBlendPostProcessShaderLoader::LoadSampler2D_GodRaysUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_GodRaysUniform, TEXTURE, tNo, textureID);
}
//extra
void SceneBlendPostProcessShaderLoader::LoadSampler2D_ShadowMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_ShadowMapTextureUniform, TEXTURE, tNo, textureID);
}

void SceneBlendPostProcessShaderLoader::LoadSampler2D_DepthTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_DepthTextureUniform, TEXTURE, tNo, textureID);
}

bool SceneBlendPostProcessShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs,filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void SceneBlendPostProcessShaderLoader::GetAllUniformLocations(void)
{
	//extra
	v3SunPos_Uniform=GetUniformLocation("v3SunPos");
	v3EyePos_Uniform=GetUniformLocation("v3EyePos");
	v3EyeDir_Uniform=GetUniformLocation("v3EyeDir");
	//matrices
	m4InvProjX_Uniform= GetUniformLocation("m4InvProjX");
	m4InvViewX_Uniform= GetUniformLocation("m4InvViewX");
	m4LightSpaceX_Uniform= GetUniformLocation("m4LightSpaceX");
	//scene
	Sampler2D_CloudTextureUniform = GetUniformLocation("tSampler2D_CloudTexture");
	Sampler2D_SkyTextureUniform = GetUniformLocation("tSampler2D_SkyTexture");
	Sampler2D_SceneTextureUniform = GetUniformLocation("tSampler2D_SceneTexture");
	Sampler2D_SceneMaskTextureUniform = GetUniformLocation("tSampler2D_SceneMaskTexture");
	//postprocess
	Sampler2D_GodRaysUniform = GetUniformLocation("tSampler2D_GodRaysTexture");
	//extra
	Sampler2D_ShadowMapTextureUniform = GetUniformLocation("tSampler2D_ShadowMapTexture");
	Sampler2D_DepthTextureUniform = GetUniformLocation("tSampler2D_DepthTexture");
	//float
	fVignetteUniform = GetUniformLocation("fVignette");
}
