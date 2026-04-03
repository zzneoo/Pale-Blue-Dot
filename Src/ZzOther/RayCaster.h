#pragma once
#include"stdafx.h"

class RayCaster
{
public:
	RayCaster()
	{
		fRotAngleX = 0;;
		fRotAngleY = 0;;
		fRotAngleZ = 0;;
	};
	~RayCaster() {};

	void getWorldSpacePosFromScreenSpace(void);
	float linearize_depth(float d, float zNear, float zFar)
	{
		float z_n = 2.0f * d - 1.0f;
		return 2.0f * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
	}

	//varibles
	static GLfloat fUniformScale;

	static vmath::vec3 WorldPos;

	static GLfloat fRotAngleX;
	static GLfloat fRotAngleY;
	static GLfloat fRotAngleZ;

private:
	//functions

	vmath::vec3 ViewportToNDC(GLfloat depth);
	vmath::vec4 NDC_SpaceToWorldSpace(vmath::vec3 NDC);

};

