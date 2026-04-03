#pragma once
#include "MyObject3D.h"
class Terrain3D :public MyObject3D
{
public:
	Terrain3D();
	~Terrain3D();

	void Render(void)override;

	GLuint getMeshWidth(void)
	{
		return MeshWidth;
	}

	GLfloat* getTerrainFactor(void)
	{
		return &fTerrainFactor;
	}
	vmath::vec3* Getv3LightColor(void)
	{
		return &v3LightColor;
	}

	//grass-----------------------------------------------------------------------------------------------
	GLuint getTextureID_Grass(void)
	{
		return textureID_Grass;
	}
	GLuint getTextureID_GrassAO(void)
	{
		return textureID_GrassAO;
	}
	GLuint getTextureID_GrassNormal(void)
	{
		return textureID_GrassNormal;
	}
	GLuint getTextureID_GrassRoughness(void)
	{
		return textureID_GrassRoughnes;
	}
	GLuint getTextureID_GrassDisplacement(void)
	{
		return textureID_GrassDisplacement;
	}

	//sand----------------------------------------------------------------
	GLuint getTextureID_Sand(void)
	{
		return textureID_Sand;
	}
	GLuint getTextureID_SandAO(void)
	{
		return textureID_SandAO;
	}
	GLuint getTextureID_SandRoughness(void)
	{
		return textureID_SandRoughnes;
	}
	GLuint getTextureID_SandNormal(void)
	{
		return textureID_SandNormal;
	}
	GLuint getTextureID_SandDisplacement(void)
	{
		return textureID_SandDisplacement;
	}
	//rock------------------------------------------------------------
	GLuint getTextureID_Rock(void)
	{
		return textureID_Rock;
	}
	GLuint getTextureID_RockAO(void)
	{
		return textureID_RockAO;
	}
	GLuint getTextureID_RockNormal(void)
	{
		return textureID_RockNormal;
	}
	GLuint getTextureID_RockRoughness(void)
	{
		return textureID_RockRoughnes;
	}
	GLuint getTextureID_RockDisplacement(void)
	{
		return textureID_RockDisplacement;
	}
	//talus--------------------------------------------------------------
	GLuint getTextureID_Talus(void)
	{
		return textureID_Talus;
	}
	GLuint getTextureID_TalusAO(void)
	{
		return textureID_TalusAO;
	}
	GLuint getTextureID_TalusNormal(void)
	{
		return textureID_TalusNormal;
	}
	GLuint getTextureID_TalusRoughness(void)
	{
		return textureID_TalusRoughnes;
	}
	GLuint getTextureID_TalusDisplacement(void)
	{
		return textureID_TalusDisplacement;
	}
//------------------------------------------------------------------------
	GLuint getTextureID_LandSWTR(void)
	{
		return textureID_LandSWTR;
	}
	GLuint getTextureID_WaterRBWD(void)
	{
		return textureID_WaterRBWD;
	}
	GLuint getTextureID_WaterGradient(void)
	{
		return textureID_WaterGradient;
	}
	//lightColor
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
	vmath::vec3 getLightColor(GLdouble sunTheta);

private:
	//Texture IDs
	//grass
	GLuint textureID_Grass;
	GLuint textureID_GrassAO;
	GLuint textureID_GrassNormal;
	GLuint textureID_GrassRoughnes;
	GLuint textureID_GrassDisplacement;
	//sand
	GLuint textureID_Sand;
	GLuint textureID_SandAO;
	GLuint textureID_SandNormal;
	GLuint textureID_SandRoughnes;
	GLuint textureID_SandDisplacement;
	//rock
	GLuint textureID_Rock;
	GLuint textureID_RockAO;
	GLuint textureID_RockNormal;
	GLuint textureID_RockRoughnes;
	GLuint textureID_RockDisplacement;
	//talus
	GLuint textureID_Talus;
	GLuint textureID_TalusAO;
	GLuint textureID_TalusNormal;
	GLuint textureID_TalusRoughnes;
	GLuint textureID_TalusDisplacement;

	GLuint textureID_LandSWTR;
	GLuint textureID_WaterRBWD;
	GLuint textureID_WaterGradient;

	//Functions..............
	void LoadVao(void)override;
	void LoadObject3DTextures(void)override;

	//Class Variables 
	 const GLuint MeshWidth  = 64;
	 const GLuint MeshHeight = 64;

	 GLfloat terrainScale ;//ndc patch culling depends on scale factor

	 GLfloat fTerrainFactor;//for GUI
	 vmath::vec3 v3LightColor;//for GUI

	GLsizei IndicesCount;

	const GLchar* terrainHeightMapTextureFilename = { "Resources/Textures/Island/IslandHeight.r16" };
	const GLchar* terrainWaterGradientTextureFilename = { "Resources/Textures/Island/waterGradient.tga" };
	const GLchar* terrainNormalTextureFilename = { "Resources/Textures/Island/IslandNormalFlow.tga" };

	const GLchar* terrainLandSWTR_TextureFilename = { "Resources/Textures/Island/SWTR.tga" };
	const GLchar* terrainWaterRBWD_TextureFilename = { "Resources/Textures/Island/RBWD.tga" };

	//grass
	const GLchar* terrainGrassTextureFilename = { "Resources/Textures/Island/Material/moss4k/se4rwei_4K_Albedo.tga" };
	const GLchar* terrainGrassNormalTextureFilename = { "Resources/Textures/Island/Material/moss4k/se4rwei_4K_Normal.tga" };
	const GLchar* terrainGrassAOTextureFilename = { "Resources/Textures/Island/Material/moss4k/se4rwei_4K_AO.tga" };
	const GLchar* terrainGrassRoughnessTextureFilename = { "Resources/Textures/Island/Material/moss4k/se4rwei_4K_Roughness.tga" };
	const GLchar* terrainGrassDisplacementTextureFilename = { "Resources/Textures/Island/Material/moss4k/se4rwei_4K_Displacement.tga" };

	//sand
	const GLchar* terrainSandTextureFilename = { "Resources/Textures/Island/Material/sand4k/Sand_Albedo.tga" };
	const GLchar* terrainSandNormalTextureFilename = { "Resources/Textures/Island/Material/sand4k/Sand_Normal.tga" };
	const GLchar* terrainSandAO_TextureFilename = { "Resources/Textures/Island/Material/sand4k/Sand_AO.tga" };
	const GLchar* terrainSandRoughnessTextureFilename = { "Resources/Textures/Island/Material/sand4k/Sand_Rough.tga" };
	const GLchar* terrainSandDisplacementTextureFilename = { "Resources/Textures/Island/Material/sand4k/Sand_Disp.tga" };
	//rock
	const GLchar* terrainRockTextureFilename = { "Resources/Textures/Island/Material/rock4k/Rock_Albedo.tga" };
	const GLchar* terrainRockNormalTextureFilename = { "Resources/Textures/Island/Material/rock4k/Rock_Normal.tga" };
	const GLchar* terrainRockAO_TextureFilename = { "Resources/Textures/Island/Material/rock4k/Rock_AO.tga" };
	const GLchar* terrainRockRoughnessTextureFilename = { "Resources/Textures/Island/Material/rock4k/Rock_Rough.tga" };
	const GLchar* terrainRockDisplacementTextureFilename = { "Resources/Textures/Island/Material/rock4k/Rock_Disp.tga" };
	//talus
	const GLchar* terrainTalusTextureFilename = { "Resources/Textures/Island/Material/talus4k/Talus_Albedo.tga" };
	const GLchar* terrainTalusNormalTextureFilename = { "Resources/Textures/Island/Material/talus4k/Talus_Normal.tga" };
	const GLchar* terrainTalusAO_TextureFilename = { "Resources/Textures/Island/Material/talus4k/Talus_AO.tga" };
	const GLchar* terrainTalusRoughnessTextureFilename = { "Resources/Textures/Island/Material/talus4k/Talus_Rough.tga" };
	const GLchar* terrainTalusDisplacementTextureFilename = { "Resources/Textures/Island/Material/talus4k/Talus_Disp.tga" };

};

