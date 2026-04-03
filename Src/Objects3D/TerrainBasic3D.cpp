#include "TerrainBasic3D.h"

TerrainBasic3D::TerrainBasic3D()
{
	terrainScale = 10.0f;
	glPatchParameteri(GL_PATCH_VERTICES, 4);//tesselation
	LoadObject3DTextures();

	SetModelMatrix(vmath::vec3(0.0f, -100.0f, 0.0f), 0.0f, vmath::vec3(1.0f, 0.0f, 0.0f), vmath::vec3(terrainScale));//y-10.0f PaleBLue
}

TerrainBasic3D::~TerrainBasic3D()
{
	//Material
	{
		if (textureID_MaterialAlbedo)
		{
			glDeleteTextures(1, &textureID_MaterialAlbedo);
			textureID_MaterialAlbedo = 0;
		}
		if (textureID_MaterialNormal)
		{
			glDeleteTextures(1, &textureID_MaterialNormal);
			textureID_MaterialNormal = 0;
		}
		if (textureID_MaterialRoughness)
		{
			glDeleteTextures(1, &textureID_MaterialRoughness);
			textureID_MaterialRoughness = 0;
		}

		if (textureID_MaterialAO)
		{
			glDeleteTextures(1, &textureID_MaterialAO);
			textureID_MaterialAO = 0;
		}
		//	if (textureID_GrassDisplacement)
		//	{
		//		glDeleteTextures(1, &textureID_GrassDisplacement);
		//		textureID_GrassDisplacement = 0;
		//	}
		//}

		//maps
		{

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
		}
	}
}

void TerrainBasic3D::Render(void)
{

//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawArraysInstanced(GL_PATCHES, 0, 4, 128 * 128);

//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void TerrainBasic3D::LoadVao(void)
{
//overrided
}

void TerrainBasic3D::LoadObject3DTextures(void)
{

	textureID_Height = MyTGA_Loader::LoadRaw16_texture(terrainHeightMapTextureFilename, 1024, 1024);
	textureID_Normal = MyTGA_Loader::LoadTGA_texture(terrainNormalTextureFilename, true, GL_LINEAR, false);

	//Material
	{
		textureID_MaterialAlbedo = MyTGA_Loader::LoadTGA_texture(MaterialAlbedoTextureFilename, true, GL_LINEAR, true);
		textureID_MaterialAO = MyTGA_Loader::LoadTGA_texture(MaterialAOTextureFilename, true, GL_LINEAR, true);
		textureID_MaterialNormal = MyTGA_Loader::LoadTGA_texture(MaterialNormalTextureFilename, true, GL_LINEAR, true);
		textureID_MaterialRoughness = MyTGA_Loader::LoadTGA_texture(MaterialRoughnessTextureFilename, true, GL_LINEAR, true);
	//	textureID_GrassDisplacement = MyTGA_Loader::LoadTGA_texture(terrainGrassDisplacementTextureFilename, true, GL_LINEAR, true);
	}

}

