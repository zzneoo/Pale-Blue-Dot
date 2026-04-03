#include "Earth3D.h"
#include"RayCaster.h"

Earth3D::Earth3D()
{
	//terrainScale = 10.0f;
	glPatchParameteri(GL_PATCH_VERTICES, 4);//tesselation
	//LoadObject3DTextures();

	SetModelMatrix(vmath::vec3(0.0f, 0.0f, 0.0f), 0.0f, vmath::vec3(1.0f, 0.0f, 0.0f), vmath::vec3(20.0f));//not used..screenspace sphere

	EarthIndiaRotX = vmath::rotate(186.207f, 1.0f, 0.0f, 0.0f) * vmath::rotate(263.793f, 0.0f, 1.0f, 0.0f) * vmath::rotate(65.172f, 1.0f, 0.0f, 0.0f);//my location on earth
	EarthCloudRotX =  vmath::rotate(120.0f, 0.0f, 1.0f, 0.0f)* vmath::rotate(208.831f, 1.0f, 0.0f, 0.0f);//my location on earth
		
}

Earth3D::~Earth3D()
{
	
	//maps
	{
		if (textureID_MoonAlbedo)
		{
			glDeleteTextures(1, &textureID_MoonAlbedo);
			textureID_MoonAlbedo = 0;
		}

		if (textureID_EarthAlbedoSpec)
		{
			glDeleteTextures(1, &textureID_EarthAlbedoSpec);
			textureID_EarthAlbedoSpec = 0;
		}
		if (textureID_EarthClouds)
		{
			glDeleteTextures(1, &textureID_EarthClouds);
			textureID_EarthClouds = 0;
		}
		if (textureID_EarthNormal)
		{
			glDeleteTextures(1, &textureID_EarthNormal);
			textureID_EarthNormal = 0;
		}
		if (textureID_EarthWaterNormalMap)
		{
			glDeleteTextures(1, &textureID_EarthWaterNormalMap);
			textureID_EarthWaterNormalMap = 0;
		}
		//Everest
		if (textureID_EarthLODEverest)
		{
			glDeleteTextures(1, &textureID_EarthLODEverest);
			textureID_EarthLODEverest = 0;
		}
		if (textureID_EarthLODEverest_NormalAO)
		{
			glDeleteTextures(1, &textureID_EarthLODEverest_NormalAO);
			textureID_EarthLODEverest_NormalAO = 0;
		}
		if (textureID_EarthLODEverest_Heightmap)
		{
			glDeleteTextures(1, &textureID_EarthLODEverest_Heightmap);
			textureID_EarthLODEverest_Heightmap = 0;
		}
		//India
		if (textureID_EarthLODIndia)
		{
			glDeleteTextures(1, &textureID_EarthLODIndia);
			textureID_EarthLODIndia = 0;
		}
		if (textureID_EarthLODIndia_NormalAO)
		{
			glDeleteTextures(1, &textureID_EarthLODIndia_NormalAO);
			textureID_EarthLODIndia_NormalAO = 0;
		}
		if (textureID_EarthLODIndia_Heightmap)
		{
			glDeleteTextures(1, &textureID_EarthLODIndia_Heightmap);
			textureID_EarthLODIndia_Heightmap = 0;
		}
		if (textureID_EarthLodBlendMap)
		{
			glDeleteTextures(1, &textureID_EarthLodBlendMap);
			textureID_EarthLodBlendMap = 0;
		}


	}
}


void Earth3D::Render(void)
{
	//glDisable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawArraysInstanced(GL_PATCHES, 0, 4, MeshWidth * MeshHeight);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Earth3D::LoadVao(void)
{
//overrided
}

void Earth3D::UpdateEarthRotation(GLfloat factor)
{
	EarthIndiaRotX = vmath::rotate(191.207f - 17.0f * (1.0f - factor), 1.0f, 0.0f, 0.0f) * vmath::rotate(263.793f, 0.0f, 1.0f, 0.0f) * vmath::rotate(65.172f, 1.0f, 0.0f, 0.0f);//my location on earth
	EarthCloudRotX = vmath::rotate(RayCaster::fRotAngleZ, 0.0f, 0.0f, 1.0f)*vmath::rotate(RayCaster::fRotAngleY, 0.0f, 1.0f, 0.0f)*vmath::rotate(90.390f, 1.0f, 0.0f, 0.0f)*EarthIndiaRotX;//my location on earth
}

void Earth3D::UpdateEarthRotationDefault(void)
{
	EarthIndiaRotX = vmath::rotate(186.207f, 1.0f, 0.0f, 0.0f) * vmath::rotate(263.793f, 0.0f, 1.0f, 0.0f) * vmath::rotate(65.172f, 1.0f, 0.0f, 0.0f);//my location on earth
	EarthCloudRotX = vmath::rotate(120.0f, 0.0f, 1.0f, 0.0f) * vmath::rotate(208.831f, 1.0f, 0.0f, 0.0f);//my location on earth
}

void Earth3D::UpdateEarthCloudRotationDelusion(void)
{
	//EarthCloudRotX = vmath::rotate(172.987f, 0.0f, 0.0f, 1.0f) * vmath::rotate(0.0f, 0.0f, 1.0f, 0.0f) * vmath::rotate(110.649f, 1.0f, 0.0f, 0.0f);//Delusion scene
	//EarthCloudRotX = vmath::rotate(84.156f, 0.0f, 0.0f, 1.0f) * vmath::rotate(65.455f, 0.0f, 1.0f, 0.0f) * vmath::rotate(32.727f, 1.0f, 0.0f, 0.0f);//lonelySpec scene
	EarthCloudRotX = vmath::rotate(0.0f, 0.0f, 0.0f, 1.0f) * vmath::rotate(212.143f, 0.0f, 1.0f, 0.0f) * vmath::rotate(154.286f, 1.0f, 0.0f, 0.0f);//AfterLife scene
}

void Earth3D::UpdateEarthCloudRotationTeacher(GLfloat factor)
{
	EarthCloudRotX = vmath::rotate(120.0f - factor, 0.0f, 1.0f, 0.0f) * vmath::rotate(208.831f, 1.0f, 0.0f, 0.0f);//my location on earth
}

void Earth3D::UpdateEarthCloudRotationAggregate(GLfloat factor)
{
	EarthIndiaRotX = vmath::rotate(186.207f-10.0f* (1.0f - factor), 1.0f, 0.0f, 0.0f) * vmath::rotate(263.793f, 0.0f, 1.0f, 0.0f) * vmath::rotate(65.172f, 1.0f, 0.0f, 0.0f);//my location on earth
	EarthCloudRotX = vmath::rotate( vmath::mix(12.8572f,16.854405f, (1.0f - factor)) , 0.0f, 0.0f, 1.0f) * vmath::rotate(24.935f, 0.0f, 1.0f, 0.0f) * vmath::rotate(RayCaster::fRotAngleX, 1.0f, 0.0f, 0.0f) * EarthIndiaRotX;//my location on earth
}


void Earth3D::LoadObject3DTextures(void)
{
	//textureID_Height = MyTGA_Loader::LoadTGA_texture(terrainHeightMapTextureFilename,true, GL_LINEAR, false);
	//textureID_Height = MyTGA_Loader::LoadRaw16_texture(terrainHeightMapTextureFilename, 4096, 4096);
	//textureID_Normal = MyTGA_Loader::LoadTGA_texture(terrainNormalTextureFilename, true, GL_LINEAR, false);

	//textureID_MoonAlbedo = MyTGA_Loader::LoadTGA_texture(MoonAlbedo_TextureFilename, true, GL_LINEAR, true);

	textureID_EarthAlbedoSpec = MyTGA_Loader::LoadTGA_texture(EarthAlbedoSpec_TextureFilename, true, GL_LINEAR, true);
	textureID_EarthClouds = MyTGA_Loader::LoadTGA_texture(EarthClouds_TextureFilename, true, GL_LINEAR, true);
	textureID_EarthNormal = MyTGA_Loader::LoadTGA_texture(EarthNormal_TextureFilename, true, GL_LINEAR, false);
	//textureID_EarthWaterNormalMap = MyTGA_Loader::LoadTGA_texture(EarthWaterNormalMap_TextureFilename, false, GL_LINEAR, true);

	////Everest
	//textureID_EarthLODEverest = MyTGA_Loader::LoadTGA_texture(EarthLODEverest_TextureFilename, true, GL_LINEAR, false);
	//textureID_EarthLODEverest_NormalAO = MyTGA_Loader::LoadTGA_texture(EarthLODEverest_NormalAOTextureFilename, true, GL_LINEAR, false);
	//textureID_EarthLODEverest_Heightmap = MyTGA_Loader::LoadTGA_texture(EarthLODEverest_HeightMapTextureFilename, true, GL_LINEAR, false);

	////India
	//textureID_EarthLODIndia = MyTGA_Loader::LoadTGA_texture(EarthLODIndia_TextureFilename, true, GL_LINEAR, false);
	//textureID_EarthLODIndia_NormalAO = MyTGA_Loader::LoadTGA_texture(EarthLODIndia_NormalAOTextureFilename, true, GL_LINEAR, false);
	//textureID_EarthLODIndia_Heightmap = MyTGA_Loader::LoadTGA_texture(EarthLODIndia_HeightMapTextureFilename, true, GL_LINEAR, false);

	textureID_EarthLodBlendMap = MyTGA_Loader::LoadTGA_texture(EarthLodBlendMap_TextureFilename, false, GL_LINEAR, false);


}

