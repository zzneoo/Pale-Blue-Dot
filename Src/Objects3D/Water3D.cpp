#include "Water3D.h"

Water3D::Water3D()
{

	//glPatchParameteri(GL_PATCH_VERTICES, 4);//tesselation
	//LoadVao();
	TextureID_previousSceneTexture = 0;
	LoadObject3DTextures();

	//SetModelMatrix(vmath::vec3(-terrainScale * 0.5f, 0.0f, -terrainScale*0.5f), 0.0f, vmath::vec3(1.0f, 0.0f, 0.0f), vmath::vec3(terrainScale));
	SetModelMatrix(vmath::vec3(0.0f, 40.0f, 0.0f), 0.0f, vmath::vec3(1.0f, 0.0f, 0.0f), vmath::vec3(WaterScale));
}

Water3D::~Water3D()
{
	if (TextureID_previousSceneTexture)
	{
		glDeleteTextures(1, &TextureID_previousSceneTexture);
		TextureID_previousSceneTexture = 0;
	}

	if (TextureID_TerrainDepthTexture)
	{
		glDeleteTextures(1, &TextureID_TerrainDepthTexture);
		TextureID_TerrainDepthTexture = 0;
	}
}

void Water3D::LoadVao(void)
{

}

void Water3D::Render(void)
{
	//glFrontFace(GL_CW);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDrawArraysInstanced(GL_PATCHES, 0, 4, 512 * 512);
		glDrawArraysInstanced(GL_QUADS, 0, 4, 256 * 256);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	glFrontFace(GL_CCW);
}

void Water3D::LoadObject3DTextures(void)
{
	CreateWaterHeightMapTexture();
	CreateWaterNormalMapTexture();


	glGenTextures(1, &TextureID_previousSceneTexture);
	glBindTexture(GL_TEXTURE_2D, TextureID_previousSceneTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &TextureID_TerrainDepthTexture);
	glBindTexture(GL_TEXTURE_2D, TextureID_TerrainDepthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 1920, 1080, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);


}

void Water3D::CreateWaterHeightMapTexture(void)
{
	//WaterHeightMap
	glGenTextures(1, &textureID_Height);
	glBindTexture(GL_TEXTURE_2D, textureID_Height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, WaterTextureSize, WaterTextureSize, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

	//glGenerateTextureMipmap(textureID_Height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLfloat fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &fLargest);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, fLargest);

	glBindTexture(GL_TEXTURE_2D, 0);

}
void Water3D::CreateWaterNormalMapTexture(void)
{

	//WaterNormalMap
	glGenTextures(1, &textureID_Normal);
	glBindTexture(GL_TEXTURE_2D, textureID_Normal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WaterTextureSize, WaterTextureSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//glGenerateTextureMipmap(textureID_Normal);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLfloat fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &fLargest);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, fLargest);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Water3D::UpdateWater(GLuint TextureID_PerlinWorley3D,GLuint TextureID_Permutation,GLfloat fTime)
{
	const GLsizei WorkGroupSize = 32;

	const GLuint FinalComputeSize = INT_CEIL(WaterTextureSize, WorkGroupSize);

	//WATER_HEIGHT_MAP

	waterHeightMapShader.Start();

	waterHeightMapShader.LoadfTime_Uniform(fTime);
	waterHeightMapShader.LoadSampler1D_permutationTextureUniform(GL_TEXTURE0, 0, TextureID_Permutation);
	waterHeightMapShader.LoadSampler3D_perlinWorleyTextureUniform(GL_TEXTURE1, 1, TextureID_PerlinWorley3D);
	waterHeightMapShader.LoadiTextureSize_Uniform(WaterTextureSize);

	glBindImageTexture(0, textureID_Height, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R8);
	glDispatchCompute(FinalComputeSize, FinalComputeSize, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	waterHeightMapShader.Stop();

	//WATER_NORMAL_MAP
	waterNormalMapShader.Start();
	waterNormalMapShader.LoadSampler2D_WaterHeightMapTextureUniform(GL_TEXTURE0, 0, textureID_Height);
	waterNormalMapShader.LoadiTextureSize_Uniform(WaterTextureSize);

	glBindImageTexture(0, textureID_Normal, 0, GL_FALSE, 0, GL_WRITE_ONLY,GL_RGBA8);
	glDispatchCompute(FinalComputeSize, FinalComputeSize, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	waterNormalMapShader.Stop();
}

vmath::vec3 Water3D::getSpecularColor(GLdouble sunTheta)
{
	vmath::vec3 first = vmath::mix(vmath::vec3(0.376f, 0.067f, 0.0f), vmath::vec3(0.471f, 0.176f, 0.008f), (float)smoothstep(0.0f, 0.1f, sunTheta* sunTheta* sunTheta* sunTheta));//sunTheta

	vmath::vec3 second = vmath::mix(first, vmath::vec3(1.0f, 0.812f, 0.471f), (float)smoothstep(0.1f, 0.22f, pow(sunTheta,0.9f)));

	vmath::vec3 last = vmath::mix(second, vmath::vec3(1.0f),(float) smoothstep(0.22f, 0.528f, sunTheta));

	return last;
}
