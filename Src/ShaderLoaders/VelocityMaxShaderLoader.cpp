#include "VelocityMaxShaderLoader.h"
#include"MyOpenGL.h"

VelocityMaxShaderLoader::VelocityMaxShaderLoader()
{

	Sampler2DCurrVelocity_Uniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}

VelocityMaxShaderLoader::~VelocityMaxShaderLoader()
{

}

//Velocity
void VelocityMaxShaderLoader::LoadSampler2DCurrVelocity_Uniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2DCurrVelocity_Uniform, TEXTURE, tNo, textureID);
}


bool VelocityMaxShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_cs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void VelocityMaxShaderLoader::GetAllUniformLocations(void)
{

	Sampler2DCurrVelocity_Uniform = GetUniformLocation("tSamplerCurrVelocity");

}
