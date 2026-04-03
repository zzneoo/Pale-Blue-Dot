#include "stdafx.h"
#include "TAA.h"
#include"MyOpenGL.h"
#include "MyWin32.h"

TAA::TAA()
{
	VelocityMaxTexture = 0;
	CameraVelocityTexture = 0;
	TemporalUpscaled4kTexture = 0;
	AntiAliasedTexture = 0;

	//Initialize();

}

TAA::~TAA()
{


	if (AntiAliasedTexture)
	{
		glDeleteTextures(1, &AntiAliasedTexture);
		AntiAliasedTexture = 0;
	}

	if (TemporalUpscaled4kTexture)
	{
		glDeleteTextures(1, &TemporalUpscaled4kTexture);
		TemporalUpscaled4kTexture = 0;
	}
	if (TemporalUpscaled4kTempTexture)
	{
		glDeleteTextures(1, &TemporalUpscaled4kTempTexture);
		TemporalUpscaled4kTempTexture = 0;
	}
	if (VelocityMaxTexture)
	{
		glDeleteTextures(1, &VelocityMaxTexture);
		VelocityMaxTexture = 0;
	}
	if (CameraVelocityTexture)
	{
		glDeleteTextures(1, &CameraVelocityTexture);
		CameraVelocityTexture = 0;
	}
	
}

void TAA::Initialize(void)
{

	//Previous
	glGenTextures(1, &AntiAliasedTexture);
	glBindTexture(GL_TEXTURE_2D, AntiAliasedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

#ifdef TEMPORAL_UPSCALE_8K
	//8k
	glGenTextures(1, &TemporalUpscaled4kTexture);
	glBindTexture(GL_TEXTURE_2D, TemporalUpscaled4kTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 3840 * 2, 2160 * 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

#else
	//4k
	glGenTextures(1, &TemporalUpscaled4kTexture);
	glBindTexture(GL_TEXTURE_2D, TemporalUpscaled4kTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 3840 , 2160 , 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

#endif // TEMPORAL_UPSCALE_8K



	//4k Temp for ComputeShader reprojection
	glGenTextures(1, &TemporalUpscaled4kTempTexture);
	glBindTexture(GL_TEXTURE_2D, TemporalUpscaled4kTempTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 3840, 2160, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);


	//VelocityMaxTexture
	glGenTextures(1, &VelocityMaxTexture);
	glBindTexture(GL_TEXTURE_2D, VelocityMaxTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, 1920, 1080, 0, GL_RG, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	//CameraVelocity
	glGenTextures(1, &CameraVelocityTexture);
	glBindTexture(GL_TEXTURE_2D, CameraVelocityTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, 1920, 1080, 0, GL_RG, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TAA::CalculateVelocity(vmath::mat4 CameraViewMatrix, GLuint CurrentDepthTexture,vmath::vec2 Halton23,vmath::vec3 EyePos)
{
//	GLushort WorkGroupSize = 16;
//	vmath::vec2 resolution = vmath::vec2(1920.0, 1080.0);//

	VelocityShader.Start();

	VelocityShader.Loadm4ProjX_Uniform(MyWin32::gProjectionMatrix);
	VelocityShader.Loadm4InvProjX_Uniform(MyWin32::gInvProjectionMatrix);
	VelocityShader.Loadm4InvViewX_Uniform(MyWin32::gInvViewMatrix);
	VelocityShader.Loadm4PrevViewX_Uniform(MyWin32::gPreviousViewMatrix);

	VelocityShader.Loadv2Halton23_Uniform(Halton23);
//	VelocityShader.Loadv2NearFar_Uniform(MyWin32::gNearFarFrustum);

	VelocityShader.Loadv3EyePos_Uniform(EyePos);

	VelocityShader.LoadSampler2DCurrDepth_Uniform(GL_TEXTURE0, 0, CurrentDepthTexture);

//	glBindImageTexture(0, VelocityTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG16F);

//	glDispatchCompute(INT_CEIL(resolution[0], WorkGroupSize), INT_CEIL(resolution[1], WorkGroupSize), 1);

//	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glDrawArrays(GL_QUADS, 0, 4);

	VelocityShader.Stop();


}

void TAA::CalculateMaxVelocity(GLuint CurrVeclocityTexture)
{
	GLushort WorkGroupSize = 16;
	vmath::vec2 resolution = vmath::vec2(1920.0, 1080.0);//

	VelocityMaxShader.Start();

	VelocityMaxShader.LoadSampler2DCurrVelocity_Uniform(GL_TEXTURE0, 0, CurrVeclocityTexture);

	glBindImageTexture(0, VelocityMaxTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG32F);

	glDispatchCompute(INT_CEIL(resolution[0], WorkGroupSize), INT_CEIL(resolution[1], WorkGroupSize), 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);


	VelocityMaxShader.Stop();
}

void TAA::TemporalAntiAlias( vmath::mat4 CameraViewMatrix , GLuint CurrentFrame , GLuint CurrentDepthTexture,vmath::vec2 CurrHalton23, vmath::vec2 PrevHalton23)
{
	//GLushort WorkGroupSize = 16;
	vmath::vec2 resolution = vmath::vec2(3840.0, 2160.0);//


	TemporalAntiAliasingShader.Start();

	TemporalAntiAliasingShader.LoadfTime_Uniform(MyOpenGL::fTime);
	TemporalAntiAliasingShader.LoadbIsTAA_Uniform(MyWin32::TAA);
	TemporalAntiAliasingShader.Loadv4Halton23_Uniform(vmath::vec4(CurrHalton23, PrevHalton23));
	TemporalAntiAliasingShader.Loadm4ProjX_Uniform(MyWin32::gProjectionMatrix);
	TemporalAntiAliasingShader.Loadm4ViewX_Uniform(CameraViewMatrix);
	TemporalAntiAliasingShader.Loadm4PrevViewX_Uniform(MyWin32::gPreviousViewMatrix);

	TemporalAntiAliasingShader.LoadSampler2DCurrFrameNearest_Uniform(GL_TEXTURE0, 0, CurrentFrame);
	TemporalAntiAliasingShader.LoadSampler2DCurrDepth_Uniform(GL_TEXTURE1, 1, CurrentDepthTexture);
	TemporalAntiAliasingShader.LoadSampler2DCurrVelocity_Uniform(GL_TEXTURE2, 2, VelocityMaxTexture);
	TemporalAntiAliasingShader.LoadSampler2DPrevFrame_Uniform(GL_TEXTURE3, 3, AntiAliasedTexture);

//	glBindImageTexture(0, AntiAliasedTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

//	glDispatchCompute(INT_CEIL(resolution[0], WorkGroupSize), INT_CEIL(resolution[1], WorkGroupSize), 1);

//	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glDrawArrays(GL_QUADS, 0, 4);

	TemporalAntiAliasingShader.Stop();

}


void TAA::TemporalUpscale(vmath::mat4 CameraViewMatrix, GLuint CurrentFrame, GLuint CurrentDepthTexture, GLuint iBlockPixelID)
{
	//vmath::vec2 resolution = vmath::vec2(3840.0, 2160.0);//
	//GLushort WorkGroupSize = 8;

	TemporalUpscale4kShader.Start();

#ifdef TEMPORAL_UPSCALE_8K
	TemporalUpscale4kShader.LoadiBlockWidth_Uniform(4);
	TemporalUpscale4kShader.Loadv2Resolution_Uniform(vmath::vec2(3840.0, 2160.0) * 2.0f);
#else
	TemporalUpscale4kShader.LoadiBlockWidth_Uniform(2);
	TemporalUpscale4kShader.Loadv2Resolution_Uniform(vmath::vec2(3840.0, 2160.0));
#endif // TEMPORAL_UPSCALE_8K

	TemporalUpscale4kShader.LoadiBlockPixelID_Uniform(iBlockPixelID);
	TemporalUpscale4kShader.LoadSampler2DCurrFrameLinear_Uniform(GL_TEXTURE0,0,CurrentFrame);
	TemporalUpscale4kShader.LoadSampler2DCurrVelocity_Uniform(GL_TEXTURE1, 1, VelocityMaxTexture);
	TemporalUpscale4kShader.LoadSampler2DPrevFrame_Uniform(GL_TEXTURE2, 2, TemporalUpscaled4kTexture);

//	glBindImageTexture(0, TemporalUpscaled4kTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

//	glDispatchCompute(INT_CEIL(resolution[0], WorkGroupSize), INT_CEIL(resolution[1], WorkGroupSize), 1);

//	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glDrawArrays(GL_QUADS, 0, 4);

	TemporalUpscale4kShader.Stop();
}

//void Clouds::RenderTemporalUpscaleQuad(vmath::mat4 ViewMatrix, vmath::vec3 CameraPos)
//{
//
//	glBindFramebuffer(GL_FRAMEBUFFER, Fbo_TemporalUpscale->GetFbo());
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	temporalPassShader.Start();
//
//	temporalPassShader.LoadSampler2DCurrFrameNearest_Uniform(GL_TEXTURE0, 0, CloudCurrFrameNearestTexture);//quarter res
//	temporalPassShader.LoadSampler2DCurrFrameLinear_Uniform(GL_TEXTURE1, 1, CloudCurrFrameLinearTexture);//quarter res
//
//	temporalPassShader.LoadSampler2DPrevFrame_Uniform(GL_TEXTURE2, 2, CloudPreviousFrameTexture);//full res
//	temporalPassShader.LoadSampler2DPrevDepth_Uniform(GL_TEXTURE3, 3, CloudPreviousDepthTexture);//full res
//
//	temporalPassShader.LoadSampler2DCurrVelocity_Uniform(GL_TEXTURE4, 4, CloudVelocityTexture);//quarter res
//	temporalPassShader.LoadSampler2DCurrDepth_Uniform(GL_TEXTURE5, 5, CloudDepthTexture);//quarter res
//
//	temporalPassShader.Loadv3CameraPosition_Uniform(CameraPos);
//
//	temporalPassShader.Loadm4ProjX_Uniform(MyWin32::gProjectionMatrix);
//	temporalPassShader.Loadm4PrevViewX_Uniform(MyWin32::gPreviousViewMatrix);
//	temporalPassShader.Loadm4ViewX_Uniform(ViewMatrix);
//
//	temporalPassShader.LoadfTime_Uniform(MyOpenGL::fTime);
//	temporalPassShader.LoadiBlockPixelID_Uniform(MyWin32::iBlockPixelID);
//
//	glDrawArrays(GL_QUADS, 0, 4);
//
//	temporalPassShader.Stop();
//
//	ResolveToPreviousFrame();
//}

void TAA::ResolveToPreviousFrame(GLuint currTexture,GLuint prevTexture,vmath::uvec2 Res)
{
	//glCopyImageSubData(Fbo_Temporal->GetFbo_Color0_Texture(), GL_TEXTURE_2D, 0, 0, 0, 0, CloudPreviousFrameTexture, GL_TEXTURE_2D, 0, 0, 0, 0, MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight, 1);

	//previous frame
	glCopyImageSubData(currTexture,
		GL_TEXTURE_2D, 0, 0, 0, 0,
		prevTexture,
		GL_TEXTURE_2D, 0, 0, 0, 0,
		Res[0], Res[1], 1);

}
