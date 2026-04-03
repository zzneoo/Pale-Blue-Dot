#pragma once
#include"MyObject3D.h"

class ParticleSystem :public MyObject3D
{
public:
	ParticleSystem();
	~ParticleSystem();

	GLuint getTextureID_WindParticle(void)
	{
		return TextureID_WindParticle;
	}

	void Render(void)override;

private:
	void LoadVao(void)override;
	void LoadObject3DTextures(void)override;

	GLuint TextureID_WindParticle;
	const GLuint ParticleCount = 32;
	const GLfloat ParticleSize = 1200;

	vmath::vec4* particles;
};
