#include"passThroughShaderLoader.h"
#include"MyOpenGL.h"

PassThroughShaderLoader::PassThroughShaderLoader()
{
	ModelViewProjXUniform = 0;
	iFrameID_Uniform = 0;
	fTime_Uniform = 0;
	Sampler2D_AlbedoTextureUniform = 0;

	if(MyOpenGL::bShaderInitStatus==true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
	
};
PassThroughShaderLoader::~PassThroughShaderLoader()
{
};

bool PassThroughShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();

	return true;
}

void PassThroughShaderLoader::LoadModelViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(ModelViewProjXUniform, matrix);
}
void PassThroughShaderLoader::LoadiFrameID_Uniform(GLuint iFrameID)
{
	UniformLoad1ui(iFrameID_Uniform, iFrameID);
}
void PassThroughShaderLoader::LoadfTime_Uniform(GLfloat fTime)
{
	UniformLoad1f(fTime_Uniform, fTime);
}

void PassThroughShaderLoader::LoadAlbedoTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_AlbedoTextureUniform, TEXTURE, tNo, textureID);
}

void PassThroughShaderLoader::GetAllUniformLocations(void)
{
	ModelViewProjXUniform = GetUniformLocation("m4ModelViewProjX");
	iFrameID_Uniform = GetUniformLocation("iFrameID");
	fTime_Uniform = GetUniformLocation("fTime");

	Sampler2D_AlbedoTextureUniform = GetUniformLocation("tSampler2D");
}

