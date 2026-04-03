#include "ModelPBR_Shading.h"


ModelPBR_Shading::ModelPBR_Shading(void)
{
	LoadObject3DTextures();
}

void ModelPBR_Shading::Render() {};
void ModelPBR_Shading::LoadVao() {};

void ModelPBR_Shading::LoadObject3DTextures(void)
{
	textureID_Albedo = MyTGA_Loader::LoadTGA_texture(AlbedoTextureFilename, true, GL_LINEAR, true);
	textureID_AmbientOcclusion = MyTGA_Loader::LoadTGA_texture(AOTextureFilename, true, GL_LINEAR, true);
	textureID_Normal = MyTGA_Loader::LoadTGA_texture(NormalTextureFilename, true, GL_LINEAR, true);
	textureID_Roughness = MyTGA_Loader::LoadTGA_texture(RoughnessTextureFilename, true, GL_LINEAR, true);
}
