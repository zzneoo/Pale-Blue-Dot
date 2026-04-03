#include"stdafx.h"
#include"ParticleSystem.h"
#include"MyOpenGL.h"

ParticleSystem::ParticleSystem()
{
	LoadVao();
	LoadObject3DTextures();
}
ParticleSystem::~ParticleSystem()
{
	if (particles)
	{
		delete[] particles;
		particles = nullptr;
	}

	if (TextureID_WindParticle)
	{
		glDeleteTextures(1, &TextureID_WindParticle);
		TextureID_WindParticle = 0;
	}
}

void ParticleSystem::LoadObject3DTextures(void)
{
	TextureID_WindParticle = MyTGA_Loader::LoadTGA_texture("Resources/Textures/Particle/smoke.tga", false, GL_LINEAR, false);
}

void ParticleSystem::LoadVao(void)
{

	particles = new vmath::vec4[ParticleCount];

	for (GLuint i = 0; i < ParticleCount; i++)
	{
		particles[i][0] = (((float)rand() / RAND_MAX) * 2.0f - 1.0f) * 5120.0f;
		particles[i][1] = (((float)rand() / RAND_MAX) ) * 3200.0f;
		particles[i][2] = (((float)rand() / RAND_MAX) * 2.0f - 1.0f) * 10120.0f;
		particles[i][3] = 1.0f;
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vmath::vec4)* ParticleCount, particles, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(ZZNEO_ATTRIB_POSITION, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(ZZNEO_ATTRIB_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


}

void ParticleSystem::Render(void)
{

	glPointSize(ParticleSize);

	for (GLuint i = 0; i < ParticleCount; i++)
	{
		particles[i][2] -= MyOpenGL::fDeltaTime*5000.0f;

		if (particles[i][2] < -10120.0f)
		{
			particles[i][2] = 10120.0f;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vmath::vec4) * ParticleCount, particles, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnable(GL_POINT_SPRITE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, ParticleCount);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}
