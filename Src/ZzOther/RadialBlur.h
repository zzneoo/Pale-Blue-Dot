#pragma once
#include"stdafx.h"
#include"PostProcessor.h"
#include"GodRaysShaderLoader.h"
#include"FrameBuffer.h"
#include"TemporalPassShaderLoader.h"
#include"LightAreaMaskShaderLoader.h"

class RadialBlur 
{
public:
	RadialBlur();
	~RadialBlur();

	GLuint GetTextureID_RadialBlur(void)
	{
		return TextureID_RadialBlur;
	}

	GLuint GetTextureID_LightAreaMask(void)
	{
		return Fbo_LightAreaMask->GetFbo_Color_Texture(0);
	}

	//functions
	void ResizeFramebuffers(GLsizei Width, GLsizei Height);

	void RenderComputeGodRay(GLuint TextureID_LightArea, GLuint TextureID_BlueNoise, vmath::vec2 ScreenSunPos);
	void UpdateLightAreaMask(GLuint TextureID_SceneMask, GLuint TextureID_CloudMask);


private:
	//variables
	const GLfloat RadialBlurTextureWidth = 960;
	const GLfloat RadialBlurTextureHeight = 540;

	//shaderLoader
	GodRaysShaderLoader GodRaysShader;
	LightAreaMaskShaderLoader LightAreaMaskShader;

	//textures
	GLuint TextureID_RadialBlur;
	FrameBuffer* Fbo_LightAreaMask;
};

