#pragma once
#include "MyObject3D.h"
class ModelPBR_Shading :public MyObject3D
{
public:
	ModelPBR_Shading();
	~ModelPBR_Shading() {};

	void Render(void)override;
private:
	//functions
	void LoadVao(void)override;//pure virtual
	void LoadObject3DTextures(void)override;//pure virtual

	//file names
	const GLchar* AlbedoTextureFilename = { "Resources/Textures/Rat/Brown/Albedo.tga" };
	const GLchar* NormalTextureFilename = { "Resources/Textures/Rat/Brown/Normal.tga" };
	const GLchar* AOTextureFilename = { "Resources/Textures/Rat/Brown/AO.tga" };
	const GLchar* RoughnessTextureFilename = { "Resources/Textures/Rat/Brown/Roughness.tga" };

};

