#pragma once
#include "MyObject3D.h"
#include"WaterHeightMapShaderLoader.h"
#include"WaterNormalMapShaderLoader.h"

class Water3D :public MyObject3D
{
public:
	Water3D();
	~Water3D();

	void Render(void)override;
	void UpdateWater(GLuint TextureID_PerlinWorley3D, GLuint TextureID_Permutation, GLfloat fTime);

	GLfloat* GetfWaterFactor(void)
	{
		return &fWaterFactor;
	}

	vmath::vec3* Getv3ColorFactor(void)
	{
		return &v3ColorFactor;
	}

	GLuint GetTextureID_previousSceneTexture(void)
	{
		return TextureID_previousSceneTexture;
	}

	GLuint GetTextureID_TerrainDepthTexture(void)
	{
		return TextureID_TerrainDepthTexture;
	}

	GLdouble clamp(GLdouble factor, GLdouble minimum, GLdouble maximum)
	{
#pragma push_macro("min")
#undef min

#pragma push_macro("max")
#undef max
		return vmath::min(vmath::max(factor, minimum), maximum);
#pragma pop_macro("min")
#pragma pop_macro("max")
	}

	GLdouble smoothstep(GLdouble edge0, GLdouble edge1, GLdouble factor)
	{
		GLdouble t = clamp((factor - edge0) / (edge1 - edge0), 0.0, 1.0);
		return t * t * (3.0 - 2.0 * t);
	}

	vmath::vec3 getSpecularColor(GLdouble sunTheta);

private:	
	//Functions..............
	void LoadVao(void)override;
	void LoadObject3DTextures(void)override;

	//shaders
	WaterHeightMapShaderLoader waterHeightMapShader;
	WaterNormalMapShaderLoader waterNormalMapShader;

	//for compute
	void CreateWaterHeightMapTexture(void);
	void CreateWaterNormalMapTexture(void);

	const GLsizei WaterTextureSize = 1024;

	const GLfloat WaterScale = 1.0f;//ndc patch culling depends on scale factor

	GLfloat fWaterFactor;//gui
	vmath::vec3 v3ColorFactor;//gui

	GLuint TextureID_previousSceneTexture;
	GLuint TextureID_TerrainDepthTexture;
};



