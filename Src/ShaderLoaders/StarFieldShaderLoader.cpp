#include"StarFieldShaderLoader.h"
#include"MyOpenGL.h"

StarFieldShaderLoader::StarFieldShaderLoader()
{
	m4ModelViewProjXUniform = 0;
	fIntensityFactorUniform = 0;

	if(MyOpenGL::bShaderInitStatus==true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
	
};
StarFieldShaderLoader::~StarFieldShaderLoader()
{
};

bool StarFieldShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();

	return true;
}

void StarFieldShaderLoader::LoadModelViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelViewProjXUniform, matrix);
}

void StarFieldShaderLoader::LoadIntensityFactorUniform(GLfloat IntensityFactor)
{
	UniformLoad1f(fIntensityFactorUniform, IntensityFactor);
}

void StarFieldShaderLoader::GetAllUniformLocations(void)
{
	m4ModelViewProjXUniform = GetUniformLocation("m4ModelViewProjX");
	fIntensityFactorUniform = GetUniformLocation("fIntensityFactor");
}

