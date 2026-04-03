#pragma once
#include "MyObject3D.h"
class Earth3D :public MyObject3D
{
public:
	Earth3D();
	~Earth3D();

	void Render(void)override;

	GLuint getMeshWidth(void)
	{
		return MeshWidth;
	}

	GLfloat* getDeformedSphereFactor(void)
	{
		return &fDeformedSphereFactor;
	}

	void UpdateEarthRotation(GLfloat factor);

	void UpdateEarthRotationDefault(void);

	void UpdateEarthCloudRotationDelusion(void);

	void UpdateEarthCloudRotationTeacher(GLfloat factor);

	void UpdateEarthCloudRotationAggregate(GLfloat factor);

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
	GLuint getTextureID_MoonAlbedo(void)
	{
		return textureID_MoonAlbedo;
	}

	GLuint getTextureID_EarthAlbedoSpec(void)
	{
		return textureID_EarthAlbedoSpec;
	}
	GLuint getTextureID_EarthClouds(void)
	{
		return textureID_EarthClouds;
	}
	GLuint getTextureID_EarthNormal(void)
	{
		return textureID_EarthNormal;
	}
	GLuint getTextureID_EarthWaterNormalMap(void)
	{
		return textureID_EarthWaterNormalMap;
	}
	//Everest
	GLuint getTextureID_EarthLODEverest(void)
	{
		return textureID_EarthLODEverest;
	}
	GLuint getTextureID_EarthLODEverest_NormalAO(void)
	{
		return textureID_EarthLODEverest_NormalAO;
	}
	GLuint getTextureID_EarthLODEverest_HeightMap(void)
	{
		return textureID_EarthLODEverest_Heightmap;
	}
	//India
	GLuint getTextureID_EarthLODIndia(void)
	{
		return textureID_EarthLODIndia;
	}
	GLuint getTextureID_EarthLODIndia_NormalAO(void)
	{
		return textureID_EarthLODIndia_NormalAO;
	}
	GLuint getTextureID_EarthLODIndia_HeightMap(void)
	{
		return textureID_EarthLODIndia_Heightmap;
	}
	GLuint getTextureID_EarthLodBlendMap(void)
	{
		return textureID_EarthLodBlendMap;
	}
	//
	vmath::mat4 GetEarthIndiaRotUVMatrix()
	{
		return EarthIndiaRotX;
	}
	vmath::mat4 GetEarthCloudRotUVMatrix()
	{
		return EarthCloudRotX;
	}

private:
	//Texture IDs
	////grass
	//GLuint textureID_Grass;
	//GLuint textureID_GrassAO;
	//GLuint textureID_GrassNormal;
	//GLuint textureID_GrassRoughnes;
	//GLuint textureID_GrassDisplacement;

	GLuint textureID_MoonAlbedo;

	GLuint textureID_EarthAlbedoSpec;
	GLuint textureID_EarthNormal;
	GLuint textureID_EarthClouds;
	GLuint textureID_EarthWaterNormalMap;

	GLuint textureID_EarthLODIndia;
	GLuint textureID_EarthLODIndia_Heightmap;
	GLuint textureID_EarthLODIndia_NormalAO;

	GLuint textureID_EarthLODEverest;
	GLuint textureID_EarthLODEverest_Heightmap;
	GLuint textureID_EarthLODEverest_NormalAO;

	GLuint textureID_EarthLodBlendMap;
	//GLuint textureID_WaterGradient;

	//Functions..............
	void LoadVao(void)override;
	void LoadObject3DTextures(void)override;

	//Class Variables 
	 const GLuint MeshWidth  = 128;
	 const GLuint MeshHeight = 128;

	// GLfloat terrainScale ;//ndc patch culling depends on scale factor

	 GLfloat fDeformedSphereFactor;//for GUI
	// vmath::vec3 v3LightColor;//for GUI
	 vmath::mat4 EarthIndiaRotX;
	 vmath::mat4 EarthCloudRotX;

	//const GLchar* terrainHeightMapTextureFilename = { "Resources/Textures/TerrainLOD2/Terrain.r16" };
	//const GLchar* terrainNormalTextureFilename = { "Resources/Textures/TerrainLOD2/TerrainNormal.tga" };

	const GLchar* EarthAlbedoSpec_TextureFilename = { "Resources/Textures/Earth/EarthAlbedoSpec.tga" };
	const GLchar* MoonAlbedo_TextureFilename = { "Resources/Textures/Earth/2k_moon.tga" };
	const GLchar* EarthNormal_TextureFilename = { "Resources/Textures/Earth/EarthNormal.tga" };
	const GLchar* EarthClouds_TextureFilename = { "Resources/Textures/Earth/EarthClouds.tga" };

	const GLchar* EarthLODIndia_NormalAOTextureFilename = { "Resources/Textures/Earth/LOD_India/EarthLODIndiaNormal.tga" };
	const GLchar* EarthLODIndia_HeightMapTextureFilename = { "Resources/Textures/Earth/LOD_India/EarthLODIndiaHeight.tga" };
	const GLchar* EarthLODIndia_TextureFilename = { "Resources/Textures/Earth/LOD_India/EarthLODIndiaAlbedoSpec.tga" };
	const GLchar* EarthLodBlendMap_TextureFilename = { "Resources/Textures/Earth/LOD_India/EarthLODAlpha.tga" };

	const GLchar* EarthLODEverest_NormalAOTextureFilename = { "Resources/Textures/Earth/LOD_Everest/EarthLODEverestNormal.tga" };
	const GLchar* EarthLODEverest_HeightMapTextureFilename = { "Resources/Textures/Earth/LOD_Everest/EarthLODEverestHeight.tga" };
	const GLchar* EarthLODEverest_TextureFilename = { "Resources/Textures/Earth/LOD_Everest/EarthLODEverestAlbedoSpec.tga" };

	const GLchar* EarthWaterNormalMap_TextureFilename = { "Resources/Textures/Earth/EarthWaterNormal.tga" };

};

