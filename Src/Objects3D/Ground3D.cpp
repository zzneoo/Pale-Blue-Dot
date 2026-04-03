#include "Ground3D.h"

Ground3D::Ground3D()
{

	glPatchParameteri(GL_PATCH_VERTICES, 4);//tesselation

	LoadObject3DTextures();
	SetModelMatrix(vmath::vec3(0.0f, 7.0f, 0.0f), 0.0f, vmath::vec3(1.0f, 0.0f, 0.0f), vmath::vec3(groundScale));
}

Ground3D::~Ground3D()
{
	//Ground
	{
		if (textureID_Ground)
		{
			glDeleteTextures(1, &textureID_Ground);
			textureID_Ground = 0;
		}
		if (textureID_GroundNormal)
		{
			glDeleteTextures(1, &textureID_GroundNormal);
			textureID_GroundNormal = 0;
		}
		if (textureID_GroundRoughnes)
		{
			glDeleteTextures(1, &textureID_GroundRoughnes);
			textureID_GroundRoughnes = 0;
		}

		if (textureID_GroundAO)
		{
			glDeleteTextures(1, &textureID_GroundAO);
			textureID_GroundAO = 0;
		}
		if (textureID_GroundDisplacement)
		{
			glDeleteTextures(1, &textureID_GroundDisplacement);
			textureID_GroundDisplacement = 0;
		}
	}

	//Scene1
	{
		if (textureID_Scene1)
		{
			glDeleteTextures(1, &textureID_Scene1);
			textureID_Scene1 = 0;
		}
		if (textureID_Scene1Normal)
		{
			glDeleteTextures(1, &textureID_Scene1Normal);
			textureID_Scene1Normal = 0;
		}
		if (textureID_Scene1Roughnes)
		{
			glDeleteTextures(1, &textureID_Scene1Roughnes);
			textureID_Scene1Roughnes = 0;
		}

		if (textureID_Scene1AO)
		{
			glDeleteTextures(1, &textureID_Scene1AO);
			textureID_Scene1AO = 0;
		}
		if (textureID_Scene1Displacement)
		{
			glDeleteTextures(1, &textureID_Scene1Displacement);
			textureID_Scene1Displacement = 0;
		}
	}
	
}

void Ground3D::LoadVao(void)
{

}

void Ground3D::Render(void)
{

//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//	glBindVertexArray(vao);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element);
	//glDrawElements(GL_TRIANGLES, IndicesCount, GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_PATCHES, IndicesCount, GL_UNSIGNED_INT, 0);
	glDrawArraysInstanced(GL_PATCHES, 0, 4, 1024 * 1024);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);

//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Ground3D::LoadObject3DTextures(void)
{

	//Mossy
	{
		textureID_Ground = MyTGA_Loader::LoadTGA_texture(GroundTextureFilename, true, GL_LINEAR, true);
		textureID_GroundAO = MyTGA_Loader::LoadTGA_texture(GroundAOTextureFilename, true, GL_LINEAR, true);
		textureID_GroundNormal = MyTGA_Loader::LoadTGA_texture(GroundNormalTextureFilename, true, GL_LINEAR, true);
		textureID_GroundRoughnes = MyTGA_Loader::LoadTGA_texture(GroundRoughnessTextureFilename, true, GL_LINEAR, true);
		textureID_GroundDisplacement = MyTGA_Loader::LoadTGA_texture(GroundDisplacementTextureFilename, true, GL_LINEAR, true);
	}
	//Scene1
	{
		textureID_Scene1 = MyTGA_Loader::LoadTGA_texture(Scene1TextureFilename, true, GL_LINEAR, true);
		textureID_Scene1AO = MyTGA_Loader::LoadTGA_texture(Scene1AOTextureFilename, true, GL_LINEAR, true);
		textureID_Scene1Normal = MyTGA_Loader::LoadTGA_texture(Scene1NormalTextureFilename, true, GL_LINEAR, true);
		textureID_Scene1Roughnes = MyTGA_Loader::LoadTGA_texture(Scene1RoughnessTextureFilename, true, GL_LINEAR, true);
		textureID_Scene1Displacement = MyTGA_Loader::LoadTGA_texture(Scene1DisplacementTextureFilename, true, GL_LINEAR, true);
	}

}

vmath::vec3 Ground3D::getLightColor(GLdouble sunTheta)
{
	vmath::vec3 first = vmath::mix(vmath::vec3(0.240f, 0.045f, 0.004f), vmath::vec3(0.343f, 0.131f, 0.000f), (float)smoothstep(0.0f, 0.1f, sunTheta));//sunTheta

	vmath::vec3 second = vmath::mix(first, vmath::vec3(0.897f, 0.786f, 0.519f), (float)smoothstep(0.1f, 0.164f, sunTheta));

	vmath::vec3 last = vmath::mix(second, vmath::vec3(1.0f), (float)smoothstep(0.164f, 0.528f, sunTheta));

	return last;
}
