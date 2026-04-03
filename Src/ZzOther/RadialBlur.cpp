#include "RadialBlur.h"
#include"MyWin32.h"

RadialBlur::RadialBlur() 
{
	TextureID_RadialBlur = 0;

	glGenTextures(1, &TextureID_RadialBlur);
	glBindTexture(GL_TEXTURE_2D, TextureID_RadialBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F,(GLsizei) RadialBlurTextureWidth, (GLsizei)RadialBlurTextureHeight, 0, GL_RED, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	//framebuffer
	Fbo_LightAreaMask = new FrameBuffer(GL_TRUE, 1, GL_FALSE, GL_FALSE, GL_FALSE, GL_RGBA, GL_UNSIGNED_BYTE, GL_FALSE,1, uvec2(1920, 1080));
}

RadialBlur::~RadialBlur()
{
	if (TextureID_RadialBlur)
	{
		glDeleteTextures(1, &TextureID_RadialBlur);
		TextureID_RadialBlur = 0;
	}

	if (Fbo_LightAreaMask)
	{
		delete Fbo_LightAreaMask;
		Fbo_LightAreaMask = nullptr;
	}
}

void RadialBlur::ResizeFramebuffers(GLsizei Width, GLsizei Height)
{
	if(Fbo_LightAreaMask)
	Fbo_LightAreaMask->ResizeFramebuffer(Width, Height);
}

void RadialBlur::UpdateLightAreaMask(GLuint TextureID_SceneMask, GLuint TextureID_CloudMask)
{
	glBindFramebuffer(GL_FRAMEBUFFER, Fbo_LightAreaMask->GetFbo());
	glClear(GL_COLOR_BUFFER_BIT);

	LightAreaMaskShader.Start();

	LightAreaMaskShader.LoadSampler2DSceneMask_Uniform(GL_TEXTURE0, 0, TextureID_SceneMask);
	LightAreaMaskShader.LoadSampler2DCloudMask_Uniform(GL_TEXTURE1, 1, TextureID_CloudMask);
	glDrawArrays(GL_QUADS, 0, 4);

	LightAreaMaskShader.Stop();
}

void RadialBlur::RenderComputeGodRay(GLuint TextureID_LightArea, GLuint TextureID_BlueNoise,vmath::vec2 ScreenSunPos)
{
	GLushort WorkGroupSize = 16;
	vmath::vec2 resolution = vmath::vec2(RadialBlurTextureWidth, RadialBlurTextureHeight);//half resolution

	GodRaysShader.Start();

	GodRaysShader.LoadSampler2D_LightAreaUniform(GL_TEXTURE0, 0, TextureID_LightArea);
	GodRaysShader.LoadSampler2D_BlueNoiseUniform(GL_TEXTURE1, 1, TextureID_BlueNoise);

	GodRaysShader.Loadv2Resolution_Uniform(resolution);
	GodRaysShader.Loadv2ScreenSunPos_Uniform(ScreenSunPos);

	GodRaysShader.LoadfTime_Uniform(MyOpenGL::fTime);
	GodRaysShader.LoadiFrameID_Uniform(MyWin32::iFrameID);

	glBindImageTexture(0, TextureID_RadialBlur, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
	glDispatchCompute(INT_CEIL(resolution[0], WorkGroupSize), INT_CEIL(resolution[1], WorkGroupSize), 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	GodRaysShader.Stop();
}

