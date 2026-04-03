#pragma once
#include "MyObject3D.h"
class TerrainLOD2 :public MyObject3D
{
public:
	TerrainLOD2();
	~TerrainLOD2();

	void Render(void)override;

	GLuint getMeshWidth(void)
	{
		return MeshWidth;
	}

	GLfloat* getTerrainFactor(void)
	{
		return &fTerrainFactor;
	}
	//vmath::vec3* Getv3LightColor(void)
	//{
	//	return &v3LightColor;
	//}

	////grass-----------------------------------------------------------------------------------------------
	//GLuint getTextureID_Grass(void)
	//{
	//	return textureID_Grass;
	//}
	//GLuint getTextureID_GrassAO(void)
	//{
	//	return textureID_GrassAO;
	//}
	//GLuint getTextureID_GrassNormal(void)
	//{
	//	return textureID_GrassNormal;
	//}
	//GLuint getTextureID_GrassRoughness(void)
	//{
	//	return textureID_GrassRoughnes;
	//}
	//GLuint getTextureID_GrassDisplacement(void)
	//{
	//	return textureID_GrassDisplacement;
	//}

//------------------------------------------------------------------------
	GLuint getTextureID_WearSlopeSnowAO(void)
	{
		return textureID_WearSlopeSnowAO;
	}

	//GLuint getTextureID_WaterGradient(void)
	//{
	//	return textureID_WaterGradient;
	//}


private:
	//Texture IDs
	////grass
	//GLuint textureID_Grass;
	//GLuint textureID_GrassAO;
	//GLuint textureID_GrassNormal;
	//GLuint textureID_GrassRoughnes;
	//GLuint textureID_GrassDisplacement;
	
	GLuint textureID_WearSlopeSnowAO;
	//GLuint textureID_WaterGradient;

	//Functions..............
	void LoadVao(void)override;
	void LoadObject3DTextures(void)override;

	//Class Variables 
	 const GLuint MeshWidth  = 128;
	 const GLuint MeshHeight = 128;

	 GLfloat terrainScale ;//ndc patch culling depends on scale factor

	 GLfloat fTerrainFactor;//for GUI
	// vmath::vec3 v3LightColor;//for GUI


	const GLchar* terrainHeightMapTextureFilename = { "Resources/Textures/TerrainLOD2/Terrain.r16" };
	//const GLchar* terrainWaterGradientTextureFilename = { "Resources/Textures/Island/waterGradient.tga" };
	const GLchar* terrainNormalTextureFilename = { "Resources/Textures/TerrainLOD2/TerrainNormal.tga" };

	const GLchar* terrainWearSlopeSnowAO_TextureFilename = { "Resources/Textures/TerrainLOD2/WEAR_SLOPE_SNOW_AO.tga" };

	////grass
	//const GLchar* terrainGrassTextureFilename = { "Resources/Textures/Island/Material/moss4k/se4rwei_4K_Albedo.tga" };
	//const GLchar* terrainGrassNormalTextureFilename = { "Resources/Textures/Island/Material/moss4k/se4rwei_4K_Normal.tga" };
	//const GLchar* terrainGrassAOTextureFilename = { "Resources/Textures/Island/Material/moss4k/se4rwei_4K_AO.tga" };
	//const GLchar* terrainGrassRoughnessTextureFilename = { "Resources/Textures/Island/Material/moss4k/se4rwei_4K_Roughness.tga" };
	//const GLchar* terrainGrassDisplacementTextureFilename = { "Resources/Textures/Island/Material/moss4k/se4rwei_4K_Displacement.tga" };

};

