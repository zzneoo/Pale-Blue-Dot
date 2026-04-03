#include "DefferedRenderer.h"
#include "MyWin32.h"

//#define DEFFERED_WIDTH 7680
//#define DEFFERED_HEIGHT 4320

//#define DEFFERED_WIDTH 1920
//#define DEFFERED_HEIGHT 1080

#define DEFFERED_WIDTH 1
#define DEFFERED_HEIGHT 1

DefferedRenderer::DefferedRenderer()
{
	Fbo_gBuffer = 0;
	TextureID_WorldSpacePosition = 0;
	TextureID_DefferedVelocity = 0;
	TextureID_WorldSpaceNormal = 0;
	TextureID_SpecRoughAlphaDither = 0;
	TextureID_DepthBuffer = 0;

	Initialize();
};

DefferedRenderer::~DefferedRenderer()
{
	if (TextureID_WorldSpacePosition)
	{
		glDeleteTextures(1, &TextureID_WorldSpacePosition);
		TextureID_WorldSpacePosition = 0;
	}
	if (TextureID_DefferedVelocity)
	{
		glDeleteTextures(1, &TextureID_DefferedVelocity);
		TextureID_DefferedVelocity = 0;
	}
	if (TextureID_WorldSpaceNormal)
	{
		glDeleteTextures(1, &TextureID_WorldSpaceNormal);
		TextureID_WorldSpaceNormal = 0;
	}
	if (TextureID_AlbedoAO)
	{
		glDeleteTextures(1, &TextureID_AlbedoAO);
		TextureID_AlbedoAO = 0;
	}
	if (TextureID_SpecRoughAlphaDither)
	{
		glDeleteTextures(1, &TextureID_SpecRoughAlphaDither);
		TextureID_SpecRoughAlphaDither = 0;
	}

	if (TextureID_DepthBuffer)
	{
		glDeleteTextures(1, &TextureID_DepthBuffer);
		TextureID_DepthBuffer = 0;
	}
	if (Fbo_gBuffer)
	{
		glDeleteFramebuffers(1, &Fbo_gBuffer);
		Fbo_gBuffer = 0;
	}
};

void DefferedRenderer::BindDefferedFbo(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, Fbo_gBuffer);
	glViewport(0, 0, DEFFERED_WIDTH, DEFFERED_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3,GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(5, attachments);
}

void DefferedRenderer::Initialize(void)
{

	//
	glGenFramebuffers(1, &Fbo_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, Fbo_gBuffer);

	glGenTextures(1, &TextureID_WorldSpacePosition);
	glBindTexture(GL_TEXTURE_2D, TextureID_WorldSpacePosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, DEFFERED_WIDTH, DEFFERED_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TextureID_WorldSpacePosition, 0);

	glGenTextures(1, &TextureID_WorldSpaceNormal);
	glBindTexture(GL_TEXTURE_2D, TextureID_WorldSpaceNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, DEFFERED_WIDTH, DEFFERED_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, TextureID_WorldSpaceNormal, 0);

	glGenTextures(1, &TextureID_AlbedoAO);
	glBindTexture(GL_TEXTURE_2D, TextureID_AlbedoAO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, DEFFERED_WIDTH, DEFFERED_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, TextureID_AlbedoAO, 0);

	glGenTextures(1, &TextureID_SpecRoughAlphaDither);
	glBindTexture(GL_TEXTURE_2D, TextureID_SpecRoughAlphaDither);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, DEFFERED_WIDTH, DEFFERED_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, TextureID_SpecRoughAlphaDither, 0);

	glGenTextures(1, &TextureID_DefferedVelocity);
	glBindTexture(GL_TEXTURE_2D, TextureID_DefferedVelocity);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, DEFFERED_WIDTH, DEFFERED_HEIGHT, 0, GL_RG, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, TextureID_DefferedVelocity, 0);

	glGenTextures(1, &TextureID_DepthBuffer);
	glBindTexture(GL_TEXTURE_2D, TextureID_DepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, DEFFERED_WIDTH, DEFFERED_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, TextureID_DepthBuffer, 0);

	GLuint attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3,GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(5, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		MessageBox(NULL, TEXT("Framebuffer Failed"), TEXT("Error"), MB_TOPMOST | MB_OK | MB_ICONERROR);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}