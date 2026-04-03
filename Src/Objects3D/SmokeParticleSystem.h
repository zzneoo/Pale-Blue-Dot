#pragma once
#include"MyObject3D.h"

class SmokeParticleSystem :public MyObject3D
{
public:
	SmokeParticleSystem();
	~SmokeParticleSystem();

	GLuint getTextureID_WindParticle(void)
	{
		return TextureID_WindParticle;
	}

	void Render(void)override;

private:
	void LoadVao(void)override;
	void LoadObject3DTextures(void)override;

	GLuint TextureID_WindParticle;
	const GLuint ParticleCount = 1024;
	const GLfloat ParticleSize = 256;

	vmath::vec4* particles;
};
