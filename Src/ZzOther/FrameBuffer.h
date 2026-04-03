#pragma once
#include"stdafx.h"



class FrameBuffer
{
public:
	FrameBuffer(GLboolean isColorTexture, GLushort colorAttachmentCount,GLboolean isDepth, GLboolean isOnlyDepth, GLboolean isDepthTexture,GLenum internalFormat,GLenum dataType,GLboolean isMSAA,GLuint sampleCount,vmath::uvec2 Resolution);
	~FrameBuffer();

	void ResizeFramebuffer(GLsizei Width,GLsizei Height);

	void DrawBuffers(void);

	GLuint GetFbo(void)
	{
		return Fbo;
	}

	GLuint GetFbo_Color_Texture(GLushort AttachmentIndex)
	{
#pragma push_macro("min")
#undef min
		return Fbo_Color_Texture[vmath::min(AttachmentIndex, ColorAttachmentMaxCount)];
#pragma pop_macro("min")
	}

	GLuint GetFbo_Depth_Texture(void)
	{
		return Fbo_Depth_Texture;
	}
	vmath::uvec2 GetFboResolution(void)
	{
		return v2Resolution;
	}

private:
	const GLushort ColorAttachmentMaxCount = 2;
	GLsizei MultiSampleCount;

	GLushort ColorAttachmentCount;
	GLboolean bColorTexture;
	GLboolean bDepthTexture;
	GLboolean bDepth;
	vmath::uvec2 v2Resolution;

	GLboolean bMSAA;
	GLenum ColorFormat;
	GLenum ColorDataType;

	GLuint Fbo;
	GLuint Fbo_Color_Texture[2];
	GLuint Fbo_Depth_RenderBuffer;
	GLuint Fbo_Color_RenderBuffer[2];
	GLuint Fbo_Depth_Texture;
};

