#include "SunGlareShaderLoader.h"
#include"MyOpenGL.h"

SunGlareShaderLoader::SunGlareShaderLoader()
{

	m4ModelViewProjXUniform = 0;
	v3ScaleUniform = 0;
	tSampler_StockFlareUniform = 0;
	fFactorPitchYawUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}

};
SunGlareShaderLoader::~SunGlareShaderLoader()
{
};

bool SunGlareShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();

	return true;
}


void SunGlareShaderLoader::LoadModelViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelViewProjXUniform, matrix);
}
void SunGlareShaderLoader::LoadSamplerStockFlareUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(tSampler_StockFlareUniform, TEXTURE, tNo, textureID);
}


void SunGlareShaderLoader::Loadv3ScaleUniform(vmath::vec3 scale)
{
	UniformLoadVec3(v3ScaleUniform, scale);
}

void SunGlareShaderLoader::LoadFactorPitchYawUniform(vmath::vec3 factorPitchYaw)
{
	UniformLoadVec3(fFactorPitchYawUniform, factorPitchYaw);
}


void SunGlareShaderLoader::GetAllUniformLocations(void)
{

	m4ModelViewProjXUniform = GetUniformLocation("m4ModelViewProjX");
	v3ScaleUniform = GetUniformLocation("v3Scale");
	fFactorPitchYawUniform = GetUniformLocation("fFactorPitchYaw"); 
	tSampler_StockFlareUniform = GetUniformLocation("tSampler_StockFlare");
}

