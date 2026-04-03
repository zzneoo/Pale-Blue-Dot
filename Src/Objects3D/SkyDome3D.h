#pragma once
#include "MyObject3D.h"

class SkyDome3D :	public MyObject3D
{
public:
	SkyDome3D();
	~SkyDome3D();

	void Render(void)override;

	GLfloat GetfOuterRadius(void)
	{
		return fOuterRadius;
	}
	GLfloat GetfInnerRadius(void)
	{
		return fInnerRadius;
	}
	GLfloat GetfScale(void)
	{
		return fScale;
	}

	GLfloat GetfScaleDepth(void)
	{
		return fScaleDepth;
	}
	GLfloat GetfScaleOverScaleDepth(void)
	{
		return fScaleOverScaleDepth;
	}

	GLfloat GetfKrESun(void)
	{
		return fKrESun;
	}
	GLfloat GetfKmESun(void)
	{
		return fKmESun;
	}
	GLfloat GetfKr4PI(void)
	{
		return fKr4PI;
	}
	GLfloat GetfKm4PI(void)
	{
		return fKm4PI;
	}
	vmath::vec3 Getv3InvWavelength(void)
	{
		return v3InvWavelength;
	}

	GLfloat* GetfSkyFactor(void)
	{
		return &fSkyFactor;
	}

private:
	//Functions
	void LoadVao(void)override;
	void LoadObject3DTextures(void)override;

	//Class Variables mesh
	GLfloat fSkyFactor;

	const GLint QuarterDivider = 8;//for making hemisphere from sphere   //divisible to stacks

	const GLint slices =128;
	const GLint stacks = 128;
	const GLint adjustStack = 5;//6

	const GLsizei MAX_SPHERE_X = ((2 * slices) + 1);
	const GLsizei MAX_SPHERE_Y = (stacks + 1);

	const GLsizei indicesCount = 6 * (MAX_SPHERE_X - 1) *(((MAX_SPHERE_Y/ QuarterDivider)- adjustStack) -1);

	//for scattering
	const GLfloat ESun = 21.0f;										//Sun Energy
	const GLfloat fKr = 0.0045f;									//Rayleigh Constant
	const GLfloat fKm = 0.0010f;									//Mie Constant

	//for const Scattering Uniforms
	const GLfloat fScaleDepth = 0.25f;

	const GLfloat fInnerRadius = 10.0f;												// The inner (planetary) radius
	const GLfloat fOuterRadius = fInnerRadius+ fScaleDepth;							// The outer (atmosphere) radius
								
	const GLfloat fScale = 1 / (fOuterRadius - fInnerRadius);				// 1 / (fOuterRadius - fInnerRadius)										// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
	const GLfloat fScaleOverScaleDepth = fScale / fScaleDepth;				// fScale / fScaleDepth

	const GLfloat fKrESun = fKr * ESun;									// Kr * ESun
	const GLfloat fKmESun = fKm * ESun;									// Km * ESun

	const GLfloat fKr4PI = (GLfloat)(fKr * 4 * M_PI);					// Kr * 4 * PI
	const GLfloat fKm4PI = (GLfloat)(fKm * 4 * M_PI);;					// Km * 4 * PI

	const vmath::vec3 v3InvWavelength = vmath::vec3((GLfloat)(1 / pow(0.650, 4)), (GLfloat)(1 / pow(0.570, 4)), (GLfloat)(1 / pow(0.475, 4)));
};

