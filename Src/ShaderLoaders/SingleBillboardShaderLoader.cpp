#include "SingleBillboardShaderLoader.h"
#include"MyOpenGL.h"

SingleBillboardShaderLoader::SingleBillboardShaderLoader()
{

	m4ModelViewProjXUniform = 0;
	v3ScaleUniform = 0;
	fIntensityUniform = 0;
	tSamplerBillboardUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}

};
SingleBillboardShaderLoader::~SingleBillboardShaderLoader()
{
};

bool SingleBillboardShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();

	return true;
}


void SingleBillboardShaderLoader::LoadModelViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelViewProjXUniform, matrix);
}

void SingleBillboardShaderLoader::Loadv3ScaleUniform(vmath::vec3 scale)
{
	UniformLoadVec3(v3ScaleUniform, scale);
}
void SingleBillboardShaderLoader::LoadfIntensityUniform(GLfloat Intensity)
{
	UniformLoad1f(fIntensityUniform, Intensity);
}

void SingleBillboardShaderLoader::LoadSamplerBillboardUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(tSamplerBillboardUniform, TEXTURE, tNo, textureID);
}



void SingleBillboardShaderLoader::GetAllUniformLocations(void)
{

	m4ModelViewProjXUniform = GetUniformLocation("m4ModelViewProjX");
	v3ScaleUniform = GetUniformLocation("v3Scale");
	fIntensityUniform = GetUniformLocation("fIntensity");
	tSamplerBillboardUniform = GetUniformLocation("tSampler_SingleBillboard");

}

