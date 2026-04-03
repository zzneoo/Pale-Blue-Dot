#include "ModelPBR_Animated.h"
#include "MyOpenGL.h"

ModelPBR_Animated::ModelPBR_Animated(const char* filePath)
{
	indicesCount = 0;
	if (true == MyOpenGL::bObject3DInitStatus)
	{
	//	MyOpenGL::bObject3DInitStatus = MyObjLoader(filePath);
		if (true == MyOpenGL::bObject3DInitStatus)
		{
			LoadVao();
		}
	}
	LoadObject3DTextures();
}


void ModelPBR_Animated::LoadObject3DTextures(void)
{
	textureID_Albedo = MyTGA_Loader::LoadTGA_texture("Resources/Models/ganesha/albedo.tga", true, GL_LINEAR, false);
	textureID_Normal = MyTGA_Loader::LoadTGA_texture("Resources/Models/ganesha/normal.tga", true, GL_LINEAR, false);
	textureID_AmbientOcclusion = MyTGA_Loader::LoadTGA_texture("Resources/Models/ganesha/AO.tga", true, GL_LINEAR, false);
	textureID_Roughness = MyTGA_Loader::LoadTGA_texture("Resources/Models/ganesha/roughness.tga", true, GL_LINEAR, false);

}
void ModelPBR_Animated::LoadVao(void)
{

}

void ModelPBR_Animated::Render(void)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element);
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ModelPBR_Animated::~ModelPBR_Animated()
{
	vertices.clear();
	texCoords.clear();
	normals.clear();
	tangents.clear();

	modelIndices.clear();
}
