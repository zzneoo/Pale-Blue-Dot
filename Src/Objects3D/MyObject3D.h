#pragma once
#include"stdafx.h"
#include"MyTGA_Loader.h"
enum
{
	ZZNEO_ATTRIB_POSITION = 0,
	ZZNEO_ATTRIB_TEXCOORD = 1,
	ZZNEO_ATTRIB_NORMAL = 2,
	ZZNEO_ATTRIB_TANGENT = 3,
	ZZNEO_ATTRIB_JOINT_INDICES = 4,
	ZZNEO_ATTRIB_JOINT_WEIGHTS = 5,
	ZZNEO_ATTRIB_COLOR = 6,
	ZZNEO_ATTRIB_VELOCITY = 7,
};

typedef struct InstanceModelData
{
	GLuint CurrentInstanceCount;
	GLuint maxInstancesCount;
	vmath::mat4* InstanceModelMatrices;

}InstanceModelData;

class MyObject3D
{
public:
	MyObject3D();
	virtual ~MyObject3D();

	virtual void Render(void) = 0;

	GLuint getHeightMapTextureID(void)
	{
		return textureID_Height;
	}

	GLuint getNormalMapTextureID(void)
	{
		return textureID_Normal;
	}
	GLuint getDisplacementMapTextureID(void)
	{
		return textureID_Displacement;
	}
	GLuint getAmbientOcclusionMapTextureID(void)
	{
		return textureID_AmbientOcclusion;
	}

	GLuint getAlbedoMapTextureID(void)
	{
		return textureID_Albedo;
	}
	GLuint getRoughnessMapTextureID(void)
	{
		return textureID_Roughness;
	}
	GLuint getSubsurfaceMapTextureID(void)
	{
		return textureID_Subsurface;
	}

	vmath::mat4 GetModelMatrix(void)
	{
		return ModelMatrix;
	}
	void SetModelMatrix(vmath::vec3 T,GLfloat rotationAngle, vmath::vec3 R, vmath::vec3 S)
	{
		ModelMatrix = vmath::translate(T)*vmath::rotate(rotationAngle, R[0], R[1], R[2])* vmath::scale(S);
	}

	//Data
	GLuint GetVao(void)
	{
		return vao;
	}

private:
	//Functions
	void CleanUp_Object3D(void);
	//Class Members
	vmath::mat4 ModelMatrix;
protected:
	//Functions
	virtual void LoadVao(void) = 0;
	virtual void LoadObject3DTextures(void) = 0;

	//Class Members

	GLuint vao;
	GLuint vbo;
	GLuint vboPerInstanceModelX;
	GLuint vbo_element;

	GLuint textureID_Albedo;
	GLuint textureID_Normal;
	GLuint textureID_Displacement;
	GLuint textureID_Height;
	GLuint textureID_AmbientOcclusion;
	GLuint textureID_Roughness;
	GLuint textureID_Subsurface;
};
