#pragma once
#include "MyObject3D.h"
class Ground3D :public MyObject3D
{
public:
	Ground3D();
	~Ground3D();

	void Render(void)override;

	GLfloat* getGroundFactor(void)
	{
		return &fGroundFactor;
	}
	vmath::vec3* Getv3LightColor(void)
	{
		return &v3LightColor;
	}

	//ground-----------------------------------------------------------------------------------------------
	GLuint getTextureID_Ground(void)
	{
		return textureID_Ground;
	}
	GLuint getTextureID_GroundAO(void)
	{
		return textureID_GroundAO;
	}
	GLuint getTextureID_GroundNormal(void)
	{
		return textureID_GroundNormal;
	}
	GLuint getTextureID_GroundRoughness(void)
	{
		return textureID_GroundRoughnes;
	}
	GLuint getTextureID_GroundDisplacement(void)
	{
		return textureID_GroundDisplacement;
	}

	//Scene1-----------------------------------------------------------------------------------------------
	GLuint getTextureID_Scene1(void)
	{
		return textureID_Scene1;
	}
	GLuint getTextureID_Scene1AO(void)
	{
		return textureID_Scene1AO;
	}
	GLuint getTextureID_Scene1Normal(void)
	{
		return textureID_Scene1Normal;
	}
	GLuint getTextureID_Scene1Roughness(void)
	{
		return textureID_Scene1Roughnes;
	}
	GLuint getTextureID_Scene1Displacement(void)
	{
		return textureID_Scene1Displacement;
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
	//Ground
	GLuint textureID_Ground;
	GLuint textureID_GroundAO;
	GLuint textureID_GroundNormal;
	GLuint textureID_GroundRoughnes;
	GLuint textureID_GroundDisplacement;
	//Scene1
	GLuint textureID_Scene1;
	GLuint textureID_Scene1AO;
	GLuint textureID_Scene1Normal;
	GLuint textureID_Scene1Roughnes;
	GLuint textureID_Scene1Displacement;
	
	//Functions..............
	void LoadVao(void)override;
	void LoadObject3DTextures(void)override;

	 const GLfloat groundScale = 10.0f;//ndc patch culling depends on scale factor

	 GLfloat fGroundFactor;//for GUI
	 vmath::vec3 v3LightColor;//for GUI

	//ground
	const GLchar* GroundTextureFilename = { "Resources/Textures/Ground/Mossy/Albedo.tga" };
	const GLchar* GroundNormalTextureFilename = { "Resources/Textures/Ground/Mossy/Normal.tga" };
	const GLchar* GroundAOTextureFilename = { "Resources/Textures/Ground/Mossy/AO.tga" };
	const GLchar* GroundRoughnessTextureFilename = { "Resources/Textures/Ground/Mossy/Rough.tga" };
	const GLchar* GroundDisplacementTextureFilename = { "Resources/Textures/Ground/Mossy/Disp.tga" };	

	//grass
	//const GLchar* GroundTextureFilename = { "Resources/Textures/Ground/Grass/Albedo.tga" };
	//const GLchar* GroundNormalTextureFilename = { "Resources/Textures/Ground/Grass/Normal.tga" };
	//const GLchar* GroundAOTextureFilename = { "Resources/Textures/Ground/Grass/AO.tga" };
	//const GLchar* GroundRoughnessTextureFilename = { "Resources/Textures/Ground/Grass/Rough.tga" };
	//const GLchar* GroundDisplacementTextureFilename = { "Resources/Textures/Ground/Grass/Disp.tga" };
	
	//scene1
	const GLchar* Scene1TextureFilename = { "Resources/Textures/Ground/scene1/Albedo.tga" };
	const GLchar* Scene1NormalTextureFilename = { "Resources/Textures/Ground/scene1/Normal.tga" };
	const GLchar* Scene1AOTextureFilename = { "Resources/Textures/Ground/scene1/AO.tga" };
	const GLchar* Scene1RoughnessTextureFilename = { "Resources/Textures/Ground/scene1/Rough.tga" };
	const GLchar* Scene1DisplacementTextureFilename = { "Resources/Textures/Ground/scene1/Disp.tga" };

	
};

