#include "WaterShaderLoader.h"
#include"MyOpenGL.h"

WaterShaderLoader::WaterShaderLoader()
{
	m4ModelX_Uniform = 0;
	m4ViewX_Uniform = 0;
	m4ProjX_Uniform = 0;
	m4ModelViewProjX_Uniform = 0;

	fWaterFactor_Uniform = 0;

	v3EyePos_Uniform = 0;
	v3EyeDir_Uniform = 0;
	v3SunPos_Uniform = 0;
	v3ColorFactor_Uniform = 0;
	v3SpecularColor_Uniform = 0;

	Sampler2D_HeightMapUniform = 0;
	Sampler2D_NormalMapUniform = 0;
	Sampler2D_RefractionUniform = 0;
	Sampler2D_ReflectionUniform = 0;
	Sampler2D_TerrainDepthUniform = 0;
	
	if (MyOpenGL::bShaderInitStatus == true)
	{
		MyOpenGL::bShaderInitStatus = Initialize();
	}
}

WaterShaderLoader::~WaterShaderLoader()
{

}
//matrices---------------------------------------------------------------
void WaterShaderLoader::LoadModelXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelX_Uniform, matrix);
}

void WaterShaderLoader::LoadViewXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ViewX_Uniform, matrix);
}

void WaterShaderLoader::LoadProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ProjX_Uniform, matrix);
}

void WaterShaderLoader::LoadModelViewProjXUniform(vmath::mat4 matrix)
{
	UniformLoadMatrix(m4ModelViewProjX_Uniform, matrix);
}

//vectors

void WaterShaderLoader::Loadv3EyePosUniform(vmath::vec3 eyePos)
{
	UniformLoadVec3(v3EyePos_Uniform, eyePos);
}
void WaterShaderLoader::Loadv3SunPosUniform(vmath::vec3 sunPos)
{
	UniformLoadVec3(v3SunPos_Uniform, sunPos);
}
void WaterShaderLoader::Loadv3EyeDirUniform(vmath::vec3 eyeDir)
{
	UniformLoadVec3(v3EyeDir_Uniform, eyeDir);
}
void WaterShaderLoader::Loadv3ColorFactorUniform(vmath::vec3 v3Color)
{
	UniformLoadVec3(v3ColorFactor_Uniform, v3Color);
}
void WaterShaderLoader::Loadv3SpecularColorUniform(vmath::vec3 v3SpecularColor)
{
	UniformLoadVec3(v3SpecularColor_Uniform, v3SpecularColor);
}

void WaterShaderLoader::LoadfWaterFactorUniform(GLfloat factor)
{
	UniformLoad1f(fWaterFactor_Uniform, factor);
}

//Samplers----------------------------------------------------------------------------------------------
void WaterShaderLoader::LoadHeightMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_HeightMapUniform, TEXTURE, tNo, textureID);
}
void WaterShaderLoader::LoadNormalMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_NormalMapUniform, TEXTURE, tNo, textureID);
}
void WaterShaderLoader::LoadRefractionTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_RefractionUniform, TEXTURE, tNo, textureID);
}
void WaterShaderLoader::LoadReflectionTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_ReflectionUniform, TEXTURE, tNo, textureID);
}
void WaterShaderLoader::LoadTerrainDepthTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID)
{
	UniformLoadTexture2D(Sampler2D_TerrainDepthUniform, TEXTURE, tNo, textureID);
}

bool WaterShaderLoader::Initialize()
{
	if (!InitializeShaderLoader(filepath_vs,filepath_fs))
	{
		return false;
	}
	GetAllUniformLocations();
	return true;
}

void WaterShaderLoader::GetAllUniformLocations(void)
{
	//matrices
	m4ModelX_Uniform = GetUniformLocation("m4ModelX");
	m4ViewX_Uniform = GetUniformLocation("m4ViewX");
	m4ProjX_Uniform = GetUniformLocation("m4ProjX");

	m4ModelViewProjX_Uniform = GetUniformLocation("m4ModelViewProjX");

	//vectors
	v3EyePos_Uniform = GetUniformLocation("v3EyePos");
	v3EyeDir_Uniform = GetUniformLocation("v3EyeDir");
	v3SunPos_Uniform = GetUniformLocation("v3SunPos");
	v3ColorFactor_Uniform = GetUniformLocation("v3ColorFactor");
	v3SpecularColor_Uniform = GetUniformLocation("v3SpecularColor");

	fWaterFactor_Uniform = GetUniformLocation("fWaterFactor");

	//samplers
	Sampler2D_HeightMapUniform = GetUniformLocation("tSampler_heightmap");
	Sampler2D_NormalMapUniform = GetUniformLocation("tSampler_normalmap");
	Sampler2D_RefractionUniform = GetUniformLocation("tSampler_refraction");
	Sampler2D_ReflectionUniform = GetUniformLocation("tSampler_reflection");
	Sampler2D_TerrainDepthUniform = GetUniformLocation("tSampler_terrainDepth");

}
