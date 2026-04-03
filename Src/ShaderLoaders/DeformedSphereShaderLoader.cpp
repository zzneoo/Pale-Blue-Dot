#include "DeformedSphereShaderLoader.h"
#include"MyOpenGL.h"

DeformedSphereShaderLoader::DeformedSphereShaderLoader()
{
	//uniforms

	//matrices
	m4ModelX_Uniform = 0;
	m4ViewX_Uniform = 0;
	m4ProjX_Uniform = 0;
	m4ModelViewProjX_Uniform = 0;

	//maps
	Sampler2D_SphereMap_Uniform = 0;

	fDeformedSphereFactorUniform = 0;
	//shadow
	//Sampler2D_ShadowMapUniform = 0;
	//m4LightSpaceX_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}


DeformedSphereShaderLoader::~DeformedSphereShaderLoader()
{
	
}

//floats
void DeformedSphereShaderLoader::LoadDeformedSphereFactorUniform(GLfloat fSphereFactor)
{
	UniformLoad1f(fDeformedSphereFactorUniform, fSphereFactor);
}

//matrices
void DeformedSphereShaderLoader::LoadModelXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelX_Uniform, matrix);
}

void DeformedSphereShaderLoader::LoadViewXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ViewX_Uniform, matrix);
}

void DeformedSphereShaderLoader::LoadProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ProjX_Uniform, matrix);
}

void DeformedSphereShaderLoader::LoadModelViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelViewProjX_Uniform, matrix);
}


//maps------------------------------------------------------------------------------------------------------------------------------------------
void DeformedSphereShaderLoader::LoadSphereMap_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_SphereMap_Uniform, TEXTURE, tNo, textureID);
}


////Shadow
//void DeformedSphereShaderLoader::LoadShadowMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
//{
//	UniformLoadTexture2D(Sampler2D_ShadowMapUniform, TEXTURE, tNo, textureID);
//}
//
//void DeformedSphereShaderLoader::Loadm4LightSpaceXUniform(vmath::mat4 matrix)
//{
//	UniformLoadMatrix(m4LightSpaceX_Uniform, matrix);
//}

bool DeformedSphereShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs, filepath_tcs, filepath_tes, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void DeformedSphereShaderLoader::GetAllUniformLocations(void)
{
	//floats
	fDeformedSphereFactorUniform = GetUniformLocation("fDeformedSphereFactor");

	//matrices
	m4ModelX_Uniform = GetUniformLocation("m4ModelX");
	m4ViewX_Uniform = GetUniformLocation("m4ViewX");
	m4ProjX_Uniform = GetUniformLocation("m4ProjX");

	m4ModelViewProjX_Uniform= GetUniformLocation("m4ModelViewProjX");

	//samplers

	//maps
	Sampler2D_SphereMap_Uniform = GetUniformLocation("tSampler_SphereMap");

	//shadow
	//m4LightSpaceX_Uniform = GetUniformLocation("m4LightSpaceX");
	//Sampler2D_ShadowMapUniform = GetUniformLocation("tSampler_ShadowMap");
}
