#pragma once
#include "MyObject3D.h"
class TerrainBasic3D :public MyObject3D
{
public:
	TerrainBasic3D();
	~TerrainBasic3D();

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
	GLuint getTextureID_MaterialAlbedo(void)
	{
		return textureID_MaterialAlbedo;
	}
	GLuint getTextureID_MaterialAO(void)
	{
		return textureID_MaterialAO;
	}
	GLuint getTextureID_MaterialNormal(void)
	{
		return textureID_MaterialNormal;
	}
	GLuint getTextureID_MaterialRoughness(void)
	{
		return textureID_MaterialRoughness;
	}
	//GLuint getTextureID_GrassDisplacement(void)
	//{
	//	return textureID_GrassDisplacement;
	//}

//------------------------------------------------------------------------


private:
	//Texture IDs
	////grass
	GLuint textureID_MaterialAlbedo;
	GLuint textureID_MaterialAO;
	GLuint textureID_MaterialNormal;
	GLuint textureID_MaterialRoughness;
	//GLuint textureID_GrassDisplacement;
	
	//Functions..............
	void LoadVao(void)override;
	void LoadObject3DTextures(void)override;

	//Class Variables 
	 const GLuint MeshWidth  = 128;
	 const GLuint MeshHeight = 128;

	 GLfloat terrainScale ;//ndc patch culling depends on scale factor

	 GLfloat fTerrainFactor;//for GUI
	// vmath::vec3 v3LightColor;//for GUI


	const GLchar* terrainHeightMapTextureFilename = { "Resources/Textures/TerrainBasic3D/Terrain.r16" };
	const GLchar* terrainNormalTextureFilename = { "Resources/Textures/TerrainBasic3D/TerrainNormal.tga" };

	//Material 
	const GLchar* MaterialAlbedoTextureFilename = { "Resources/Textures/Island/Material/moss4k/se4rwei_4K_Albedo.tga" };
	const GLchar* MaterialNormalTextureFilename = { "Resources/Textures/Island/Material/moss4k/se4rwei_4K_Normal.tga" };
	const GLchar* MaterialAOTextureFilename = { "Resources/Textures/Island/Material/moss4k/se4rwei_4K_AO.tga" };
	const GLchar* MaterialRoughnessTextureFilename = { "Resources/Textures/Island/Material/moss4k/se4rwei_4K_Roughness.tga" };
	//const GLchar* MaterialDisplacementTextureFilename = { "Resources/Textures/Island/Material/moss4k/se4rwei_4K_Displacement.tga" };

};

