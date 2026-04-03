#include "RainDropShaderLoader.h"
#include"MyOpenGL.h"

RainDropShaderLoader::RainDropShaderLoader()
{
	m4ViewXUniform = 0;
	m4ProjXUniform = 0;
	m4ModelXUniform = 0;
	v3EyePosUniform = 0;
	SamplerCube_ReflectionTextureUniform = 0;
	SamplerCube_RefractionTextureUniform = 0;

	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}

};
RainDropShaderLoader::~RainDropShaderLoader()
{
};

bool RainDropShaderLoader::Initialize(void)
{
	if (!InitializeShaderLoader(filepath_vs, filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();

	return true;
}

void RainDropShaderLoader::LoadProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ProjXUniform, matrix);
}
void RainDropShaderLoader::LoadViewXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ViewXUniform, matrix);
}
void RainDropShaderLoader::LoadModelXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelXUniform, matrix);
}

void RainDropShaderLoader::LoadEyePosUniform(vmath::vec3 eyePos)
{
	UniformLoadVec3(v3EyePosUniform, eyePos);
}

void RainDropShaderLoader::LoadReflectionTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTextureCubeMap(SamplerCube_ReflectionTextureUniform, TEXTURE, tNo, textureID);
}

void RainDropShaderLoader::LoadRefractionTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTextureCubeMap(SamplerCube_RefractionTextureUniform, TEXTURE, tNo, textureID);
}

void RainDropShaderLoader::GetAllUniformLocations(void)
{
	m4ViewXUniform = GetUniformLocation("m4ViewX");
	m4ProjXUniform = GetUniformLocation("m4ProjX");
	m4ModelXUniform = GetUniformLocation("m4ModelX");
	v3EyePosUniform = GetUniformLocation("v3EyePos");

	SamplerCube_ReflectionTextureUniform = GetUniformLocation("tSamplerReflection");
	SamplerCube_RefractionTextureUniform = GetUniformLocation("tSamplerRefraction");
}

