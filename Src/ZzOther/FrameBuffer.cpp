#include "FrameBuffer.h"
//#include "MyWin32.h"

#define SHADOWMAP_SIZE 8192

FrameBuffer::FrameBuffer(GLboolean isColorTexture, GLushort colorAttachmentCount, GLboolean isDepth, GLboolean isOnlyDepth, GLboolean isDepthTexture, GLenum internalFormat, GLenum dataType, GLboolean isMSAA, GLuint sampleCount ,vmath::uvec2 Resolution) :bColorTexture(isColorTexture), ColorAttachmentCount(colorAttachmentCount), bDepth(isDepth), bDepthTexture(isDepthTexture),ColorFormat(internalFormat),ColorDataType(dataType),bMSAA(isMSAA), MultiSampleCount(sampleCount) , v2Resolution(Resolution)
{
	Fbo = 0;
	Fbo_Color_Texture[0] = 0;
	Fbo_Color_Texture[1] = 0;
	Fbo_Color_RenderBuffer[0] = 0;
	Fbo_Color_RenderBuffer[1] = 0;

	Fbo_Depth_RenderBuffer = 0;
	Fbo_Depth_Texture = 0;

	glGenFramebuffers(1, &Fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, Fbo);

	//COLOR
	//TEXTURES
	if (isOnlyDepth)//shadows mostly
	{

		if (bDepthTexture)
		{
			glGenTextures(1, &Fbo_Depth_Texture);
			glBindTexture(GL_TEXTURE_2D, Fbo_Depth_Texture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glBindTexture(GL_TEXTURE_2D, 0);

			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Fbo_Depth_Texture, 0);
		}
		else
		{
			//depth renderbuffer
			glGenRenderbuffers(1, &Fbo_Depth_RenderBuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, Fbo_Depth_RenderBuffer);

			if (bMSAA)
			{
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, MultiSampleCount, GL_DEPTH_COMPONENT32F, SHADOWMAP_SIZE, SHADOWMAP_SIZE);
			}
			else
			{
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, SHADOWMAP_SIZE, SHADOWMAP_SIZE);
			}
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, Fbo_Depth_RenderBuffer);
		}
	
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		ResizeFramebuffer(SHADOWMAP_SIZE, SHADOWMAP_SIZE);

	}
	else
	{
		if (isColorTexture)
		{
			glGenTextures(colorAttachmentCount, Fbo_Color_Texture);
			for (int i = 0; i < colorAttachmentCount; i++)
			{
				glBindTexture(GL_TEXTURE_2D, Fbo_Color_Texture[i]);

				if (ColorFormat == GL_RG16F)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, ColorFormat, Resolution[0], Resolution[1], 0, GL_RG, ColorDataType, NULL);
				}
				else if(ColorFormat == GL_R32F)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, ColorFormat, Resolution[0], Resolution[1], 0, GL_RED, ColorDataType, NULL);
				}
				else
				{				
					glTexImage2D(GL_TEXTURE_2D, 0, ColorFormat, Resolution[0], Resolution[1], 0, GL_RGBA, ColorDataType, NULL);
				}


				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glBindTexture(GL_TEXTURE_2D, 0);

				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, Fbo_Color_Texture[i], 0);
			}
		}
		else
		{
			//RENDERBUFFERS
			glGenRenderbuffers(colorAttachmentCount, Fbo_Color_RenderBuffer);
			for (int i = 0; i < colorAttachmentCount; i++)
			{
				glBindRenderbuffer(GL_RENDERBUFFER, Fbo_Color_RenderBuffer[i]);

				if (bMSAA)
				{
					glRenderbufferStorageMultisample(GL_RENDERBUFFER, MultiSampleCount, ColorFormat, Resolution[0], Resolution[1]);
				}
				else
				{
					glRenderbufferStorage(GL_RENDERBUFFER, ColorFormat, Resolution[0], Resolution[1]);
				}
				glBindRenderbuffer(GL_RENDERBUFFER, 0);

				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, Fbo_Color_RenderBuffer[i]);
			}
		}
		//depth texture
		if (bDepth)
		{
			if (bDepthTexture)
			{
				glGenTextures(1, &Fbo_Depth_Texture);
				glBindTexture(GL_TEXTURE_2D, Fbo_Depth_Texture);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, Resolution[0], Resolution[1], 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glBindTexture(GL_TEXTURE_2D, 0);

				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Fbo_Depth_Texture, 0);
			}
			else
			{
				//depth renderbuffer
				glGenRenderbuffers(1, &Fbo_Depth_RenderBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, Fbo_Depth_RenderBuffer);

				if (bMSAA)
				{
					glRenderbufferStorageMultisample(GL_RENDERBUFFER, MultiSampleCount, GL_DEPTH_COMPONENT32, Resolution[0], Resolution[1]);
				}
				else
				{
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, Resolution[0], Resolution[1]);
				}
				glBindRenderbuffer(GL_RENDERBUFFER, 0);

				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, Fbo_Depth_RenderBuffer);
			}
		}

		DrawBuffers();
	}




	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		MessageBox(NULL, TEXT("Framebuffer Failed"), TEXT("Error"), MB_TOPMOST | MB_OK | MB_ICONERROR);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{

	for (int i = 0; i < ColorAttachmentCount; i++)
	{
		if (Fbo_Color_Texture[i])
		{
			glDeleteTextures(1, &Fbo_Color_Texture[i]);
			Fbo_Color_Texture[i] = 0;
		}
	}

	if (Fbo_Depth_Texture)
	{
		glDeleteTextures(1, &Fbo_Depth_Texture);
		Fbo_Depth_Texture = 0;
	}


	//RenderBuffers
	for (int i = 0; i < ColorAttachmentCount; i++)
	{
		if (Fbo_Color_RenderBuffer[i])
		{
			glDeleteRenderbuffers(1, &Fbo_Color_RenderBuffer[i]);
			Fbo_Color_RenderBuffer[i] = 0;
		}
	}

	if (Fbo_Depth_RenderBuffer)
	{
		glDeleteRenderbuffers(1, &Fbo_Depth_RenderBuffer);
		Fbo_Depth_RenderBuffer = 0;
	}

	//FBO
	if (Fbo)
	{
		glDeleteFramebuffers(1, &Fbo);
		Fbo = 0;
	}
}

void FrameBuffer::DrawBuffers(void)
{
	if (ColorAttachmentCount == 1)
	{
		GLenum DrawStrings1[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawStrings1);
	}
	else if (ColorAttachmentCount == 2)
	{
		GLenum DrawStrings2[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, DrawStrings2);
	}
}

void FrameBuffer::ResizeFramebuffer(GLsizei Width,GLsizei Height)
{
	if (bColorTexture)
	{
		for (int i = 0; i < ColorAttachmentCount; i++)
		{
			glBindTexture(GL_TEXTURE_2D, Fbo_Color_Texture[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, ColorFormat, Width, Height, 0, GL_RGBA, ColorDataType, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	else
	{
		for (int i = 0; i < ColorAttachmentCount; i++)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, Fbo_Color_RenderBuffer[i]);
			if (bMSAA)
			{
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, MultiSampleCount, ColorFormat, Width, Height);
			}
			else
			{
				glRenderbufferStorage(GL_RENDERBUFFER, ColorFormat, Width, Height);
			}
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
	}

	if (Fbo_Depth_Texture)
	{
		glBindTexture(GL_TEXTURE_2D, Fbo_Depth_Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if(Fbo_Depth_RenderBuffer)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, Fbo_Depth_RenderBuffer);
		if (bMSAA)
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, MultiSampleCount, GL_DEPTH_COMPONENT24, Width, Height);
		}
		else
		{
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, Width, Height);
		}
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

}

