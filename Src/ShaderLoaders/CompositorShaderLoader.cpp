#include "CompositorShaderLoader.h"
#include"MyOpenGL.h"

CompositorShaderLoader::CompositorShaderLoader()
{
	Sampler2D_Deffered_Uniform = 0;
	Sampler2D_Background_Uniform = 0;
	Sampler2D_Clouds_Uniform = 0;
	Sampler2D_FarClouds_Uniform = 0;
	Sampler2D_Flare_Uniform = 0;
	Sampler2D_Extra_Uniform = 0;
	v2InvResolution_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}

CompositorShaderLoader::~CompositorShaderLoader()
{

}

void CompositorShaderLoader::LoadSampler2D_FarClouds_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_FarClouds_Uniform, TEXTURE, tNo, textureID);
}

void CompositorShaderLoader::LoadSampler2D_Clouds_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_Clouds_Uniform, TEXTURE, tNo, textureID);
}

void CompositorShaderLoader::LoadSampler2D_Deffered_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_Deffered_Uniform, TEXTURE, tNo, textureID);
}

void CompositorShaderLoader::LoadSampler2D_Background_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_Background_Uniform, TEXTURE, tNo, textureID);
}

void CompositorShaderLoader::LoadSampler2D_Extra_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_Extra_Uniform, TEXTURE, tNo, textureID);
}

void CompositorShaderLoader::LoadSampler2D_Flare_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_Flare_Uniform, TEXTURE, tNo, textureID);
}

void CompositorShaderLoader::Loadv2InvResolution_Uniform(vmath::vec2 InvRes)
{
	UniformLoadVec2(v2InvResolution_Uniform, InvRes);
}


bool CompositorShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void CompositorShaderLoader::GetAllUniformLocations(void)
{
	Sampler2D_Deffered_Uniform = GetUniformLocation("tSamplerDeffered");
	Sampler2D_Clouds_Uniform = GetUniformLocation("tSamplerClouds");
	Sampler2D_FarClouds_Uniform = GetUniformLocation("tSamplerFarClouds");
	Sampler2D_Background_Uniform = GetUniformLocation("tSamplerBackground");
	Sampler2D_Flare_Uniform = GetUniformLocation("tSamplerFlare");
	Sampler2D_Extra_Uniform = GetUniformLocation("tSamplerExtra");

	v2InvResolution_Uniform = GetUniformLocation("v2InvResolution");
}
