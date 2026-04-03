#pragma once
#include"MyTGA_Loader.h"
#include"PerlinWorley3DShaderLoader.h"
#include"Worley32ShaderLoader.h"
#include"WeatherMapShaderLoader.h"

class Noise
{
public:
	Noise() ;
	~Noise();

	GLuint GetTextureID_BlueNoise(void)
	{
		return TextureID_BlueNoise;
	}

	GLuint GetTextureID_BlueGlow(void)
	{
		return TextureID_BlueGlow;
	}

	GLuint GetTextureID_WhirlNoise(void)
	{
		return TextureID_WhirlNoise;
	}
	GLuint GetTextureID_WhirlTexture(void)
	{
		return TextureID_WhirlTexture;
	}
	GLuint GetTextureID_SunGlare(void)
	{
		return TextureID_SunGlare;
	}

	GLuint GetTextureID_Lightning(void)
	{
		return TextureID_Lightning;
	}
	GLuint GetTextureID_Lightning2(void)
	{
		return TextureID_Lightning2;
	}

	GLuint GetTextureID_PerlinWorley3D(void)
	{
		return TextureID_PerlinWorley3D;
	}

	GLuint GetTexureID_FroxelInScatter(void)
	{
		return TexureID_FroxelInScatter;
	}

	GLuint GetTexureID_FroxelTransmittance(void)
	{
		return TexureID_FroxelTransmittance;
	}

	GLuint GetTextureID_WeatherMap(void)
	{
		return TextureID_WeatherMap;
	}
	GLuint GetTextureID_Worley32(void)
	{
		return TextureID_Worley32;
	}
	
	GLuint GetTextureID_Permutation(void)
	{
		return TextureID_Permutation;
	}

	GLuint* GetBayerMatrix4(void)
	{
		return BayerMatrix4;
	}
	GLuint* GetGreje656Matrix4(void)
	{
		return Greje656Matrix4;
	}
	
	vmath::vec2* GetHalton23(void)
	{
		return Halton23;
	}

	void SetCurrentHalton23(vmath::vec2 halton23)
	{
		currentHalton23 = halton23;
	}

	vmath::vec2 GetCurrentHalton23(void)
	{
		return currentHalton23;
	}

	void SetPreviousHalton23(vmath::vec2 prevHalton23)
	{
		previousHalton23 = prevHalton23;
	}

	vmath::vec2 GetPreviousHalton23(void)
	{
		return previousHalton23;
	}

	float halton(int base, int index)
	{
		float result = 0.;
		float f = 1.;
		int i = index;
		while (i > 0)
		{
			f /= base;
			result += f * (i % base);
			//i = index / base;
			i = int(floor( i/ float(base)));
		}
		return result;
	}

	void InitializeHalton23(void)
	{

		 for (int i = 0; i < 16; i++)
		 {
			 vmath::vec2 seq = vmath::vec2(halton(2, i+1)-0.5f, halton(3, i+1)-0.5f );
			 Halton23[i] = seq;
		 }

	}

	void CleanUp(void);

private:
	//functions
	void ComputePerlinWorley3D(void);
	void ComputeWorley32(void);
	void CreatePermutationTexture(void);
	void CreateWeatherMapTexture(void);
	void CreateFroxelTextures(void);

	//3D Noise compute
	GLuint TextureID_PerlinWorley3D;
	GLuint TextureID_Worley32;

	//3D Froxel
	GLuint TexureID_FroxelTransmittance;
	GLuint TexureID_FroxelInScatter;

	//2D weatherTexture
	GLuint TextureID_WeatherMap;
	//1D permutation random noise
	GLuint TextureID_Permutation;//256*2

	//blue noise
	const GLchar* BlueNoiseFileName = {"Resources/Textures/BlueNoiseRGB.tga"};
	GLuint TextureID_BlueNoise;

	//whirl noise
	const GLchar* WhirlNoiseFileName = { "Resources/Textures/WhirlNoise.tga" };
	GLuint TextureID_WhirlNoise;

	//whirl textures
	const GLchar* WhirlTextureFileName = { "Resources/Textures/WhirlTextureSet.tga" };
	GLuint TextureID_WhirlTexture;

	//lightning
	const GLchar* LightningFileName = { "Resources/Textures/lightningHalf.tga" };
	const GLchar* LightningFileName2 = { "Resources/Textures/lightning2.tga" };
	GLuint TextureID_Lightning;
	GLuint TextureID_Lightning2;

	//glow
	GLuint TextureID_BlueGlow;

	//SunFlare
	const GLchar* SunGlareFileName = { "Resources/Textures/SunGlare3.tga" };
	GLuint TextureID_SunGlare;
	//BayerMatrix
	GLuint BayerMatrix4[16] = { 0,8,2,10,12,4,14,6,3,11,1,9,15,7,13,5 };
	GLuint Greje656Matrix4[16] = { 6,9,2,4,14,11,7,12,1,5,15,0,10,13,3,8 };

	//halton sequence
	vmath::vec2 Halton23[16];

	vmath::vec2 currentHalton23;
	vmath::vec2 previousHalton23;
};

