#pragma once
#include"MyObject3D.h"

class Triangle3D :public MyObject3D
{
public:
	Triangle3D() ;
	~Triangle3D() ;

	void Render(void)override;

private:
	void LoadVao(void)override;
	void LoadObject3DTextures(void)override {}

};