#include "stdafx.h"
#include "Noise.h"

Noise::Noise(void)
{
	const GLchar* TextureID_BlueGlowFileName = { "Resources/Textures/Glow.tga" };
	TextureID_BlueGlow = MyTGA_Loader::LoadTGA_texture(TextureID_BlueGlowFileName, false, GL_LINEAR, false);

	TextureID_BlueNoise = MyTGA_Loader::LoadTGA_texture(BlueNoiseFileName, false, GL_NEAREST,true);
	TextureID_WhirlNoise = MyTGA_Loader::LoadTGA_texture(WhirlNoiseFileName, false, GL_LINEAR,false);
	TextureID_WhirlTexture = MyTGA_Loader::LoadTGA_texture(WhirlTextureFileName, false, GL_LINEAR,false);
	//TextureID_Lightning = MyTGA_Loader::LoadTGA_texture(LightningFileName, false, GL_LINEAR,false);
	//TextureID_Lightning2 = MyTGA_Loader::LoadTGA_texture(LightningFileName2, false, GL_LINEAR,false);
	TextureID_SunGlare = MyTGA_Loader::LoadTGA_texture(SunGlareFileName, false, GL_LINEAR,false);

	CreateFroxelTextures();
	CreatePermutationTexture();//must be before perlinWorley
	//CreateWeatherMapTexture();
	ComputePerlinWorley3D();
	ComputeWorley32();

	InitializeHalton23();

	currentHalton23 = vmath::vec2(0.0f, 0.0f);
	previousHalton23 = vmath::vec2(0.0f, 0.0f);
}

Noise::~Noise(void)
{
	CleanUp();
}

void Noise::CreatePermutationTexture(void)
{
	const GLubyte permutation[256] = { 151,160,137,91,90,15,
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
	190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
	129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
	49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
	};

	GLubyte perm[512];

	for (int i = 0; i < 256; i++)
		perm[256 + i] = perm[i] = permutation[i];

	glGenTextures(1, &TextureID_Permutation);
	glBindTexture(GL_TEXTURE_1D, TextureID_Permutation);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RED, 512, 0, GL_RED, GL_UNSIGNED_BYTE,(const void*) perm);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_1D, 0);
}

void Noise::ComputePerlinWorley3D(void)
{

	//Perlin-Worley

	PerlinWorley3DShaderLoader PerlinWorley3DShader;

	const GLsizei PerlinWorleyTextureSize = 128;
	const GLsizei WorkGroupSize = 4;

	const GLuint FinalComputeSize = INT_CEIL(PerlinWorleyTextureSize, WorkGroupSize);

	glGenTextures(1, &TextureID_PerlinWorley3D);
	glBindTexture(GL_TEXTURE_3D, TextureID_PerlinWorley3D);

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, PerlinWorleyTextureSize, PerlinWorleyTextureSize, PerlinWorleyTextureSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//Allocate memory for mipmap levels
	glGenerateTextureMipmap(TextureID_PerlinWorley3D);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_3D, 0);

	//Run Compute

	PerlinWorley3DShader.Start();

	PerlinWorley3DShader.LoadSampler1D_permutationTextureUniform(GL_TEXTURE0, 0, TextureID_Permutation);
	PerlinWorley3DShader.LoadiTextureSize_Uniform(PerlinWorleyTextureSize);

	glBindImageTexture(0, TextureID_PerlinWorley3D, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glDispatchCompute(FinalComputeSize, FinalComputeSize, FinalComputeSize);

	//actual mipmap levels are created using zeroth level of texture
	glBindTexture(GL_TEXTURE_3D, TextureID_PerlinWorley3D);
	glGenerateTextureMipmap(TextureID_PerlinWorley3D);
	glBindTexture(GL_TEXTURE_3D, 0);

	PerlinWorley3DShader.Stop();
}

void Noise::ComputeWorley32(void)
{
	//Worley32
	Worley32ShaderLoader Worley32Shader;

	const GLsizei Worley32TextureSize = 32;
	const GLsizei WorkGroupSize = 4;

	const GLuint FinalComputeSize = INT_CEIL(Worley32TextureSize, WorkGroupSize);

	glGenTextures(1, &TextureID_Worley32);
	glBindTexture(GL_TEXTURE_3D, TextureID_Worley32);

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, Worley32TextureSize, Worley32TextureSize, Worley32TextureSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//allocate memory for mipmap levels because data points to NULL
	glGenerateTextureMipmap(TextureID_Worley32);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_3D, 0);

	//Run Compute

	Worley32Shader.Start();

	Worley32Shader.LoadSampler1D_permutationTextureUniform(GL_TEXTURE0, 0, TextureID_Permutation);
	Worley32Shader.LoadiTextureSize_Uniform(Worley32TextureSize);

	glBindImageTexture(0, TextureID_Worley32, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glDispatchCompute(FinalComputeSize, FinalComputeSize, FinalComputeSize);

	//actual mipmap levels are created using zeroth level of texture after getting data in texture
	glBindTexture(GL_TEXTURE_3D, TextureID_Worley32);
	glGenerateTextureMipmap(TextureID_Worley32);
	glBindTexture(GL_TEXTURE_3D, 0);

	Worley32Shader.Stop();
}

void Noise::CreateFroxelTextures(void)
{
	const GLsizei FroxelTextureSize = 64;

	//Transmittance
	glGenTextures(1, &TexureID_FroxelTransmittance);
	glBindTexture(GL_TEXTURE_3D, TexureID_FroxelTransmittance);

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, FroxelTextureSize, FroxelTextureSize, FroxelTextureSize, 0, GL_RGBA, GL_FLOAT, NULL);

	//Allocate memory for mipmap levels
	//glGenerateTextureMipmap(TexureID_FroxelTransmittance);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_3D, 0);

	//Inscatter
	glGenTextures(1, &TexureID_FroxelInScatter);
	glBindTexture(GL_TEXTURE_3D, TexureID_FroxelInScatter);

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, FroxelTextureSize, FroxelTextureSize, FroxelTextureSize, 0, GL_RGBA, GL_FLOAT, NULL);

	//Allocate memory for mipmap levels
	//glGenerateTextureMipmap(TexureID_FroxelInScatter);


	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_3D, 0);
}

void Noise::CreateWeatherMapTexture(void)
{
	const GLsizei WeatherTextureSize = 1024;
	const GLsizei WorkGroupSize = 32;

	const GLuint FinalComputeSize = INT_CEIL(WeatherTextureSize, WorkGroupSize);

	WeatherMapShaderLoader weatherMapShader;
	//weatherMap
	glGenTextures(1, &TextureID_WeatherMap);
	glBindTexture(GL_TEXTURE_2D, TextureID_WeatherMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WeatherTextureSize, WeatherTextureSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	weatherMapShader.Start();

	weatherMapShader.LoadSampler1D_permutationTextureUniform(GL_TEXTURE0, 0, TextureID_Permutation);
	weatherMapShader.LoadiTextureSize_Uniform(WeatherTextureSize);

	glBindImageTexture(0, TextureID_WeatherMap, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glDispatchCompute(FinalComputeSize, FinalComputeSize, 1);

	weatherMapShader.Stop();
}

void Noise::CleanUp(void)
{
	//blue noise
	if (TextureID_BlueNoise)
	{
		glDeleteTextures(1, &TextureID_BlueNoise);
		TextureID_BlueNoise = 0;
	}	
	//whirl noise
	if (TextureID_WhirlNoise)
	{
		glDeleteTextures(1, &TextureID_WhirlNoise);
		TextureID_WhirlNoise = 0;
	}
	//whirl noise
	if (TextureID_WhirlTexture)
	{
		glDeleteTextures(1, &TextureID_WhirlTexture);
		TextureID_WhirlTexture = 0;
	}
	//Sun Glare
	if (TextureID_SunGlare)
	{
		glDeleteTextures(1, &TextureID_SunGlare);
		TextureID_SunGlare = 0;
	}

	//Perlin-Worley
	if (TextureID_PerlinWorley3D)
	{
		glDeleteTextures(1, &TextureID_PerlinWorley3D);
		TextureID_PerlinWorley3D = 0;
	}

	//Worley32
	if (TextureID_Worley32)
	{
		glDeleteTextures(1, &TextureID_Worley32);
		TextureID_Worley32 = 0;
	}

	//Froxel
	if (TexureID_FroxelTransmittance)
	{
		glDeleteTextures(1, &TexureID_FroxelTransmittance);
		TexureID_FroxelTransmittance = 0;
	}

	if (TexureID_FroxelInScatter)
	{
		glDeleteTextures(1, &TexureID_FroxelInScatter);
		TexureID_FroxelInScatter = 0;
	}

	//permutation for perlinNoise
	if(TextureID_Permutation)
	{
		glDeleteTextures(1, &TextureID_Permutation);
		TextureID_Permutation = 0;
	}

	//weather
	if(TextureID_WeatherMap)
	{
		glDeleteTextures(1, &TextureID_WeatherMap);
		TextureID_WeatherMap = 0;
	}

	//lightning
	if (TextureID_Lightning)
	{
		glDeleteTextures(1, &TextureID_Lightning);
		TextureID_Lightning = 0;
	}
	if (TextureID_Lightning2)
	{
		glDeleteTextures(1, &TextureID_Lightning2);
		TextureID_Lightning2 = 0;
	}
}
