#pragma once

#include"DefferedRenderer.h"
#include"FrameBuffer.h"
//3dObjects
#include"Terrain3D.h"
#include"TerrainLOD2.h"
#include"Ground3D.h"
#include"SkyDome3D.h"
#include"Earth3D.h"
#include"ModelPBR.h"
#include"Water3D.h"
#include"ModelPBR_Shading.h"
#include"ModelPBR_SpeedTree.h"
#include"ModelPBR_SpeedTreeInstanced.h"
#include"ModelPBR_RockAsset.h"
#include"ModelTextured.h"
#include"Triangle3D.h"
#include"TerrainBasic3D.h"
#include"CubeMap3D.h"
#include"ParticleSystem.h"
#include"SmokeParticleSystem.h"
#include"RainParticleSystem.h"

//shaders
#include"passThroughShaderLoader.h"
#include"StarFieldShaderLoader.h"
#include "Axis3D_ShaderLoader.h"
#include"DeformedSphereShaderLoader.h"
#include"TerrainShaderLoader.h"
#include"TerrainLOD2ShaderLoader.h"
#include"TerrainBasicShaderLoader.h"
#include"EarthLODIndiaShaderLoader.h"
#include"EarthLODEverestShaderLoader.h"
#include"GroundShaderLoader.h"
#include"SingleBillboardShaderLoader.h"
#include"SunGlareShaderLoader.h"
	//water
#include"WaterShaderLoader.h"
#include"WaterHeightMapShaderLoader.h"

#include"SkyDomeShaderLoader.h"
#include"FinalQuadShaderLoader.h"
#include"FinalQuadBartWronskiDownsampleShaderLoader.h"
#include"TwoTextureQuadShaderLoader.h"

//PBR
#include"PBR_ShaderLoader.h"
#include"PBR_AnimatedShaderLoader.h"
#include"PBR_AnimatedShadowShaderLoader.h"
#include"PBR_SpeedTreeShaderLoader.h"
#include"PBR_SpeedTreeShadowShaderLoader.h"
#include"DefferedPBR_SpeedTreeInstancedShaderLoader.h"
#include"SkyPBR_ShaderLoader.h"
#include"BasicSkyPBR_ShaderLoader.h"
#include"SkyFroxel_ShaderLoader.h"
#include"SkyDistant_ShaderLoader.h"

#include"CompositorShaderLoader.h"
#include"CompositorSpaceShaderLoader.h"
#include"SceneBlendPostProcessShaderLoader.h"
#include"CullingIndirectShaderLoader.h"
#include"CubeMapShaderLoader.h"
#include"RainDropShaderLoader.h"
#include"BlackBlendQuadShaderLoader.h"
#include"LightningQuadShaderLoader.h"
#include"UnderWaterQuadShaderLoader.h"
#include"ParticleSystemShaderLoader.h"
#include"SmokeParticleSystemShaderLoader.h"
#include"RainParticleSystemShaderLoader.h"

//special
#include"Camera.h"
#include"Noise.h"
#include "Clouds.h"
#include "TAA.h"
#include"RadialBlur.h"
#include"CullIndirect.h"
#include"RayCaster.h"
#include"Atmosphere/demo.h"

//animation - assimp
#include "AnimModel.h"

//video
#include "opencv2/opencv.hpp"

class ZzRenderer
{
	//AnimModel rat_run;
	//AnimModel rat_stopRun;
	//AnimModel rat_idle;
	//AnimModel rat_walk;
	//AnimModel rat_eat1;
	//AnimModel rat_sleep;
	//AnimModel rat_rage;

public:
	ZzRenderer();
	~ZzRenderer();

	static Camera Cam;

	void Update(HWND hwnd);
	void UpdateSunPosition(void);
	void UpdateScreenSunPosition(void);
	//Render Geometry
	void Render_Triangle(vmath::vec4 worldPos);
	void Render_Axis3D(void);
	//void Render_Terrain(vmath::mat4 lightSpaceMatrix);
	//void Render_TerrainLOD2(void);
	void Render_EarthLODIndia(vmath::vec3 WaterTint, GLuint isCullIndiaLOD);
	void Render_EarthLODEverest(void);
	void Render_TerrainBasic3D(GLboolean Shadow);
	void Render_DeformedSphere(void);
	//void Render_Ground(GLuint textureSet, vmath::vec3 tint, GLfloat time, vmath::mat4 lightSpaceMatrix);
	//void Render_Water(GLfloat smoothEdgeFactor);
	//void Render_SkyDome(void);
	void Render_GUI(void);
	void Render_ModelsPBR(void);
	void Render_SkyFroxel(void);
	void Render_SkyDistant(void);
	void Render_DefferedSpeedTreeInstanced(GLboolean shadow);
	void Render_GaneshaPBR(void);
	void Render_PBR_RockAsset(void);
	void Render_FinalClouds(GLfloat VolumetricCoverage, GLfloat HighAltitudeCoverage, GLfloat timeMultiplier, GLuint WeatherMap);
	void Render_FinalCloudsGround(GLfloat VolumetricCoverage, GLfloat HighAltitudeCoverage, GLfloat timeMultiplier, GLuint WeatherMap);
	void Render_FinalCloudsSpace(GLfloat VolumetricCoverage, GLfloat HighAltitudeCoverage, GLfloat timeMultiplier, GLuint WeatherMap);
	//void Render_GodRays(void);
	void Render_SingleBillboard(GLuint TextureID, vmath::vec3 position ,vmath::vec3 size, GLfloat Intensity);
	void Render_SunGlare(GLuint TextureID, vmath::vec3 position, GLfloat size,vmath::vec3 fFactorPitchYaw);
	void Render_SkyPBR(GLfloat Exposure, vmath::vec3 Tint, vmath::vec3 WaterTint, GLfloat AnimFactor, vmath::vec2 Cloud2D_Scale_Visibility);
	void Render_BasicSkyPBR(void);
	void Render_Compositor(GLuint TextureID_Deffered,GLuint TextureID_Background ,GLuint TextureID_Flare);
	void Render_CompositorSpace(GLuint TextureID_Deffered,GLuint TextureID_Background ,GLuint TextureID_Flare);
	void Render_CompositorSpaceOffline(GLuint TextureID_Deffered, GLuint TextureID_Background, GLuint TextureID_Flare, GLboolean isFbo4k,GLfloat GroundSpaceFactorMultiplier);
	//void Render_Mouse_RunStopIdle(vmath::vec3 worldPos, GLfloat rotateY, GLfloat scale, GLfloat duration);
	//void Render_Mouse_RunStopSleep(vmath::vec3 worldPos, GLfloat rotateY, GLfloat scale, GLfloat duration);
	//void Render_Mouse_Sleep(vmath::vec3 worldPos, GLfloat rotateY, GLfloat scale, GLfloat duration);
	//void Render_Mouse_WakeUp(vmath::vec3 worldPos, GLfloat rotateY, GLfloat scale, GLfloat duration);
	//void Render_Mouse_Run(vmath::vec3 worldPos, GLfloat rotateY, GLfloat scale, GLfloat duration);
	//void Render_Mouse_WalkIdle(vmath::vec3 worldPos, GLfloat rotateY, GLfloat scale, GLfloat duration);
	//void Render_Mouse_IdleWalk(vmath::vec3 worldPos, GLfloat rotateY, GLfloat scale, GLfloat duration);
	//void Render_Mouse_Eat(vmath::vec3 worldPos, GLfloat rotateY, GLfloat scale, GLfloat duration);
	//void Render_Mouse_Rage(vmath::vec3 worldPos, GLfloat rotateY, GLfloat scale, GLfloat duration,bool startAnimation);

	void Render_All(HWND hwnd);
	//void Render_TestScene(HWND hwnd);
	void Render_DefferedSkyPBR(void);
	void Render_PaleBlueDotGroundScene();

	//1
	void Render_PaleBlueDotBeginScene();
	void Render_PaleBlueDotBeginSceneOffline(HWND hwnd,GLboolean isFbo4k);
	void Render_PaleBlueDotSceneFirstOffline(HWND hwnd, GLboolean isFbo4k);
	void Render_PaleBlueDotSceneSecondOffline(HWND hwnd, GLboolean isFbo4k);
	void Render_PaleBlueDotSceneTeacherOffline(HWND hwnd, GLboolean isFbo4k);
	void Render_PaleBlueDotSceneBloodRiverOffline(HWND hwnd, GLboolean isFbo4k);
	void Render_PaleBlueDotSceneFarEarthOffline(HWND hwnd, GLboolean isFbo4k);
	void Render_PaleBlueDotSceneFractionOffline(HWND hwnd, GLboolean isFbo4k);
	void Render_PaleBlueDotSceneDelusionOffline(HWND hwnd, GLboolean isFbo4k);
	void Render_PaleBlueDotSceneLonelySpeckOffline(HWND hwnd, GLboolean isFbo4k);
	void Render_PaleBlueDotSceneLifeOffline(HWND hwnd);
	void Render_PaleBlueDotSceneAfterLifeOffline(HWND hwnd);
	void Render_PaleBlueDotSceneFinalSceneOffline(HWND hwnd);

	void Render_PaleBlueDotScenePlaneCloudsOffline(HWND hwnd, GLboolean isFbo4k);
	void Render_PaleBlueDotCreditSceneOffline(HWND hwnd);

	//void Render_ParticleSystem(void);
	//void Render_SmokeParticleSystem(void);
	//void Render_RainParticleSystem(void);

	//OpenCV
	//bool Render_Video(const GLchar* VideoFileName, unsigned int animationSpeedDelay);

//	void Render_TitleVideo(void);
	void Render_FinalQuad(GLuint texture);
	void Render_FinalQuad(GLuint texture, uvec2 WidthHeight);
	void Render_FinalQuadBartWronskiDownsample(GLuint texture);
	void Render_FinalQuadBartWronskiDownsample(GLuint texture, uvec2 WidthHeight);
	//void Render_Credits(void);

	void Render_StarField(void);


	void Render_SceneFboToBlack(GLfloat factor)
	{
		BlackBlendShader.Start();

		BlackBlendShader.LoadSampler2DTexture_Uniform(GL_TEXTURE0, 0, fboScene->GetFbo_Color_Texture(0));
		BlackBlendShader.LoadBlendFactorUniform(factor);
		glDrawArrays(GL_QUADS, 0, 4);
		BlackBlendShader.Stop();
	}

	void Render_TwoTextureBlendQuad(GLuint TextureID_0, GLuint TextureID_1, GLfloat factor, GLuint BlueNoiseTextureID);
	
	void Render_PostProcessFboToBlack(GLfloat factor)
	{
		BlackBlendShader.Start();

		BlackBlendShader.LoadSampler2DTexture_Uniform(GL_TEXTURE0, 0, fboPostProcess->GetFbo_Color_Texture(0));
		BlackBlendShader.LoadBlendFactorUniform(factor);
		glDrawArrays(GL_QUADS, 0, 4);
		BlackBlendShader.Stop();
	}

	void Render_CreatePaleBlueDotVideo(void);

	void Render_SingleScene(cv::VideoCapture Cap_First, cv::VideoCapture Cap_Second, GLuint TotalFrames, GLuint CurrentFrame,GLuint BlueNoiseTextureID);
	void Render_SingleSceneCredit(cv::VideoCapture Cap_First, cv::VideoCapture Cap_Second, GLuint TotalFrames, GLuint CurrentFrame,GLuint BlueNoiseTextureID);

	//FINAL BLEND
	//void RenderSceneBlendPostProcessQuad(GLfloat fVignette, vmath::mat4 lightSpaceMatrix);

	//Framebuffer binding
	void BindDefaultFbo(void);
	void BindDefaultFbo(uvec2 WidthHeight);
	void BindSceneFbo(void);
	void BindFbo4k(void);
	void BindFbo4kExtra(void);
	void BindFbo4kFinal(void);
	void BindSceneOfflineFbo(void);
	void BindFboMSAA(void);
	void BindFboMSAA8(void);
	void BindFboMSAA16(void);
	void BindFboSkyDistant(void);
	void BindFboShadowMap(void);
	void BindFboTemporalAA(void);
	void BindFboTemporalUpscale4K(void);
	void BindFboPostProcess(void);
	void BindFboVelocity(void);


	//Resolve fbo
	void MSAA_toDefaultFbo(GLenum readBuffer)
	{
		ResolveFBO(fboMSAA->GetFbo(), 0, readBuffer);
	}
	void MSAA_toSceneFbo()
	{
		ResolveFBO(fboMSAA->GetFbo(), fboScene->GetFbo(), GL_COLOR_ATTACHMENT0);
		ResolveFBO(fboMSAA->GetFbo(), fboScene->GetFbo(), GL_COLOR_ATTACHMENT1);
	}
	//MSAA8
	void MSAA8_toDefaultFbo(GLenum readBuffer)
	{
		ResolveFBO(fboMSAA8->GetFbo(), 0, readBuffer);
	}
	void MSAA8_toSceneFbo()
	{
		ResolveFBO(fboMSAA8->GetFbo(), fboScene->GetFbo(), GL_COLOR_ATTACHMENT0);
		ResolveFBO(fboMSAA8->GetFbo(), fboScene->GetFbo(), GL_COLOR_ATTACHMENT1);
	}
	//MSAA16
	void MSAA16_toDefaultFbo(GLenum readBuffer)
	{
		ResolveFBO(fboMSAA16->GetFbo(), 0, readBuffer);
	}
	void MSAA16_toSceneFbo()
	{
		ResolveFBO(fboMSAA16->GetFbo(), fboScene->GetFbo(), GL_COLOR_ATTACHMENT0);
		ResolveFBO(fboMSAA16->GetFbo(), fboScene->GetFbo(), GL_COLOR_ATTACHMENT1);
	}
	void Scene_toDefaultFbo(GLenum readBuffer)
	{
		ResolveFBO(fboScene->GetFbo(), 0, readBuffer);
	}
	void Sky_toDefaultFbo(GLenum readBuffer)
	{
		ResolveFBO(fboSkyDistant->GetFbo(), 0, readBuffer);
	}
	void TemporalAA_toDefaultFbo(GLenum readBuffer)
	{
		ResolveFBO(fboTemporalAA->GetFbo(), 0, readBuffer);
	}

	void PostProcess_toDefaultFbo(GLenum readBuffer)
	{
		ResolveFBO(fboPostProcess->GetFbo(), 0, readBuffer);
	}

	//Framebuffers methods
	void ResizeFramebuffers(GLsizei Width, GLsizei Height);

	//backInTime
	void GetLastFrameData(void);

private:
	//Framebuffers methods
	void ResolveFBO(GLuint src, GLuint dst, GLenum readBuffer);
	//Shaders
	PassThroughShaderLoader passThroughShader;
	StarFieldShaderLoader StarFieldShader;
	Axis3D_ShaderLoader Axis3D_Shader;
	TerrainShaderLoader terrainShader;
	TerrainLOD2ShaderLoader terrainLOD2Shader;
	TerrainBasicShaderLoader TerrainBasicShader;
	EarthLODIndiaShaderLoader EarthLODIndiaShader;
	EarthLODEverestShaderLoader EarthLODEverestShader;
	DeformedSphereShaderLoader DeformedSphereShader;
	GroundShaderLoader groundShader;
	SkyDomeShaderLoader skyDomeShader;
	FinalQuadShaderLoader finalQuadShader;
	FinalQuadBartWronskiDownsampleShaderLoader FinalQuadBartWronskiDownsampleShader;
	TwoTextureQuadShaderLoader twoTextureQuadShader;
	SceneBlendPostProcessShaderLoader SceneBlendPostProcessShader;
	CompositorShaderLoader CompositorShader;
	CompositorSpaceShaderLoader CompositorSpaceShader;
	WaterShaderLoader waterShaderLoader;
	SingleBillboardShaderLoader SingleBillboardShader;
	SunGlareShaderLoader SunGlareShader;

	PBR_ShaderLoader PBR_Shader;
	PBR_AnimatedShaderLoader PBR_AnimatedShader;
	PBR_AnimatedShadowShaderLoader PBR_AnimatedShadowShader;
	PBR_SpeedTreeShaderLoader PBR_SpeedTreeShader;
	PBR_SpeedTreeShadowShaderLoader PBR_SpeedTreeShadowShader;
	DefferedPBR_SpeedTreeInstancedShaderLoader DefferedPBR_SpeedTreeInstancedShader;
	SkyPBR_ShaderLoader *SkyPBR_Shader;//need pointer because this shaderloader must be created after atmosphere shader creation
	BasicSkyPBR_ShaderLoader *BasicSkyPBR_Shader;//need pointer because this shaderloader must be created after atmosphere shader creation
	SkyFroxel_ShaderLoader * SkyFroxel_Shader;//need pointer because this shaderloader must be created after atmosphere shader creation
	SkyDistant_ShaderLoader* SkyDistant_Shader;//need pointer because this shaderloader must be created after atmosphere shader creation

	CullingIndirectShaderLoader CullIndirectShader;
	CubeMapShaderLoader CubeMapShader;
	RainDropShaderLoader RainDropShader;
	BlackBlendQuadShaderLoader BlackBlendShader;
	LightningQuadShaderLoader LightningShader;
	UnderWaterQuadShaderLoader UnderWaterQuadShader;
	ParticleSystemShaderLoader ParticleSystemShader;
	SmokeParticleSystemShaderLoader SmokeParticleSystemShader;
	RainParticleSystemShaderLoader RainParticleSystemShader;

	//3D Objects
	//Terrain3D terrain;
	//TerrainLOD2 terrainLOD2;
	TerrainBasic3D TerrainTreeFog;
	Earth3D earth;
	//Ground3D ground;
	//SkyDome3D skyDome;
	//Water3D water;
	//ModelPBR_Shading rat_shading;
	Triangle3D triangle;
	//CubeMap3D cubeMap;
	//ParticleSystem particleSystem;
	//SmokeParticleSystem smokeParticleSystem;
	//RainParticleSystem rainParticleSystem;

	//ganesha
	ModelPBR *ganesha;
	const char* ganeshaFilePath = { "Resources/Models/ganesha/ganesha.obj" };

	//RockAsset
	ModelPBR_RockAsset* MossyRocks;

	//palm tree
	ModelPBR_SpeedTree*palm;
	const char* palmFilePath = { "Resources/Models/trees/palm.obj" };
	ModelPBR_SpeedTreeInstanced*palmInstanced;

	const char* BroadLeafFilePath = { "Resources/Models/trees/BroadLeaf.obj" };
	ModelPBR_SpeedTreeInstanced* BroadLeafInstanced;

	//Cave
	ModelTextured* Cave;
	const char* CaveFilePath = { "Resources/Models/Cave/Cave.obj" };

	//RainDrop
	ModelTextured* RainDrop;
	const char* RainDropFilePath = { "Resources/Models/RainDrop/RainDrop.obj" };



	//Special
	static GLfloat sunAngle;
	static vmath::vec3 SunPos;
	static vmath::vec3 ToSunVector;
	static vmath::vec3 SunWorldPos;
	static vmath::vec2 ScreenSunPos;

	//video
	cv::VideoCapture VideoCapEveryoneLove;
	cv::VideoCapture VideoCapAggregate;
	cv::VideoCapture VideoCapCoupleInLove;
	cv::VideoCapture VideoCapTeacherOfMoral;
	cv::VideoCapture VideoCapBloodRiver;
	cv::VideoCapture VideoCapFractionOfDot;
	cv::VideoCapture VideoCapDelusion;
	cv::VideoCapture VideoCapLonelySpeck;
	cv::VideoCapture VideoCapLife;
	cv::VideoCapture VideoCapAfterLife;
	cv::VideoCapture VideoCapFinalScene;
	cv::VideoCapture VideoCapCredit;
	cv::VideoCapture VideoCapEmpty;

	GLuint VideoFrameTextureID_First;
	GLuint VideoFrameTextureID_Second;

	cv::Mat *img_First;
	cv::Mat *Frame_First;

	cv::Mat *img_Second;
	cv::Mat *Frame_Second;
	cv::VideoWriter VideoWriter;

	//Advanced
	Clouds *clouds;
	Noise noise;
	TAA temporalAA;
	atmosphere::demo::Demo * AtmoScatter;

	//Post Process
	//RadialBlur radialBlur;

	//CullIndirect cullIndirect;
	RayCaster myRayCaster;

	//DefferedRenderer
	DefferedRenderer DefferedRenderer;
	//framebuffers
	FrameBuffer* fboMSAA;
	FrameBuffer* fboMSAA8;
	FrameBuffer* fboMSAA16;
	FrameBuffer* fboScene;
	FrameBuffer* fboSceneOffline;
	FrameBuffer* fboSkyDistant;
	FrameBuffer* fboFroxel;
	FrameBuffer* fboTemporalAA;
	FrameBuffer* fbo4k;
	FrameBuffer* fbo4kExtra;
	FrameBuffer* fbo4kFinal;
	FrameBuffer* fboTemporalUpscale4k;
	FrameBuffer* fboShadowMap;
	FrameBuffer* fboVelocity;
	FrameBuffer* fboPostProcess;

	//FILE* scene;

	CandidateDraw* pDraws;
	const unsigned int CANDIDATE_COUNT = 1024;
};

