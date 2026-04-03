#pragma once
#include"MyObject3D.h"

class RainParticleSystem :public MyObject3D
{
public:
	RainParticleSystem();
	~RainParticleSystem();

	GLuint getTextureID_RainParticle(void)
	{
		return TextureID_RainParticle;
	}

	void Render(void)override;

private:
	void LoadVao(void)override;
	void LoadObject3DTextures(void)override;

	GLuint TextureID_RainParticle;
	const GLuint ParticleCount = 512;
	const GLfloat ParticleSize = 25;

	vmath::vec4* particles;
};
