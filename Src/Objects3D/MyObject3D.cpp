#include"stdafx.h"
#include"MyObject3D.h"

MyObject3D::MyObject3D(void)
{
	vao = 0;
	vbo = 0;
	vbo_element = 0;

	textureID_Albedo = 0;
	textureID_Normal = 0;
	textureID_Displacement = 0;
	textureID_Height = 0;
	textureID_AmbientOcclusion = 0;
	textureID_Roughness = 0;
	textureID_Subsurface = 0;

	ModelMatrix = vmath::mat4::identity();
}

MyObject3D::~MyObject3D(void)
{
	CleanUp_Object3D();
}

void MyObject3D::CleanUp_Object3D(void)
{

	if (vbo_element)
	{
		glDeleteBuffers(1, &vbo_element);
		vbo_element = 0;
	}
	if (vbo)
	{
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}

	if (vboPerInstanceModelX)
	{
		glDeleteBuffers(1, &vboPerInstanceModelX);
		vboPerInstanceModelX = 0;
	}

	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}

	if (textureID_Albedo)
	{
		glDeleteTextures(1, &textureID_Albedo);
		textureID_Albedo = 0;
	}
	if (textureID_Normal)
	{
		glDeleteTextures(1, &textureID_Normal);
		textureID_Normal = 0;
	}
	if (textureID_Displacement)
	{
		glDeleteTextures(1, &textureID_Displacement);
		textureID_Displacement = 0;
	}
	if (textureID_Height)
	{
		glDeleteTextures(1, &textureID_Height);
		textureID_Height = 0;
	}
	if (textureID_AmbientOcclusion)
	{
		glDeleteTextures(1, &textureID_AmbientOcclusion);
		textureID_AmbientOcclusion = 0;
	}
	if (textureID_Roughness)
	{
		glDeleteTextures(1, &textureID_Roughness);
		textureID_Roughness = 0;
	}
	if (textureID_Subsurface)
	{
		glDeleteTextures(1, &textureID_Subsurface);
		textureID_Subsurface = 0;
	}

}
