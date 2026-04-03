#pragma once
#include "MyObject3D.h"
class Quad3D :	public MyObject3D
{
public:
	Quad3D();
	~Quad3D();

	void Render(void)override;

private:
	//Functions..............
	void LoadVao(void)override;
	void LoadObject3DTextures(void)override;//pure virtual

	//Class Variables

};

