#include "TwoTextureQuadShaderLoader.h"
#include"MyOpenGL.h"

TwoTextureQuadShaderLoader::TwoTextureQuadShaderLoader()
{
	Sampler2DTexture0_Uniform = 0;
	Sampler2DTexture1_Uniform = 0;
	Sampler2DBlueNoise_Uniform = 0;

	fBlendFactor_Uniform = 0;
	fDitherFactor_Uniform = 0;
	iFrameID_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}

TwoTextureQuadShaderLoader::~TwoTextureQuadShaderLoader()
{

}

void TwoTextureQuadShaderLoader::LoadSampler2DTexture0_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DTexture0_Uniform, TEXTURE, tNo, textureID);
}

void TwoTextureQuadShaderLoader::LoadSampler2DTexture1_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DTexture1_Uniform, TEXTURE, tNo, textureID);
}
void TwoTextureQuadShaderLoader::LoadSampler2DBlueNoise_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DBlueNoise_Uniform, TEXTURE, tNo, textureID);
}

void TwoTextureQuadShaderLoader::LoadBlendFactor(GLfloat factor)
{
	UniformLoad1f(fBlendFactor_Uniform, factor);
}
void TwoTextureQuadShaderLoader::LoadDitherFactor(GLfloat factor)
{
	UniformLoad1f(fDitherFactor_Uniform, factor);
}

void TwoTextureQuadShaderLoader::LoadFrameID_Uniform(GLuint FrameID)
{
	UniformLoad1ui(iFrameID_Uniform, FrameID);
}


bool TwoTextureQuadShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void TwoTextureQuadShaderLoader::GetAllUniformLocations(void)
{
	Sampler2DTexture0_Uniform = GetUniformLocation("tSampler0");
	Sampler2DTexture1_Uniform = GetUniformLocation("tSampler1");
	Sampler2DBlueNoise_Uniform = GetUniformLocation("tSamplerBlueNoise");

	fBlendFactor_Uniform = GetUniformLocation("fBlendFactor");
	fDitherFactor_Uniform = GetUniformLocation("fDitherFactor");
	iFrameID_Uniform = GetUniformLocation("iFrameID");
}
