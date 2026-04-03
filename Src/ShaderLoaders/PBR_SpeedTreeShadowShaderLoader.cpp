#include "PBR_SpeedTreeShadowShaderLoader.h"
#include"MyOpenGL.h"

PBR_SpeedTreeShadowShaderLoader::PBR_SpeedTreeShadowShaderLoader()
{
	//uniform locations
	m4ModelViewProjX_Uniform = 0;
	//Samplers
	Sampler2D_PBR_AlbedoUniform = 0;


	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}
PBR_SpeedTreeShadowShaderLoader::~PBR_SpeedTreeShadowShaderLoader()
{
}

//matrices-------------------------------------------------------------------------------------------
void PBR_SpeedTreeShadowShaderLoader::LoadModelViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelViewProjX_Uniform, matrix);
}

//samplers------------------------------------------------------------------------------------------------
void PBR_SpeedTreeShadowShaderLoader::LoadAlbedoTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_PBR_AlbedoUniform, TEXTURE, tNo, textureID);
}


//----------------------------------------------------------------------------------------------------------

bool PBR_SpeedTreeShadowShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void PBR_SpeedTreeShadowShaderLoader::GetAllUniformLocations(void)
{

	//matrices
	m4ModelViewProjX_Uniform = GetUniformLocation("m4ModelViewProjX");
	//samplers
	Sampler2D_PBR_AlbedoUniform = GetUniformLocation("Sampler2D_Albedo");

}
