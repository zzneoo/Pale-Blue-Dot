#pragma once
#include"stdafx.h"
#include"TemporalAntiAliasingShaderLoader.h"
#include"TemporalUpscale4kShaderLoader.h"
#include"VelocityShaderLoader.h"
#include"VelocityMaxShaderLoader.h"

class TAA
{
public:
	TAA();
	~TAA();

	GLuint GetAntiAliasedTexture(void)
	{
		return AntiAliasedTexture;
	}
	GLuint GetTemporalUpscaled4kTexture(void)
	{
		return TemporalUpscaled4kTexture;
	}
	GLuint GetTemporalUpscaled4kTempTexture(void)
	{
		return TemporalUpscaled4kTempTexture;
	}

	GLuint GetVelocityMaxTexture(void)
	{
		return VelocityMaxTexture;
	}
	GLuint GetCameraVelocityTexture(void)
	{
		return CameraVelocityTexture;
	}

	void ResolveToPreviousFrame(GLuint currTexture, GLuint prevTexture, vmath::uvec2 Res);
	void TemporalAntiAlias(vmath::mat4 CameraViewMatrix, GLuint CurrentFrame, GLuint CurrentDepthTexture, vmath::vec2 halton23, vmath::vec2 PrevHalton23);
	void TemporalUpscale(vmath::mat4 CameraViewMatrix, GLuint CurrentFrame, GLuint CurrentDepthTexture,GLuint iBlockPixelID);
	void CalculateVelocity(vmath::mat4 CameraViewMatrix, GLuint CurrentDepthTexture, vmath::vec2 Halton23, vmath::vec3 EyePos);
	void CalculateMaxVelocity(GLuint CurrentVelocityTexture);

private:
	//functions
	void Initialize(void);

	//shaderloaders
	TemporalAntiAliasingShaderLoader TemporalAntiAliasingShader;
	TemporalUpscale4kShaderLoader TemporalUpscale4kShader;
	VelocityShaderLoader VelocityShader;
	VelocityMaxShaderLoader VelocityMaxShader;

	// textures
	GLuint AntiAliasedTexture;
	GLuint TemporalUpscaled4kTexture;
	GLuint TemporalUpscaled4kTempTexture;
	GLuint VelocityMaxTexture;
	GLuint CameraVelocityTexture;

};

