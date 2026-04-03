#include "TerrainLOD2.h"

TerrainLOD2::TerrainLOD2()
{
	terrainScale = 10.0f;
	glPatchParameteri(GL_PATCH_VERTICES, 4);//tesselation
	LoadObject3DTextures();

	SetModelMatrix(vmath::vec3(0.0f, 0.0f, 0.0f), 0.0f, vmath::vec3(1.0f, 0.0f, 0.0f), vmath::vec3(terrainScale));
}

TerrainLOD2::~TerrainLOD2()
{
	////grass
	//{
	//	if (textureID_Grass)
	//	{
	//		glDeleteTextures(1, &textureID_Grass);
	//		textureID_Grass = 0;
	//	}
	//	if (textureID_GrassNormal)
	//	{
	//		glDeleteTextures(1, &textureID_GrassNormal);
	//		textureID_GrassNormal = 0;
	//	}
	//	if (textureID_GrassRoughnes)
	//	{
	//		glDeleteTextures(1, &textureID_GrassRoughnes);
	//		textureID_GrassRoughnes = 0;
	//	}

	//	if (textureID_GrassAO)
	//	{
	//		glDeleteTextures(1, &textureID_GrassAO);
	//		textureID_GrassAO = 0;
	//	}
	//	if (textureID_GrassDisplacement)
	//	{
	//		glDeleteTextures(1, &textureID_GrassDisplacement);
	//		textureID_GrassDisplacement = 0;
	//	}
	//}
	
	//maps
	{
		if (textureID_WearSlopeSnowAO)
		{
			glDeleteTextures(1, &textureID_WearSlopeSnowAO);
			textureID_WearSlopeSnowAO = 0;
		}
		if (textureID_Height)
		{
			glDeleteTextures(1, &textureID_Height);
			textureID_Height = 0;
		}
		if (textureID_Normal)
		{
			glDeleteTextures(1, &textureID_Normal);
			textureID_Normal = 0;
		}

		//if (textureID_WaterGradient)
		//{
		//	glDeleteTextures(1, &textureID_WaterGradient);
		//	textureID_WaterGradient = 0;
		//}
	}
}


void TerrainLOD2::Render(void)
{

//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawArraysInstanced(GL_PATCHES, 0, 4, 128 * 128);

//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void TerrainLOD2::LoadVao(void)
{
//overrided
}

void TerrainLOD2::LoadObject3DTextures(void)
{
	//textureID_Height = MyTGA_Loader::LoadTGA_texture(terrainHeightMapTextureFilename,true, GL_LINEAR, false);
	textureID_Height = MyTGA_Loader::LoadRaw16_texture(terrainHeightMapTextureFilename, 4096, 4096);
	textureID_Normal = MyTGA_Loader::LoadTGA_texture(terrainNormalTextureFilename, true, GL_LINEAR, false);

	//textureID_WaterGradient= MyTGA_Loader::LoadTGA_texture(terrainWaterGradientTextureFilename, false, GL_LINEAR, false);

	textureID_WearSlopeSnowAO = MyTGA_Loader::LoadTGA_texture(terrainWearSlopeSnowAO_TextureFilename, true, GL_LINEAR, false);

	////grass
	//{
	//	textureID_Grass = MyTGA_Loader::LoadTGA_texture(terrainGrassTextureFilename, true, GL_LINEAR, true);
	//	textureID_GrassAO = MyTGA_Loader::LoadTGA_texture(terrainGrassAOTextureFilename, true, GL_LINEAR, true);
	//	textureID_GrassNormal = MyTGA_Loader::LoadTGA_texture(terrainGrassNormalTextureFilename, true, GL_LINEAR, true);
	//	textureID_GrassRoughnes = MyTGA_Loader::LoadTGA_texture(terrainGrassRoughnessTextureFilename, true, GL_LINEAR, true);
	//	textureID_GrassDisplacement = MyTGA_Loader::LoadTGA_texture(terrainGrassDisplacementTextureFilename, true, GL_LINEAR, true);
	//}

}

