#include "LightningQuadShaderLoader.h"
#include"MyOpenGL.h"

LightningQuadShaderLoader::LightningQuadShaderLoader()
{
	Sampler2DTexture_Uniform = 0;
	fBlendFactorUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}

LightningQuadShaderLoader::~LightningQuadShaderLoader()
{

}

void LightningQuadShaderLoader::LoadSampler2DTexture_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DTexture_Uniform, TEXTURE, tNo, textureID);
}

void LightningQuadShaderLoader::LoadBlendFactorUniform(GLfloat factor)
{
	UniformLoad1f(fBlendFactorUniform, factor);
}


bool LightningQuadShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void LightningQuadShaderLoader::GetAllUniformLocations(void)
{
	Sampler2DTexture_Uniform = GetUniformLocation("tSampler");
	fBlendFactorUniform = GetUniformLocation("fBlendFactor");
}
