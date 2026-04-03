#pragma once
#include "MyObject3D.h"

class CubeMap3D :public MyObject3D
{
public:
	CubeMap3D();
	~CubeMap3D();

	void Render(void)override;

	GLuint GetTextureID_Jungle(void)
	{
		return textureID_Jungle;
	}

private:
	void LoadVao(void)override;
	void LoadObject3DTextures(void)override;

	GLuint textureID_Jungle;

};