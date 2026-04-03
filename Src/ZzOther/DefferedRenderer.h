#pragma once
#include"stdafx.h"

class DefferedRenderer
{
public:
	DefferedRenderer();
	~DefferedRenderer();

	void BindDefferedFbo(void);

	GLuint GetTextureID_WorldSpacePosition(void)
	{
		return TextureID_WorldSpacePosition;
	}
	GLuint GetTextureID_DefferedVelocity(void)
	{
		return TextureID_DefferedVelocity;
	}
	GLuint GetTextureID_WorldSpaceNormal(void)
	{
		return TextureID_WorldSpaceNormal;
	}
	GLuint GetTextureID_AlbedoAO(void)
	{
		return TextureID_AlbedoAO;
	}
	GLuint GetTextureID_SpecRoughAlphaDither(void)
	{
		return TextureID_SpecRoughAlphaDither;
	}

	GLuint GetFbo_gBuffer(void)
	{
		return Fbo_gBuffer;
	}

	GLuint GetTextureID_DepthBuffer(void)
	{
		return TextureID_DepthBuffer;
	}

private:

	GLuint Fbo_gBuffer;
	GLuint TextureID_WorldSpacePosition;//Float32
	GLuint TextureID_DefferedVelocity;//Float32
	GLuint TextureID_AlbedoAO;//UnsignedByte8
	GLuint TextureID_SpecRoughAlphaDither;//UnsignedByte8
	GLuint TextureID_WorldSpaceNormal;//Float32
	GLuint TextureID_DepthBuffer;//32

	void Initialize(void);

};

