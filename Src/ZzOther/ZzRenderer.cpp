#include "stdafx.h"
#include "ZzRenderer.h"
#include "MyWin32.h"
#include "AnimModel.h"
#include"CullIndirect.h"

//cameraScene
//#include "khojScene1.h"
#include "khojScene1Final.h"
#include "lightningStrikeData.h"
#include "FrameBuffer.h"

vmath::vec3 ZzRenderer::SunPos = vmath::vec3(0.0, 0.0, -1000.0);
vmath::vec3 ZzRenderer::ToSunVector = vmath::vec3(0.0, 0.0, 0.0);
vmath::vec3 ZzRenderer::SunWorldPos = vmath::vec3(0.0, 0.0, -1000.0);
vmath::vec2 ZzRenderer::ScreenSunPos = vmath::vec2(0.5, 0.5);
GLfloat ZzRenderer::sunAngle = 80.0f;
Camera ZzRenderer::Cam;

//Animation

#define RECORD_DEMO


ZzRenderer::ZzRenderer()
{	
	//PBR models
//	ganesha = new ModelPBR(ganeshaFilePath); 

	//MossyRock
	const char* MossyRocksFilePath = { "Resources/Models/RockAsset/1/RockAsset.obj" };
	MossyRocks = new ModelPBR_RockAsset(MossyRocksFilePath);
	//palm = new ModelPBR_SpeedTree(palmFilePath);
	//palmInstanced = new ModelPBR_SpeedTreeInstanced(palmFilePath);
	BroadLeafInstanced = new ModelPBR_SpeedTreeInstanced(BroadLeafFilePath);
	//Cave = new ModelTextured(CaveFilePath);
	//RainDrop = new ModelTextured(RainDropFilePath);

	//framebuffers............Remember to resize textures
//	fboMSAA = new FrameBuffer(GL_FALSE,2,GL_TRUE, GL_FALSE,GL_FALSE,GL_RGBA,GL_UNSIGNED_BYTE,GL_TRUE,4);
//	fboMSAA8 = new FrameBuffer(GL_FALSE,2,GL_TRUE, GL_FALSE,GL_FALSE,GL_RGBA,GL_UNSIGNED_BYTE,GL_TRUE,8);
//	fboMSAA16 = new FrameBuffer(GL_FALSE,2,GL_TRUE, GL_FALSE,GL_FALSE,GL_RGBA,GL_UNSIGNED_BYTE,GL_TRUE,16);
//	fboScene = new FrameBuffer(GL_TRUE,2,GL_TRUE, GL_FALSE,GL_TRUE,GL_RGBA,GL_UNSIGNED_BYTE,GL_FALSE,1, uvec2(1920, 1080));
//	fboSceneOffline = new FrameBuffer(GL_TRUE,2,GL_TRUE, GL_FALSE,GL_TRUE,GL_RGBA,GL_UNSIGNED_BYTE,GL_FALSE,1, uvec2(3840, 2160) * 2);//uvec2(3840, 2160)*2
//	fboSkyDistant = new FrameBuffer(GL_TRUE,1,GL_FALSE, GL_FALSE,GL_FALSE,GL_RGBA,GL_UNSIGNED_BYTE,GL_FALSE,1, uvec2(3840, 2160));
//	fboPostProcess = new FrameBuffer(GL_TRUE,1, GL_FALSE, GL_FALSE,GL_FALSE,GL_RGBA,GL_UNSIGNED_BYTE,GL_FALSE,1, uvec2(3840, 2160));//uvec2(3840, 2160)
//	fboTemporalAA = new FrameBuffer(GL_TRUE,1, GL_FALSE, GL_FALSE,GL_FALSE,GL_RGBA,GL_UNSIGNED_BYTE,GL_FALSE,1, uvec2(1920, 1080));
	fbo4k = new FrameBuffer(GL_TRUE,1, GL_FALSE, GL_FALSE,GL_FALSE,GL_RGBA,GL_UNSIGNED_BYTE,GL_FALSE,1, uvec2(3840, 2160));//uvec2(3840, 2160)
//	fbo4kExtra = new FrameBuffer(GL_FALSE,1, GL_FALSE, GL_FALSE,GL_FALSE,GL_RGBA,GL_UNSIGNED_BYTE,GL_TRUE,32, uvec2(3840, 2160));
//	fbo4kFinal = new FrameBuffer(GL_TRUE,1, GL_FALSE, GL_FALSE,GL_FALSE,GL_RGBA,GL_UNSIGNED_BYTE,GL_FALSE,1, uvec2(3840, 2160));

#ifdef TEMPORAL_UPSCALE_8K
//	fboTemporalUpscale4k = new FrameBuffer(GL_TRUE, 1, GL_FALSE, GL_FALSE, GL_FALSE, GL_RGBA, GL_UNSIGNED_BYTE, GL_FALSE, 1, uvec2(3840, 2160) * 2);
#else
	fboTemporalUpscale4k = new FrameBuffer(GL_TRUE, 1, GL_FALSE, GL_FALSE, GL_FALSE, GL_RGBA, GL_UNSIGNED_BYTE, GL_FALSE, 1, uvec2(3840, 2160) );
#endif // TEMPORAL_UPSCALE_8K


	//fboVelocity = new FrameBuffer(GL_TRUE,1, GL_FALSE, GL_FALSE,GL_FALSE,GL_RG16F,GL_FLOAT,GL_FALSE,1, uvec2(1920, 1080));
//	fboShadowMap = new FrameBuffer(GL_FALSE,0,GL_TRUE,GL_TRUE,GL_TRUE,GL_R16F,GL_FLOAT,GL_FALSE,1, uvec2(0,0));//OnlyDepth, so some of other params arent used.

	//rat_run.LoadModel("Resources/Models/RAT/animations/Anim_Rat_Run.FBX");
	//rat_stopRun.LoadModel("Resources/Models/RAT/animations/Anim_Rat_StopRunning.FBX");
	//rat_idle.LoadModel("Resources/Models/RAT/animations/Anim_Rat_Idle.FBX");
	//rat_walk.LoadModel("Resources/Models/RAT/animations/Anim_Rat_Walk.FBX");
	//rat_eat1.LoadModel("Resources/Models/RAT/animations/Anim_Rat_Eat_1.FBX");
	//rat_sleep.LoadModel("Resources/Models/RAT/animations/Anim_Rat_Sleep.FBX");
	//rat_rage.LoadModel("Resources/Models/RAT/animations/Anim_Rat_Rage_2.FBX");

	pDraws = nullptr;

	//Atmospheric Scattering
	AtmoScatter = new atmosphere::demo::Demo(1920, 1080);
	SkyPBR_Shader = new SkyPBR_ShaderLoader(AtmoScatter->model().shader());
	BasicSkyPBR_Shader = new BasicSkyPBR_ShaderLoader(AtmoScatter->model().shader());
	SkyFroxel_Shader = new SkyFroxel_ShaderLoader(AtmoScatter->model().ComputeAtmoShader());
	SkyDistant_Shader = new SkyDistant_ShaderLoader(AtmoScatter->model().shader());
	clouds = new Clouds(AtmoScatter->model().ComputeAtmoShader(),AtmoScatter->model().atmoTextures);


	//OpenCV record
	glGenTextures(1, &VideoFrameTextureID_First);
	glBindTexture(GL_TEXTURE_2D, VideoFrameTextureID_First);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 3840, 2160, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &VideoFrameTextureID_Second);
	glBindTexture(GL_TEXTURE_2D, VideoFrameTextureID_Second);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 3840, 2160, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

//	fopen_s(&scene, "khojScene1.h", "w");
	
	//OpenCV Writer
	img_First = new cv::Mat( 2160, 3840, CV_8UC3);
	Frame_First = new cv::Mat(2160, 3840, CV_8UC3);

	img_Second = new cv::Mat( 2160, 3840, CV_8UC3);
	Frame_Second = new cv::Mat(2160, 3840, CV_8UC3);

	VideoWriter = cv::VideoWriter("paleBlue.mp4",-1, 60, cv::Size(3840, 2160));//output path// cv::VideoWriter::fourcc('X', 'V', 'I', 'D')

	VideoCapEveryoneLove = cv::VideoCapture("../../Scenes/EveryoneWeLove.mp4");
	VideoCapAggregate = cv::VideoCapture("../../Scenes/Aggregate.mp4");
	VideoCapCoupleInLove = cv::VideoCapture("../../Scenes/CoupleInLove.mp4");
	VideoCapTeacherOfMoral = cv::VideoCapture("../../Scenes/TeacherOfMoral.mp4");
	VideoCapBloodRiver = cv::VideoCapture("../../Scenes/BloodRiver.mp4");
	VideoCapFractionOfDot = cv::VideoCapture("../../Scenes/FractionOfDot.mp4");
	VideoCapDelusion = cv::VideoCapture("../../Scenes/Delusion.mp4");
	VideoCapLonelySpeck = cv::VideoCapture("../../Scenes/LonelySpeck.mp4");
	VideoCapLife = cv::VideoCapture("../../Scenes/Life.mp4");
	VideoCapAfterLife = cv::VideoCapture("../../Scenes/AfterLife.mp4");
	VideoCapFinalScene = cv::VideoCapture("../../Scenes/FinalScene.mp4");
	VideoCapCredit = cv::VideoCapture("../../Scenes/Credit.mp4");
};

ZzRenderer::~ZzRenderer()
{
	//delete PBR models
	if (ganesha)
	{
		delete ganesha;
		ganesha = nullptr;
	}
	if (palm)
	{
		delete palm;
		palm = nullptr;
	}

	//fbo classes
	if (fboMSAA)
	{
		delete fboMSAA;
		fboMSAA = nullptr;
	}
	if (fboMSAA8)
	{
		delete fboMSAA8;
		fboMSAA8 = nullptr;
	}

	if (fboMSAA16)
	{
		delete fboMSAA16;
		fboMSAA16 = nullptr;
	}

	if (fboScene)
	{
		delete fboScene;
		fboScene = nullptr;
	}
	if (fboSkyDistant)
	{
		delete fboSkyDistant;
		fboSkyDistant = nullptr;
	}

	if (fboFroxel)
	{
		delete fboFroxel;
		fboFroxel = nullptr;
	}
	if (fboPostProcess)
	{
		delete fboPostProcess;
		fboPostProcess = nullptr;
	}
	if (fboVelocity)
	{
		delete fboVelocity;
		fboVelocity = nullptr;
	}
	if (fboTemporalAA)
	{
		delete fboTemporalAA;
		fboTemporalAA = nullptr;
	}
	if (fboTemporalUpscale4k)
	{
		delete fboTemporalUpscale4k;
		fboTemporalUpscale4k = nullptr;
	}
	//

	if (fboShadowMap)
	{
		delete fboShadowMap;
		fboShadowMap = nullptr;
	}

	if (SkyPBR_Shader)
	{
		delete SkyPBR_Shader;
		SkyPBR_Shader = nullptr;
	}


	//OpenCV
	if (VideoFrameTextureID_First)
	{
		glDeleteTextures(1, &VideoFrameTextureID_First);
		VideoFrameTextureID_First = 0;
	}
	if (VideoFrameTextureID_Second)
	{
		glDeleteTextures(1, &VideoFrameTextureID_Second);
		VideoFrameTextureID_Second = 0;
	}

	//OpenCV
	if (VideoWriter.isOpened())
	{
		VideoWriter.release();
	}
	if (img_First)
	{
		img_First->release();
	}
	if (Frame_First)
	{
		Frame_First->release();
	}
	if (img_Second)
	{
		img_Second->release();
	}
	if (Frame_Second)
	{
		Frame_Second->release();
	}

	//OpenCV Video Captures
	if (VideoCapEveryoneLove.isOpened())
	{
		VideoCapEveryoneLove.release();
	}

	if (VideoCapAggregate.isOpened())
	{
		VideoCapAggregate.release();
	}

	if (VideoCapCoupleInLove.isOpened())
	{
		VideoCapCoupleInLove.release();
	}

	if (VideoCapTeacherOfMoral.isOpened())
	{
		VideoCapTeacherOfMoral.release();
	}
	if (VideoCapBloodRiver.isOpened())
	{
		VideoCapBloodRiver.release();
	}
	if (VideoCapFractionOfDot.isOpened())
	{
		VideoCapFractionOfDot.release();
	}
	if (VideoCapDelusion.isOpened())
	{
		VideoCapDelusion.release();
	}
	if (VideoCapLonelySpeck.isOpened())
	{
		VideoCapLonelySpeck.release();
	}
	if (VideoCapLife.isOpened())
	{
		VideoCapLife.release();
	}
	if (VideoCapAfterLife.isOpened())
	{
		VideoCapAfterLife.release();
	}
	if (VideoCapFinalScene.isOpened())
	{
		VideoCapFinalScene.release();
	}
	if (VideoCapCredit.isOpened())
	{
		VideoCapCredit.release();
	}

	//OpenCV
	//if (VideoCap.isOpened())
	//{
	//	VideoCap.release();
	//}


	//if (scene)
	//{
	//	fclose(scene);
	//	scene = NULL;
	//}
	//WRITE CAMERASCENE
	//FILE* finalScene = NULL;

	//fopen_s(&finalScene, "khojScene1Final.h", "w");

	//fprintf(finalScene,"#pragma once\n");
	//fprintf(finalScene,"#include\"vmath.h\"\n");

	//fprintf(finalScene,"CamInfo camInfoKhojScene1Final[] = {\n");

	//int size = sizeof(camInfoKhojScene1) / (sizeof(vmath::vec3) + sizeof(vmath::vec2));
	//static int localCounter = 0;
	//for (int i = 0; i < size; i++)
	//{
	//	float averagePitch = (camInfoKhojScene1[localCounter - 13].PitchYaw[0] + camInfoKhojScene1[localCounter - 12].PitchYaw[0] + camInfoKhojScene1[localCounter - 11].PitchYaw[0] + camInfoKhojScene1[localCounter - 10].PitchYaw[0] + camInfoKhojScene1[localCounter - 9].PitchYaw[0] + camInfoKhojScene1[localCounter - 8].PitchYaw[0] + camInfoKhojScene1[localCounter - 7].PitchYaw[0] + camInfoKhojScene1[localCounter - 6].PitchYaw[0] + camInfoKhojScene1[localCounter - 5].PitchYaw[0] + camInfoKhojScene1[localCounter - 4].PitchYaw[0] + camInfoKhojScene1[localCounter - 3].PitchYaw[0] + camInfoKhojScene1[localCounter - 2].PitchYaw[0] + camInfoKhojScene1[localCounter - 1].PitchYaw[0] + camInfoKhojScene1[localCounter].PitchYaw[0] + camInfoKhojScene1[localCounter + 1].PitchYaw[0] + camInfoKhojScene1[localCounter + 2].PitchYaw[0] + camInfoKhojScene1[localCounter + 3].PitchYaw[0] + camInfoKhojScene1[localCounter + 4].PitchYaw[0] + camInfoKhojScene1[localCounter + 5].PitchYaw[0] + camInfoKhojScene1[localCounter + 6].PitchYaw[0] + camInfoKhojScene1[localCounter + 7].PitchYaw[0] + camInfoKhojScene1[localCounter + 8].PitchYaw[0] + camInfoKhojScene1[localCounter + 9].PitchYaw[0] + camInfoKhojScene1[localCounter + 10].PitchYaw[0] + camInfoKhojScene1[localCounter + 11].PitchYaw[0] + camInfoKhojScene1[localCounter + 12].PitchYaw[0] + camInfoKhojScene1[localCounter + 13].PitchYaw[0]) / 27.0f;
	//	float averageYaw = (camInfoKhojScene1[localCounter - 13].PitchYaw[1] + camInfoKhojScene1[localCounter - 12].PitchYaw[1] + camInfoKhojScene1[localCounter - 11].PitchYaw[1] + camInfoKhojScene1[localCounter - 10].PitchYaw[1] + camInfoKhojScene1[localCounter - 9].PitchYaw[1] + camInfoKhojScene1[localCounter - 8].PitchYaw[1] + camInfoKhojScene1[localCounter - 7].PitchYaw[1] + camInfoKhojScene1[localCounter - 6].PitchYaw[1] + camInfoKhojScene1[localCounter - 5].PitchYaw[1] + camInfoKhojScene1[localCounter - 4].PitchYaw[1] + camInfoKhojScene1[localCounter - 3].PitchYaw[1] + camInfoKhojScene1[localCounter - 2].PitchYaw[1] + camInfoKhojScene1[localCounter - 1].PitchYaw[1] + camInfoKhojScene1[localCounter].PitchYaw[1] + camInfoKhojScene1[localCounter + 1].PitchYaw[1] + camInfoKhojScene1[localCounter + 2].PitchYaw[1] + camInfoKhojScene1[localCounter + 3].PitchYaw[1] + camInfoKhojScene1[localCounter + 4].PitchYaw[1] + camInfoKhojScene1[localCounter + 5].PitchYaw[1] + camInfoKhojScene1[localCounter + 6].PitchYaw[1] + camInfoKhojScene1[localCounter + 7].PitchYaw[1] + camInfoKhojScene1[localCounter + 8].PitchYaw[1] + camInfoKhojScene1[localCounter + 9].PitchYaw[1] + camInfoKhojScene1[localCounter + 10].PitchYaw[1] + camInfoKhojScene1[localCounter + 11].PitchYaw[1] + camInfoKhojScene1[localCounter + 12].PitchYaw[1] + camInfoKhojScene1[localCounter + 13].PitchYaw[1]) / 27.0f;
	//
	//	fprintf(finalScene, "vmath::vec3(%f,%f,%f),vmath::vec2(%f,%f),\n", camInfoKhojScene1[localCounter].camPos[0], camInfoKhojScene1[localCounter].camPos[1], camInfoKhojScene1[localCounter].camPos[2], averagePitch, averageYaw);
	//	
	//	localCounter += 1;
	//}
	//fprintf(finalScene,"}; ");

	//fclose(finalScene);
};

void ZzRenderer::Update(HWND hwnd)
{
	//Lock Camera position above earth radius
	{
		//vmath::vec3 CamPos = Cam.GetCameraPos();
		//GLfloat dis = vmath::length(CamPos - vmath::vec3(0.0f, (GLfloat)-AtmoScatter->model().EarthRadius, 0.0f));
		//if (dis < AtmoScatter->model().EarthRadius + 10.0f)
		//{
		//	vmath::vec3 ZenithDir = vmath::normalize(CamPos - vmath::vec3(0.0f, (GLfloat)-AtmoScatter->model().EarthRadius, 0.0f));
		//	Cam.SetCameraPos((ZenithDir * (GLfloat)(AtmoScatter->model().EarthRadius + 10.0f)) + vmath::vec3(0.0f, (GLfloat)-AtmoScatter->model().EarthRadius, 0.0f));
		//}
	}
	MyOpenGL::fTime += MyOpenGL::fDeltaTime*0.1f;
	GLuint matrixIndex = MyWin32::iFrameID % 16;
	GLuint haltonIndex = matrixIndex;//16 elements
	GLuint CrossMatrix[4] = { 0, 3, 2, 1 };//later change the sequence to cross
	GLuint CrossMatrixIndex = MyWin32::iFrameID % 4;
	//GLuint tempIndex = MyWin32::iFrameID % 4;

	//vmath::vec4 temp = vmath::vec4(0.9729724f, 0.44444f,0.5555f,0.1111f);

	vmath::vec2 halton23 = vmath::vec2(noise.GetHalton23()[haltonIndex])*RayCaster::fUniformScale*2.0f;

	if (false == MyWin32::TAA)
	{
		halton23 *= 0.0f;
	}
	noise.SetCurrentHalton23(halton23);


#ifdef TEMPORAL_UPSCALE_8K
	MyWin32::iBlockPixelID = noise.GetBayerMatrix4()[matrixIndex];// noise.GetBayerMatrix4()[tempPixelID];//Add bayer matrix here
	vmath::uvec2 UpscalePixelOffset = vmath::uvec2(MyWin32::iBlockPixelID % 4, MyWin32::iBlockPixelID / 4);

#else
//	MyWin32::iBlockPixelID = CrossMatrix[CrossMatrixIndex];// TAAU4k
	MyWin32::iBlockPixelID = noise.GetBayerMatrix4()[matrixIndex];// CloudsHD
	vmath::uvec2 UpscalePixelOffset = vmath::uvec2(MyWin32::iBlockPixelID % 2, MyWin32::iBlockPixelID / 2);
#endif // TEMPORAL_UPSCALE_8K


	//vmath::uvec2 InceptionPixelOffset = vmath::uvec2(CrossMatrix[CrossMatrixIndex] % 2, CrossMatrix[CrossMatrixIndex] / 2);
	vmath::vec2 InceptionPixelOffset[4];
	InceptionPixelOffset[0] = vmath::vec2(0.0f, 0.25f) * 0.0f;
	InceptionPixelOffset[1] = vmath::vec2(0.25f, 0.0f) * 0.0f;
	InceptionPixelOffset[2] = vmath::vec2(0.0f, -0.25f) * 0.0f;
	InceptionPixelOffset[3] = vmath::vec2(-0.25f, 0.0f) * 0.0f;

	//highPrecision depth perspective test
	GLfloat ProjectionRadius = 170000.0f;
	GLfloat CamToOriginLength = vmath::length(Cam.GetCameraPos());
	GLfloat fNear = vmath::max(CamToOriginLength - ProjectionRadius,1.0f);
	GLfloat fFar = CamToOriginLength + ProjectionRadius;

	fNear = 10.0f;
	fFar = fNear+20000000.0f;// related to distance to mars

//	MyWin32::gNearFarFrustum = vmath::dvec2(fNear,fFar);
//	MyWin32::gProjectionMatrix = vmath::Perspective(50.0f, (GLfloat)MyWin32::myClientSize.ClientWidth / (GLfloat)MyWin32::myClientSize.ClientHeight, fNear, fFar);//* max(Cam.GetCameraPos()[1], 1.0f)
//	MyWin32::gProjectionMatrix = vmath::Perspective_FarZeroNearOne(50.0f, (GLfloat)MyWin32::myClientSize.ClientWidth / (GLfloat)MyWin32::myClientSize.ClientHeight, fNear, fFar);//* max(Cam.GetCameraPos()[1], 1.0f)
	MyWin32::gProjectionMatrix = vmath::Perspective_NearZeroFarOneLH(MyWin32::fovY, 1.777777777f, (GLfloat)MyWin32::gNearFarFrustum[0], (GLfloat)MyWin32::gNearFarFrustum[1]);//* max(Cam.GetCameraPos()[1], 1.0f)
//	MyWin32::gProjectionMatrix = vmath::PerspectiveInfinite_DXLH(50.0f, (GLfloat)MyWin32::myClientSize.ClientWidth / (GLfloat)MyWin32::myClientSize.ClientHeight, fNear);//* max(Cam.GetCameraPos()[1], 1.0f)
	//jitter
	{
		MyWin32::gJitteredProjectionMatrix = MyWin32::gProjectionMatrix;
		if (MyWin32::TAA)//TAA
		{
			MyWin32::gJitteredProjectionMatrix[2][0] = ((halton23[0]  ) / (GLfloat)MyWin32::myClientSize.ClientWidth);
			MyWin32::gJitteredProjectionMatrix[2][1] = ((halton23[1]  ) / (GLfloat)MyWin32::myClientSize.ClientHeight);
		}
		else //TTAU
		{
#ifdef TEMPORAL_UPSCALE_8K
			MyWin32::gJitteredProjectionMatrix[2][0] = (((UpscalePixelOffset[0] - 1.5f) * 2.0f + (InceptionPixelOffset[CrossMatrixIndex][0])) / (GLfloat)fboTemporalUpscale4k->GetFboResolution()[0]);
			MyWin32::gJitteredProjectionMatrix[2][1] = (((UpscalePixelOffset[1] - 1.5f) * 2.0f + (InceptionPixelOffset[CrossMatrixIndex][1])) / (GLfloat)fboTemporalUpscale4k->GetFboResolution()[1]);
#else
			MyWin32::gJitteredProjectionMatrix[2][0] = (((UpscalePixelOffset[0] - 0.5f) * 2.0f + (InceptionPixelOffset[CrossMatrixIndex][0])) / (GLfloat)fboTemporalUpscale4k->GetFboResolution()[0]);
			MyWin32::gJitteredProjectionMatrix[2][1] = (((UpscalePixelOffset[1] - 0.5f) * 2.0f + (InceptionPixelOffset[CrossMatrixIndex][1])) / (GLfloat)fboTemporalUpscale4k->GetFboResolution()[1]);
#endif // TEMPORAL_UPSCALE_8K

		}

	}
	//offline
	MyWin32::gJitteredProjectionMatrix = MyWin32::gProjectionMatrix;//

	//MyWin32::gProjectionMatrixForDepth = vmath::perspective(50.0f, (GLfloat)MyWin32::myClientSize.ClientWidth / (GLfloat)MyWin32::myClientSize.ClientHeight, 0.1f * vmath::max(Cam.GetCameraPos()[1], 1.0f), float(20000.0f * vmath::max(Cam.GetCameraPos()[1], 1.0f)));//* max(Cam.GetCameraPos()[1], 1.0f)
	MyWin32::gProjectionMatrixForDepth = vmath::Perspective_NearZeroFarOneLH(70.0f, (GLfloat)MyWin32::myClientSize.ClientWidth / (GLfloat)MyWin32::myClientSize.ClientHeight, 100.0f , 1575700000.0f);//* max(Cam.GetCameraPos()[1], 1.0f)
	//MyWin32::gLightViewProjectionMatrix = vmath::ortho(-5000.0f * 0.225f * 3.0f, 5000.0f * 0.65928f, -5000.0f * 3.0f * 0.161f, 5000.0f  *0.156f * 3.0f, 10000.0f, -10000.0f)*Cam.GetLightViewMatrix();//terrain
	GLfloat LightNear = 10.0f;
	GLfloat LightFar = LightNear + 100000.0f;
	MyWin32::gLightOrthoProjectionMatrix = vmath::Perspective_NearZeroFarOneLH(5.0f , 1.0f, LightNear, LightFar);//clouds
//	MyWin32::gLightOrthoProjectionMatrix = vmath::ortho(-200.0f*0.381f, 200.0f*0.446f, -200.0f*0.1f,200.0f*0.078f,500.0f*0.502f,-500.0f*0.169f);
	MyWin32::gLightViewProjectionMatrix = MyWin32::gLightOrthoProjectionMatrix * Cam.GetLightViewMatrix();//clouds
	//MyWin32::gLightViewProjectionMatrix = MyWin32::gProjectionMatrix *Cam.GetLightViewMatrix();//clouds
	//MyWin32::gJitteredProjectionMatrix;

	//MyWin32::gProjectionMatrix = vmath::translate((halton23[0]*2.0f - 1.0f) /1920.0f*10.0f, (halton23[1]*2.0f - 1.0f)/1080.0f * 10.0f, 0.0f);
	
	if (FALSE == MyWin32::isGUI)
	{
		static bool firstCamPosSet = true;
		//if (firstCamPosSet)
		//{
		//    //Cam.SetCameraPos(vmath::vec3(-3305.18066f, 683.814331f, 119.960991f));
		//	Cam.SetCameraPos(vmath::vec3(0.0f, 0.0f, 0.0f));
		//	Cam.SetYaw(0.0f);
		//	Cam.SetPitch(0.0f);
		//	firstCamPosSet = false;
		//}

		Cam.UpdateViewMatrix(hwnd);
		Cam.UpdateInfiniteViewMatrix();
	}
	UpdateSunPosition();
	UpdateScreenSunPosition();
	Cam.UpdateLightViewMatrix(SunWorldPos,vmath::vec3(0.0f));

	MyWin32::gViewMatrix = Cam.GetViewMatrix();
	MyWin32::gInvViewMatrix = vmath::inverse(MyWin32::gViewMatrix);
	MyWin32::gInvProjectionMatrix = vmath::inverse(MyWin32::gProjectionMatrix);


	MyWin32::gViewProjectionMatrix =MyWin32::gProjectionMatrix* MyWin32::gViewMatrix;
	MyWin32::gViewProjectionMatrixForDepth =MyWin32::gProjectionMatrixForDepth* MyWin32::gViewMatrix;
	Cam.CameraTurboSpeed(MyWin32::myCamStruct.CameraTurboSpeed, MyWin32::bToggleSpaceGroundSpeed);

	//vmath::vec2 uv = vmath::vec2(0.79f, 0.79f);
	//uv = uv ;
	//vmath::vec4 worldPos = vmath::vec4(uv, 1.0, 1.0) * vmath::inverse(MyWin32::gProjectionMatrix).transpose();
	//GLfloat temp = worldPos[3];
	//temp;
	//worldPos /= worldPos[3];
	//worldPos;
	//worldPos = worldPos * MyWin32::gProjectionMatrix.transpose();
	//worldPos;
	//vmath::vec4 uvSpace = (worldPos / worldPos[3]);
	//uvSpace;


	//vmath::mat4 temp =vmath::inverse(MyWin32::gProjectionMatrix)* MyWin32::gProjectionMatrix;
	//temp;
}

void ZzRenderer::Render_Axis3D(void)
{
	glDisable(GL_CULL_FACE);
	Axis3D_Shader.Start();

	//X
	Axis3D_Shader.LoadModelViewXUniform(Cam.GetViewMatrix());
	Axis3D_Shader.LoadProjXUniform(MyWin32::gJitteredProjectionMatrix);
	Axis3D_Shader.Loadv3ColorUniform(vmath::vec3(1.0f, 0.0f, 0.0f));
	Axis3D_Shader.Loadv3ScaleUniform(vmath::vec3(1000.0f, 10.0f, 10.0f));
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//Y
	Axis3D_Shader.LoadModelViewXUniform(Cam.GetViewMatrix());
	Axis3D_Shader.LoadProjXUniform(MyWin32::gJitteredProjectionMatrix);
	Axis3D_Shader.Loadv3ColorUniform(vmath::vec3(0.0f, 1.0f, 0.0f));
	Axis3D_Shader.Loadv3ScaleUniform(vmath::vec3(10.0f, 1000.0f, 10.0f));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//Z
	Axis3D_Shader.LoadModelViewXUniform(Cam.GetViewMatrix());
	Axis3D_Shader.LoadProjXUniform(MyWin32::gJitteredProjectionMatrix);
	Axis3D_Shader.Loadv3ColorUniform(vmath::vec3(0.0f, 0.0f, 2.0f));
	Axis3D_Shader.Loadv3ScaleUniform(vmath::vec3(10.0f, 10.0f, 1000.0f));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//GLfloat Scale = 150000.0f;

	//Axis3D_Shader.LoadModelViewXUniform(Cam.GetViewMatrix() * translate(Scale * 0.1f, 1.0f, 0.0f));
	//Axis3D_Shader.LoadProjXUniform(MyWin32::gJitteredProjectionMatrix);
	//Axis3D_Shader.Loadv3ColorUniform(vmath::vec3(1.0f, 1.0f, 0.0f));
	//Axis3D_Shader.Loadv3ScaleUniform(vmath::vec3(Scale, Scale, Scale));
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	//Axis3D_Shader.LoadModelViewXUniform(Cam.GetViewMatrix());
	//Axis3D_Shader.LoadProjXUniform(MyWin32::gJitteredProjectionMatrix);
	//Axis3D_Shader.Loadv3ColorUniform(vmath::vec3(1.0f, 0.0f, 0.0f));
	//Axis3D_Shader.Loadv3ScaleUniform(vmath::vec3(Scale, Scale, Scale));
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	GLfloat Fcoef = GLfloat(1.0 / (log2(MyWin32::gNearFarFrustum[1]- MyWin32::gNearFarFrustum[0] + 1.0)));
	Axis3D_Shader.LoadfMaxEyeDistanceUniform(Fcoef);

	Axis3D_Shader.Stop();
	glEnable(GL_CULL_FACE);
}

void ZzRenderer::Render_Triangle(vmath::vec4 worldPos)
{
	glDisable(GL_CULL_FACE);
	passThroughShader.Start();

	passThroughShader.LoadModelViewProjXUniform(MyWin32::gProjectionMatrixForDepth*Cam.GetViewMatrix() * vmath::translate(worldPos[0], worldPos[1], worldPos[2]) * vmath::scale(100.0f, 100.0f, 100.0f));
	passThroughShader.LoadiFrameID_Uniform(MyWin32::iFrameID);
	passThroughShader.LoadfTime_Uniform(MyOpenGL::fTime);

	triangle.Render();

	passThroughShader.Stop();
	glEnable(GL_CULL_FACE);
}

void ZzRenderer::Render_GUI(void)
{
	//GUI
//	this->BindDefaultFbo();
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	//clouds
	{
		ImGui::Begin("Clouds");                          // Create a window called "Hello, world!" and append into it.

		//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

		ImGui::SliderFloat("fCloudFactor", clouds->GetfCloudFactor(), 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f                    //clouds.GetfCloudFactor()
		ImGui::ColorEdit3("LightColor", (GLfloat*)clouds->Getv3SunColor()); // Edit 3 floats representing a color

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	//sun
	{
		ImGui::Begin("Sun");  

		ImGui::SliderFloat("fSunPos", &sunAngle, -50.0f, 230.0f);

		ImGui::End();
	}

	////sky
	//{
	//	ImGui::Begin("Atmosphere");  

	//	ImGui::SliderFloat("fSkyFactor", skyDome.GetfSkyFactor(), 0.0f, 1.0f);

	//	ImGui::End();
	//}

	////terrain
	//{
	//	ImGui::Begin("Terrain");

	//	ImGui::SliderFloat("fTerrainFactor", terrain.getTerrainFactor(), 0.0f, 1.0f);
	//	ImGui::ColorEdit3("v3LightColor", (GLfloat*)terrain.Getv3LightColor());

	//	ImGui::End();
	//}

	////water
	//{
	//	ImGui::Begin("Water");

	//	ImGui::SliderFloat("fWaterFactor", water.GetfWaterFactor(), 0.0f, 1.0f);
	//	ImGui::ColorEdit3("v3ColorFactor", (GLfloat*)water.Getv3ColorFactor());

	//	ImGui::End();
	//}
	//Ray-Caster
	{
		ImGui::Begin("Ray-Caster");

		ImGui::SliderFloat("fScale", &RayCaster::fUniformScale, 0.0f, 1.0f);
		ImGui::SliderFloat("fRotAngleX", &RayCaster::fRotAngleX, 0.0f, 360.0f);
		ImGui::SliderFloat("fRotAngleY", &RayCaster::fRotAngleY, 0.0f, 360.0f);
		ImGui::SliderFloat("fRotAngleZ", &RayCaster::fRotAngleZ, 0.0f, 360.0f);

		ImGui::End();
	}
	//TAA
	{
		ImGui::Begin("TAA");

		ImGui::Checkbox("Enable", &MyWin32::TAA);

		ImGui::End();
	}


	// Rendering
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	/////////////////////////////////////////////////////////////////
}

//void ZzRenderer::Render_Terrain(vmath::mat4 lightSpaceMatrix)
//{
//
//	terrainShader.Start();
//	//terrain
//	terrainShader.LoadHeightMapTextureUniform(GL_TEXTURE0, 0, terrain.getHeightMapTextureID());
//	terrainShader.LoadTerrainNormalTextureUniform(GL_TEXTURE1, 1, terrain.getNormalMapTextureID());
//
//	//grass
//	{
//		terrainShader.LoadTerrainGrassTextureUniform(GL_TEXTURE2, 2, terrain.getTextureID_Grass());
//		terrainShader.LoadTerrainGrassAOTextureUniform(GL_TEXTURE3, 3, terrain.getTextureID_GrassAO());
//		terrainShader.LoadTerrainGrassNormalTextureUniform(GL_TEXTURE4, 4, terrain.getTextureID_GrassNormal());
//		terrainShader.LoadTerrainGrassRoughnessTextureUniform(GL_TEXTURE5, 5, terrain.getTextureID_GrassRoughness());
//		terrainShader.LoadTerrainGrassDisplacementTextureUniform(GL_TEXTURE6, 6, terrain.getTextureID_GrassDisplacement());
//	}
//	//sand
//	{
//		terrainShader.LoadTerrainSandTextureUniform(GL_TEXTURE7, 7, terrain.getTextureID_Sand());
//		terrainShader.LoadTerrainSandAOTextureUniform(GL_TEXTURE8, 8, terrain.getTextureID_SandAO());
//		terrainShader.LoadTerrainSandNormalTextureUniform(GL_TEXTURE9, 9, terrain.getTextureID_SandNormal());
//		terrainShader.LoadTerrainSandRoughnessTextureUniform(GL_TEXTURE10, 10, terrain.getTextureID_SandRoughness());
//		terrainShader.LoadTerrainSandDisplacementTextureUniform(GL_TEXTURE11, 11, terrain.getTextureID_SandDisplacement());
//	}
//	//rock
//	{
//		terrainShader.LoadTerrainRockTextureUniform(GL_TEXTURE12, 12, terrain.getTextureID_Rock());
//		terrainShader.LoadTerrainRockAOTextureUniform(GL_TEXTURE13, 13, terrain.getTextureID_RockAO());
//		terrainShader.LoadTerrainRockNormalTextureUniform(GL_TEXTURE14, 14, terrain.getTextureID_RockNormal());
//		terrainShader.LoadTerrainRockRoughnessTextureUniform(GL_TEXTURE15, 15, terrain.getTextureID_RockRoughness());
//		terrainShader.LoadTerrainRockDisplacementTextureUniform(GL_TEXTURE16, 16, terrain.getTextureID_RockDisplacement());
//	}
//	//talus
//	{
//		terrainShader.LoadTerrainTalusTextureUniform(GL_TEXTURE17, 17, terrain.getTextureID_Talus());
//		terrainShader.LoadTerrainTalusAOTextureUniform(GL_TEXTURE18, 18, terrain.getTextureID_TalusAO());
//		terrainShader.LoadTerrainTalusNormalTextureUniform(GL_TEXTURE19, 19, terrain.getTextureID_TalusNormal());
//		terrainShader.LoadTerrainTalusRoughnessTextureUniform(GL_TEXTURE20, 20, terrain.getTextureID_TalusRoughness());
//		terrainShader.LoadTerrainTalusDisplacementTextureUniform(GL_TEXTURE21, 21, terrain.getTextureID_TalusDisplacement());
//	}
//
//	//map
//	terrainShader.LoadTerrainLandSWTR_TextureUniform(GL_TEXTURE22, 22, terrain.getTextureID_LandSWTR());
//	terrainShader.LoadTerrainWaterRBWD_TextureUniform(GL_TEXTURE23, 23, terrain.getTextureID_WaterRBWD());
//	terrainShader.LoadTerrainWaterGradient_TextureUniform(GL_TEXTURE24, 24, terrain.getTextureID_WaterGradient());
//
//	terrainShader.LoadTerrainFactorUniform(*terrain.getTerrainFactor());
//
//	terrainShader.LoadModelXUniform(terrain.GetModelMatrix());
//	terrainShader.LoadViewXUniform(Cam.GetViewMatrix());
//	terrainShader.LoadProjXUniform(MyWin32::gProjectionMatrix);
//
//	terrainShader.LoadModelViewProjXUniform(MyWin32::gViewProjectionMatrix * terrain.GetModelMatrix());
//
//	terrainShader.Loadv3SunPosUniform(SunPos);
//	terrainShader.Loadv3EyePosUniform(Cam.GetCameraPos());
//
//	GLdouble SunTheta = vmath::dot(vmath::normalize(this->SunPos), vec3(0.0f, 1.0f, 0.0f));
//	vmath::vec3 lightColor = terrain.getLightColor(SunTheta);
//	terrainShader.Loadv3LightColorUniform(lightColor);
//
//	//shadow
//	terrainShader.Loadm4LightSpaceXUniform(lightSpaceMatrix);
//	terrainShader.LoadShadowMapTextureUniform(GL_TEXTURE25,25,fboShadowMap->GetFbo_Depth_Texture());
//
//	terrain.Render();
//	terrainShader.Stop();
//
//
//	////UnderWaterQuad
//	//glFrontFace(GL_CW);
//
//	//UnderWaterQuadShader.Start();
//
//	//vmath::mat4 modelMatrix = vmath::translate(0.0f, -334.0f, 0.0f) * vmath::scale(1000000.0f);
//
//	//UnderWaterQuadShader.LoadModelViewProjXUniform(MyWin32::gViewProjectionMatrix* modelMatrix);
//	//UnderWaterQuadShader.LoadModelXUniform(modelMatrix);
//	//UnderWaterQuadShader.Loadv3ColorUniform(lightColor );//*clouds.Getv3SunColor()
//	//UnderWaterQuadShader.Loadv3SunPosUniform(this->SunPos);
//	//UnderWaterQuadShader.Loadv3EyePosUniform(Cam.GetCameraPos());
//	//glDrawArrays(GL_QUADS, 0, 4);
//
//	//UnderWaterQuadShader.Stop();
//
//	//glFrontFace(GL_CCW);
//}

void ZzRenderer::Render_DeformedSphere(void)
{
	DeformedSphereShader.Start();

	DeformedSphereShader.LoadModelXUniform(earth.GetModelMatrix());
	DeformedSphereShader.LoadViewXUniform(Cam.GetViewMatrix());
	DeformedSphereShader.LoadProjXUniform(MyWin32::gProjectionMatrix);

	DeformedSphereShader.LoadModelViewProjXUniform(MyWin32::gViewProjectionMatrix * earth.GetModelMatrix());

	DeformedSphereShader.LoadSphereMap_TextureUniform(GL_TEXTURE0, 0, earth.getTextureID_EarthAlbedoSpec());

	earth.Render();

	DeformedSphereShader.Stop();
}

//void ZzRenderer::Render_TerrainLOD2(void)
//{
//
//	terrainLOD2Shader.Start();
//	//terrain
//	terrainLOD2Shader.LoadHeightMapTextureUniform(GL_TEXTURE0, 0, terrainLOD2.getHeightMapTextureID());
//	terrainLOD2Shader.LoadTerrainNormalTextureUniform(GL_TEXTURE1, 1, terrainLOD2.getNormalMapTextureID());
//
//	////grass
//	//{
//	//	terrainShader.LoadTerrainGrassTextureUniform(GL_TEXTURE2, 2, terrain.getTextureID_Grass());
//	//	terrainShader.LoadTerrainGrassAOTextureUniform(GL_TEXTURE3, 3, terrain.getTextureID_GrassAO());
//	//	terrainShader.LoadTerrainGrassNormalTextureUniform(GL_TEXTURE4, 4, terrain.getTextureID_GrassNormal());
//	//	terrainShader.LoadTerrainGrassRoughnessTextureUniform(GL_TEXTURE5, 5, terrain.getTextureID_GrassRoughness());
//	//	terrainShader.LoadTerrainGrassDisplacementTextureUniform(GL_TEXTURE6, 6, terrain.getTextureID_GrassDisplacement());
//	//}
//	
//	//map
//	terrainLOD2Shader.LoadTerrainWearSlopeSnowAO_TextureUniform(GL_TEXTURE2, 2, terrainLOD2.getTextureID_WearSlopeSnowAO());
//	//terrainLOD2Shader.LoadTerrainWaterGradient_TextureUniform(GL_TEXTURE24, 24, terrain.getTextureID_WaterGradient());
//
//	terrainLOD2Shader.LoadTerrainFactorUniform(*terrain.getTerrainFactor());
//
//	terrainLOD2Shader.LoadModelXUniform(terrain.GetModelMatrix());
//	terrainLOD2Shader.LoadViewXUniform(Cam.GetViewMatrix());
//	terrainLOD2Shader.LoadProjXUniform(MyWin32::gProjectionMatrix);
//
//	terrainLOD2Shader.LoadModelViewProjXUniform(MyWin32::gViewProjectionMatrix * terrain.GetModelMatrix());
//
//	//terrainLOD2Shader.Loadv3SunPosUniform(SunPos);
//	//terrainLOD2Shader.Loadv3EyePosUniform(Cam.GetCameraPos());
//
//	//terrainLOD2Shader.Loadv3LightColorUniform(lightColor);
//
//	//shadow
//	//terrainShader.Loadm4LightSpaceXUniform(lightSpaceMatrix);
//	//terrainShader.LoadShadowMapTextureUniform(GL_TEXTURE25, 25, fboShadowMap->GetFbo_Depth_Texture());
//
//	terrainLOD2.Render();
//	terrainLOD2Shader.Stop();
//
//}

//void ZzRenderer::Render_EarthLOD1(void)
//{
//
//	EarthLOD1Shader.Start();
//	//terrain
//	//EarthLOD1Shader.LoadNormalMapTextureUniform(GL_TEXTURE1, 1, terrainLOD2.getNormalMapTextureID());
//
//
//	//map
//	EarthLOD1Shader.LoadEarthLOD1Albedo_TextureUniform(GL_TEXTURE0, 0, earth.getTextureID_EarthLOD1());
//	EarthLOD1Shader.LoadHeightMapTextureUniform(GL_TEXTURE1, 1, earth.getTextureID_EarthLOD1_HeightMap());
//	EarthLOD1Shader.LoadEarthLODBlend_TextureUniform(GL_TEXTURE2, 2, earth.getTextureID_EarthLodBlendMap());
//	EarthLOD1Shader.LoadNormalAOMapTextureUniform(GL_TEXTURE3, 3, earth.getTextureID_EarthLOD1_NormalAO());
//
//	EarthLOD1Shader.LoadEarthLOD1FactorUniform(RayCaster::fUniformScale);
//	//EarthLOD1Shader.LoadEarthLOD1FactorUniform(*terrain.getTerrainFactor());
//
//	vmath::mat4 matrix =vmath::translate(-12.5586332f,0.0f, 54.945f)*vmath::rotate(267.07755f,0.0f,1.0f,0.0f)* vmath::scale(38.78f);
//
//	EarthLOD1Shader.LoadModelXUniform(matrix);
//	EarthLOD1Shader.LoadViewXUniform(Cam.GetViewMatrix());
//	EarthLOD1Shader.LoadProjXUniform(MyWin32::gProjectionMatrix);
//
//	EarthLOD1Shader.LoadModelViewProjXUniform(MyWin32::gViewProjectionMatrix * matrix);
//
//	//terrainLOD2Shader.Loadv3SunPosUniform(SunPos);
//	//terrainLOD2Shader.Loadv3EyePosUniform(Cam.GetCameraPos());
//
//	earth.Render();
//	EarthLOD1Shader.Stop();
//
//}

void ZzRenderer::Render_EarthLODIndia(vmath::vec3 WaterTint,GLuint isCullIndiaLOD)
{

	EarthLODIndiaShader.Start();
	//terrain
	//EarthLOD1Shader.LoadNormalMapTextureUniform(GL_TEXTURE1, 1, terrainLOD2.getNormalMapTextureID());


	//map
	EarthLODIndiaShader.LoadEarthLODIndiaAlbedo_TextureUniform(GL_TEXTURE0, 0, earth.getTextureID_EarthLODIndia());
	EarthLODIndiaShader.LoadHeightMapTextureUniform(GL_TEXTURE1, 1, earth.getTextureID_EarthLODIndia_HeightMap());
	EarthLODIndiaShader.LoadEarthLODBlend_TextureUniform(GL_TEXTURE2, 2, earth.getTextureID_EarthLodBlendMap());
	EarthLODIndiaShader.LoadNormalAOMapTextureUniform(GL_TEXTURE3, 3, earth.getTextureID_EarthLODIndia_NormalAO());

	EarthLODIndiaShader.LoadEarthLODIndiaFactorUniform(RayCaster::fUniformScale);
	GLfloat Fcoef = GLfloat(1.0 / (log2((MyWin32::gNearFarFrustum[1] - MyWin32::gNearFarFrustum[0]) + 1.0)));
	EarthLODIndiaShader.LoadFcoefForLogUniform(Fcoef);
	//EarthLODIndiaShader.LoadEarthLOD1FactorUniform(*terrain.getTerrainFactor());

	vmath::mat4 matrix = vmath::translate(-4121.376f, 0.0f,3982.28288f) * vmath::rotate(95.24493f, 0.0f, 1.0f, 0.0f) * vmath::scale(336.069f);

	EarthLODIndiaShader.LoadModelXUniform(matrix);
	EarthLODIndiaShader.LoadEarthRotXUniform(earth.GetEarthIndiaRotUVMatrix());//vmath::rotate(17.143f, 0.0f, 1.0f, 0.0f) *vmath::rotate(67.0f, 1.0f, 0.0f, 0.0f) * 
	EarthLODIndiaShader.LoadPrevViewProjXUniform(MyWin32::gProjectionMatrix * MyWin32::gPreviousViewMatrix);
	EarthLODIndiaShader.LoadViewProjXUniform(MyWin32::gProjectionMatrix* Cam.GetViewMatrix());
	EarthLODIndiaShader.LoadJitteredViewProjXUniform(MyWin32::gJitteredProjectionMatrix* Cam.GetViewMatrix());

	EarthLODIndiaShader.Loadv3TintColor_Uniform(WaterTint);
	EarthLODIndiaShader.LoadIsCullIndiaLOD_Uniform(isCullIndiaLOD);
	//terrainLOD2Shader.Loadv3EyePosUniform(Cam.GetCameraPos());

	earth.Render();
	EarthLODIndiaShader.Stop();

	//TEST

}

void ZzRenderer::Render_EarthLODEverest(void)
{

	EarthLODEverestShader.Start();
	//terrain
	//EarthLOD1Shader.LoadNormalMapTextureUniform(GL_TEXTURE1, 1, terrainLOD2.getNormalMapTextureID());


	//map
	EarthLODEverestShader.LoadSampler2D_Albedo_Uniform(GL_TEXTURE0, 0, earth.getTextureID_EarthLODEverest());
	EarthLODEverestShader.LoadSampler2D_HeightMap_Uniform(GL_TEXTURE1, 1, earth.getTextureID_EarthLODEverest_HeightMap());
	EarthLODEverestShader.LoadSampler2D_LodBlend_Uniform(GL_TEXTURE2, 2, earth.getTextureID_EarthLodBlendMap());
	EarthLODEverestShader.LoadSampler2D_NormalAOmap_Uniform(GL_TEXTURE3, 3, earth.getTextureID_EarthLODEverest_NormalAO());

	EarthLODEverestShader.LoadArtistFactorUniform(RayCaster::fUniformScale);
	GLfloat Fcoef = GLfloat(1.0 / (log2((MyWin32::gNearFarFrustum[1] - MyWin32::gNearFarFrustum[0]) + 1.0)));
	EarthLODEverestShader.LoadFcoefForLogUniform(Fcoef);
	//EarthLODIndiaShader.LoadEarthLOD1FactorUniform(*terrain.getTerrainFactor());

	vmath::mat4 matrix = vmath::translate(140.2f, 0.0f, -1786.8f) * vmath::rotate(96.80293f, 0.0f, 1.0f, 0.0f) * vmath::scale(65.9456f);

	EarthLODEverestShader.LoadModelX_Uniform(matrix);
	EarthLODEverestShader.LoadEarthRotX_Uniform(earth.GetEarthIndiaRotUVMatrix());
	EarthLODEverestShader.LoadPrevViewProjX_Uniform(MyWin32::gProjectionMatrix * MyWin32::gPreviousViewMatrix);
	EarthLODEverestShader.LoadViewProjX_Uniform(MyWin32::gProjectionMatrix * Cam.GetViewMatrix());
	EarthLODEverestShader.LoadJitteredViewProjX_Uniform(MyWin32::gJitteredProjectionMatrix * Cam.GetViewMatrix());

	//terrainLOD2Shader.Loadv3SunPosUniform(SunPos);
	//terrainLOD2Shader.Loadv3EyePosUniform(Cam.GetCameraPos());

	earth.Render();
	EarthLODEverestShader.Stop();

	//TEST

}

void ZzRenderer::Render_TerrainBasic3D(GLboolean Shadow)
{
	if (Shadow)
	{
		TerrainBasicShader.Start();

		vmath::mat4 FinalModelMatrix = TerrainTreeFog.GetModelMatrix();//PaleBlueLife *vmath::translate(0.0f,-20.0f*0.018f,0.0f) * vmath::rotate(42.0f, 0.0f, 1.0f, 0.0f)

		TerrainBasicShader.LoadModelX_Uniform( FinalModelMatrix);
		TerrainBasicShader.LoadViewX_Uniform(Cam.GetLightViewMatrix());
		TerrainBasicShader.LoadProjX_Uniform(MyWin32::gLightOrthoProjectionMatrix);
		TerrainBasicShader.LoadModelViewProjX_Uniform(MyWin32::gLightViewProjectionMatrix * FinalModelMatrix);
	//	TerrainBasicShader.LoadPrevViewX_Uniform(MyWin32::gPreviousViewMatrix);
		TerrainBasicShader.LoadJitteredProjX_Uniform(MyWin32::gLightOrthoProjectionMatrix);

		TerrainBasicShader.LoadArtistFactor_Uniform(RayCaster::fUniformScale);

		TerrainBasicShader.LoadHeightMapTexture_Uniform(GL_TEXTURE0, 0, TerrainTreeFog.getHeightMapTextureID());
		TerrainBasicShader.LoadTerrainNormalTexture_Uniform(GL_TEXTURE1, 1, TerrainTreeFog.getNormalMapTextureID());

		TerrainBasicShader.LoadSampler2D_MaterialAlbedo_Uniform(GL_TEXTURE2, 2, TerrainTreeFog.getTextureID_MaterialAlbedo());
		TerrainBasicShader.LoadSampler2D_MaterialNormal_Uniform(GL_TEXTURE3, 3, TerrainTreeFog.getTextureID_MaterialNormal());
		TerrainBasicShader.LoadSampler2D_MaterialRoughness_Uniform(GL_TEXTURE4, 4, TerrainTreeFog.getTextureID_MaterialRoughness());
		TerrainBasicShader.LoadSampler2D_MaterialAO_Uniform(GL_TEXTURE5, 5, TerrainTreeFog.getTextureID_MaterialAO());

		TerrainTreeFog.Render();

		TerrainBasicShader.Stop();
	}
	else
	{
		TerrainBasicShader.Start();

		vmath::mat4 FinalModelMatrix = TerrainTreeFog.GetModelMatrix() ;

		TerrainBasicShader.LoadModelX_Uniform(vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f)*vmath::translate(400.0f*0.25f,0.0f,0.0f) * FinalModelMatrix);
		TerrainBasicShader.LoadViewX_Uniform(Cam.GetViewMatrix());
		TerrainBasicShader.LoadProjX_Uniform(MyWin32::gProjectionMatrix);
		TerrainBasicShader.LoadModelViewProjX_Uniform(MyWin32::gViewProjectionMatrix * FinalModelMatrix);
		TerrainBasicShader.LoadPrevViewX_Uniform(MyWin32::gPreviousViewMatrix);
		TerrainBasicShader.LoadJitteredProjX_Uniform(MyWin32::gJitteredProjectionMatrix);

		TerrainBasicShader.LoadArtistFactor_Uniform(RayCaster::fUniformScale);

		TerrainBasicShader.LoadHeightMapTexture_Uniform(GL_TEXTURE0, 0, TerrainTreeFog.getHeightMapTextureID());
		TerrainBasicShader.LoadTerrainNormalTexture_Uniform(GL_TEXTURE1, 1, TerrainTreeFog.getNormalMapTextureID());

		TerrainBasicShader.LoadSampler2D_MaterialAlbedo_Uniform(GL_TEXTURE2, 2, TerrainTreeFog.getTextureID_MaterialAlbedo());
		TerrainBasicShader.LoadSampler2D_MaterialNormal_Uniform(GL_TEXTURE3, 3, TerrainTreeFog.getTextureID_MaterialNormal());
		TerrainBasicShader.LoadSampler2D_MaterialRoughness_Uniform(GL_TEXTURE4, 4, TerrainTreeFog.getTextureID_MaterialRoughness());
		TerrainBasicShader.LoadSampler2D_MaterialAO_Uniform(GL_TEXTURE5, 5, TerrainTreeFog.getTextureID_MaterialAO());

		TerrainTreeFog.Render();

		TerrainBasicShader.Stop();
	}

	
}

//void ZzRenderer::Render_Ground(GLuint textureSet,vmath::vec3 tint,GLfloat time,vmath::mat4 lightSpaceMatrix)
//{
//
//	groundShader.Start();
//
//	if (textureSet == 0)
//	{
//		//	Mossy
//		{
//			groundShader.LoadTerrainGroundTextureUniform(GL_TEXTURE2, 2, ground.getTextureID_Ground());
//			groundShader.LoadTerrainGroundAOTextureUniform(GL_TEXTURE3, 3, ground.getTextureID_GroundAO());
//			groundShader.LoadTerrainGroundNormalTextureUniform(GL_TEXTURE4, 4, ground.getTextureID_GroundNormal());
//			groundShader.LoadTerrainGroundRoughnessTextureUniform(GL_TEXTURE5, 5, ground.getTextureID_GroundRoughness());
//			groundShader.LoadTerrainGroundDisplacementTextureUniform(GL_TEXTURE6, 6, ground.getTextureID_GroundDisplacement());
//		}
//	}
//	else
//	{
//		//Rocky
//		{
//			groundShader.LoadTerrainGroundTextureUniform(GL_TEXTURE2, 2, ground.getTextureID_Scene1());
//			groundShader.LoadTerrainGroundAOTextureUniform(GL_TEXTURE3, 3, ground.getTextureID_Scene1AO());
//			groundShader.LoadTerrainGroundNormalTextureUniform(GL_TEXTURE4, 4, ground.getTextureID_Scene1Normal());
//			groundShader.LoadTerrainGroundRoughnessTextureUniform(GL_TEXTURE5, 5, ground.getTextureID_Scene1Roughness());
//			groundShader.LoadTerrainGroundDisplacementTextureUniform(GL_TEXTURE6, 6, ground.getTextureID_Scene1Displacement());
//		}
//	}
//	////grass
//	//{
//	//	groundShader.LoadTerrainGroundTextureUniform(GL_TEXTURE2, 2, terrain.getTextureID_Grass());
//	//	groundShader.LoadTerrainGroundAOTextureUniform(GL_TEXTURE3, 3, terrain.getTextureID_GrassAO());
//	//	groundShader.LoadTerrainGroundNormalTextureUniform(GL_TEXTURE4, 4, terrain.getTextureID_GrassNormal());
//	//	groundShader.LoadTerrainGroundRoughnessTextureUniform(GL_TEXTURE5, 5, terrain.getTextureID_GrassRoughness());
//	//	groundShader.LoadTerrainGroundDisplacementTextureUniform(GL_TEXTURE6, 6, terrain.getTextureID_GrassDisplacement());
//	//}
//
//
//	//Scene1
//
//
//	groundShader.LoadShadowMapTextureUniform(GL_TEXTURE7, 7, fboShadowMap->GetFbo_Depth_Texture());
//	
//
//	//terrainShader.LoadTerrainFactorUniform(*terrain.getTerrainFactor());
//	groundShader.LoadfTimeUniform(time);
//
//	groundShader.LoadModelXUniform(ground.GetModelMatrix());
//	groundShader.LoadViewXUniform(Cam.GetViewMatrix());
//	groundShader.LoadProjXUniform(MyWin32::gProjectionMatrixForDepth);
//
//	groundShader.LoadModelViewProjXUniform(MyWin32::gViewProjectionMatrix * ground.GetModelMatrix());
//	groundShader.Loadm4LightSpaceXUniform(lightSpaceMatrix);
//
//	groundShader.Loadv3SunPosUniform(SunPos);
//	groundShader.Loadv3EyePosUniform(Cam.GetCameraPos());
//
//	//GLdouble SunTheta = vmath::dot(vmath::normalize(this->SunPos), vec3(0.0f, 1.0f, 0.0f));
//	groundShader.Loadv3LightColorUniform(tint); //  (0.985f, 0.556f, 0.256f, 1.000f)//*clouds.Getv3SunColor()
//
//	ground.Render();
//	groundShader.Stop();
//}

//void ZzRenderer::Render_Water(GLfloat smoothEdgeFactor)
//{
//	//Get RefractionTexture
//	GLuint refractionTexture = water.GetTextureID_previousSceneTexture();
//
//	glCopyImageSubData(fboScene->GetFbo_Color_Texture(0),
//		GL_TEXTURE_2D, 0, 0, 0, 0,
//		refractionTexture,
//		GL_TEXTURE_2D, 0, 0, 0, 0,
//		MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight, 1);
//
//	GLuint terrainDepthTexture = water.GetTextureID_TerrainDepthTexture();
//
//	glCopyImageSubData(fboScene->GetFbo_Depth_Texture(),
//		GL_TEXTURE_2D, 0, 0, 0, 0,
//		terrainDepthTexture,
//		GL_TEXTURE_2D, 0, 0, 0, 0,
//		MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight, 1);
//
//	//updateHeightmap
//	water.UpdateWater(noise.GetTextureID_PerlinWorley3D(),noise.GetTextureID_Permutation(), MyOpenGL::fTime);//MyOpenGL::fTime
//
//	////DrawMesh
//	waterShaderLoader.Start();
//	//terrain
//	waterShaderLoader.LoadHeightMapTextureUniform(GL_TEXTURE0, 0, water.getHeightMapTextureID());	
//	waterShaderLoader.LoadNormalMapTextureUniform(GL_TEXTURE1, 1, water.getNormalMapTextureID());	
//	waterShaderLoader.LoadRefractionTextureUniform(GL_TEXTURE2, 2, refractionTexture);
//	waterShaderLoader.LoadReflectionTextureUniform(GL_TEXTURE3, 3, fboPostProcess->GetFbo_Color_Texture(0));
//	waterShaderLoader.LoadTerrainDepthTextureUniform(GL_TEXTURE4, 4, terrainDepthTexture);
//
//	waterShaderLoader.Loadv3EyePosUniform(Cam.GetCameraPos());
//	waterShaderLoader.Loadv3SunPosUniform(this->SunPos);
//	waterShaderLoader.Loadv3EyeDirUniform(Cam.GetCameraDirection());
//	waterShaderLoader.Loadv3ColorFactorUniform(*water.Getv3ColorFactor());
//
//	GLdouble SunTheta = vmath::dot(vmath::normalize(this->SunPos), vec3(0.0f, 1.0f, 0.0f));
//	waterShaderLoader.Loadv3SpecularColorUniform(water.getSpecularColor(SunTheta));
//
//	waterShaderLoader.LoadModelXUniform(water.GetModelMatrix());
//	waterShaderLoader.LoadViewXUniform(Cam.GetViewMatrix());
//
//	waterShaderLoader.LoadfWaterFactorUniform(smoothEdgeFactor);
//
//	waterShaderLoader.LoadProjXUniform(MyWin32::gProjectionMatrixForDepth);
//
//	waterShaderLoader.LoadModelViewProjXUniform(MyWin32::gViewProjectionMatrix* water.GetModelMatrix());// MyWin32::gViewProjectionMatrix
//
//	water.Render();
//	waterShaderLoader.Stop();
//
//}

//void  ZzRenderer::Render_SkyDome(void)
//{
//	this->BindFboSky();
//	skyDomeShader.Start();
//	//vectors
//	skyDomeShader.LoadfSkyFactor_Uniform(*skyDome.GetfSkyFactor());
//	skyDomeShader.LoadiFrameID_Uniform(MyWin32::iFrameID);//0
//	skyDomeShader.LoadModelViewProjXUniform(MyWin32::gProjectionMatrix * Cam.GetInfiniteViewMatrix() * skyDome.GetModelMatrix());    //1
//	skyDomeShader.LoadCameraPosUniform(vmath::vec3(0.0f,skyDome.GetfInnerRadius(),0.0f));	//2
//	skyDomeShader.LoadLightPosUniform(SunPos);//3
//	skyDomeShader.LoadInverseWavelengthUniform(skyDome.Getv3InvWavelength());//4
//
//	skyDomeShader.LoadInnerRadiusUniform(skyDome.GetfInnerRadius());//5
//	skyDomeShader.LoadOuterRadiusUniform(skyDome.GetfOuterRadius());//6
//
//	skyDomeShader.LoadKrESunUniform(skyDome.GetfKrESun());//7
//	skyDomeShader.LoadKmESunUniform(skyDome.GetfKmESun());//8
//	skyDomeShader.LoadKr4PIUniform(skyDome.GetfKr4PI());//9
//	skyDomeShader.LoadKm4PIUniform(skyDome.GetfKm4PI());//10
//
//	skyDomeShader.LoadScaleDepthUniform(skyDome.GetfScaleDepth());//11
//	skyDomeShader.LoadScaleUniform(skyDome.GetfScale());//12
//	skyDomeShader.LoadScaleOverScaleDepthUniform(skyDome.GetfScaleOverScaleDepth());//13
//
//	skyDomeShader.LoadTextureID_BlueNoiseUniform(GL_TEXTURE0, 0, noise.GetTextureID_BlueNoise());//14
//
//	//RENDER
//	skyDome.Render();
//
//	skyDomeShader.Stop();
//
//}

void ZzRenderer::Render_DefferedSpeedTreeInstanced(GLboolean shadow)
{
	//TO ADD TREES (RAY CAST)
////SpeedTree
//if (TRUE == MyWin32::addObject)
//{
//	FILE* fp = NULL;
//	fopen_s(&fp, "treeData.zz", "a");

////	RayCaster::fRotAngleY =( rand() / float(RAND_MAX)) * 360.0f;//not working as intented(solve later)
////	RayCaster::fUniformScale =(( rand() / float(RAND_MAX)) * 0.0340f)+0.04f;//not working as intented(solve later)


//	if (fp)
//	{
//		fprintf(fp, "Z %f %f %f %f %f %f %f\n", RayCaster::WorldPos[0], RayCaster::WorldPos[1], RayCaster::WorldPos[2], RayCaster::fRotAngleX, RayCaster::fRotAngleY, RayCaster::fRotAngleZ, RayCaster::fUniformScale);
//		fclose(fp);
//	}

//	PerInstanceData temp;
//	temp.WorldPos = RayCaster::WorldPos;
//	temp.fRotAngleX = RayCaster::fRotAngleX;
//	temp.fRotAngleY = RayCaster::fRotAngleY;
//	temp.fRotAngleZ = RayCaster::fRotAngleZ;
//	temp.fUniformScale = RayCaster::fUniformScale;

//	cullIndirect.AddPerInstanceMatrix(temp);

//	MyWin32::addObject = FALSE;
//}

	if (shadow == GL_FALSE)
	{
		{
			InstanceModelData* temp = BroadLeafInstanced->GetMyInstanceModelData();
		//	temp->InstanceModelMatrices[0] = vmath::translate(RayCaster::WorldPos) * vmath::rotate(RayCaster::fRotAngleZ, 0.0f, 0.0f, 1.0f) * vmath::rotate(RayCaster::fRotAngleY, 0.0f, 1.0f, 0.0f) * vmath::rotate(RayCaster::fRotAngleX, 1.0f, 0.0f, 0.0f) * vmath::scale(0.002f + 0.0005f);

			if (TRUE == MyWin32::addObject && temp->CurrentInstanceCount < temp->maxInstancesCount)
			{

				GLfloat AngleY = ((float)rand() / RAND_MAX) * 360.0f;
				GLfloat AngleX = 270.0f;
				GLfloat AngleZ = RayCaster::fRotAngleZ;
				GLfloat UniformScale = 0.002f + ((float)rand() / RAND_MAX) * 0.0005f;

				temp->InstanceModelMatrices[temp->CurrentInstanceCount] = vmath::translate(RayCaster::WorldPos) * vmath::rotate(AngleZ, 0.0f, 0.0f, 1.0f) * vmath::rotate(AngleY, 0.0f, 1.0f, 0.0f) * vmath::rotate(AngleX, 1.0f, 0.0f, 0.0f) * vmath::scale(UniformScale);
				temp->CurrentInstanceCount += 1;

				MyWin32::addObject = FALSE;

				FILE* fp = NULL;
				fopen_s(&fp, "treeData.zz", "a");

				if (fp)
				{
					fprintf(fp, "Z %f %f %f %f %f %f %f\n", RayCaster::WorldPos[0], RayCaster::WorldPos[1], RayCaster::WorldPos[2], AngleX, AngleY, AngleZ, UniformScale);
					fclose(fp);
				}
			}
		}

		DefferedPBR_SpeedTreeInstancedShader.Start();

		DefferedPBR_SpeedTreeInstancedShader.LoadViewProjXUniform(MyWin32::gProjectionMatrix * Cam.GetViewMatrix());
		DefferedPBR_SpeedTreeInstancedShader.LoadModelViewProjXUniform(MyWin32::gJitteredProjectionMatrix * Cam.GetViewMatrix());
		DefferedPBR_SpeedTreeInstancedShader.LoadPrevViewProjXUniform(MyWin32::gProjectionMatrix * MyWin32::gPreviousViewMatrix);

		DefferedPBR_SpeedTreeInstancedShader.LoadAlbedoTextureUniform(GL_TEXTURE0, 0, BroadLeafInstanced->getAlbedoMapTextureID());
		DefferedPBR_SpeedTreeInstancedShader.LoadNormalTextureUniform(GL_TEXTURE1, 1, BroadLeafInstanced->getNormalMapTextureID());


		glDisable(GL_CULL_FACE);
		BroadLeafInstanced->Render();
		glEnable(GL_CULL_FACE);

		DefferedPBR_SpeedTreeInstancedShader.Stop();
	}
	else
	{
		DefferedPBR_SpeedTreeInstancedShader.Start();

		DefferedPBR_SpeedTreeInstancedShader.LoadViewProjXUniform(MyWin32::gProjectionMatrix * Cam.GetViewMatrix());
		DefferedPBR_SpeedTreeInstancedShader.LoadModelViewProjXUniform(MyWin32::gLightViewProjectionMatrix);
		DefferedPBR_SpeedTreeInstancedShader.LoadPrevViewProjXUniform(MyWin32::gProjectionMatrix * MyWin32::gPreviousViewMatrix);

		DefferedPBR_SpeedTreeInstancedShader.LoadAlbedoTextureUniform(GL_TEXTURE0, 0, BroadLeafInstanced->getAlbedoMapTextureID());
		DefferedPBR_SpeedTreeInstancedShader.LoadNormalTextureUniform(GL_TEXTURE1, 1, BroadLeafInstanced->getNormalMapTextureID());


		glDisable(GL_CULL_FACE);
		BroadLeafInstanced->Render();
		glEnable(GL_CULL_FACE);

		DefferedPBR_SpeedTreeInstancedShader.Stop();
	}


}

void ZzRenderer::Render_SkyDistant(void)
{

	GLfloat  EARTH_RADIUS = (GLfloat)AtmoScatter->model().EarthRadius;

	GLfloat sphereInnerRadius = EARTH_RADIUS * 0.0031446541f;
	GLfloat sphereOuterRadius = 2.0f * EARTH_RADIUS * 0.002f;

	GLfloat SPHERE_INNER_RADIUS = (EARTH_RADIUS + sphereInnerRadius);
	GLfloat SPHERE_OUTER_RADIUS = (SPHERE_INNER_RADIUS + sphereOuterRadius);
	GLfloat SPHERE_DELTA = (SPHERE_OUTER_RADIUS - SPHERE_INNER_RADIUS);

	vmath::vec3 EarthCenter = vmath::vec3(0.0, -EARTH_RADIUS, 0.0);

	GLfloat CameraHeightFraction = (vmath::length(Cam.GetCameraPos() - EarthCenter) - SPHERE_INNER_RADIUS) / (SPHERE_DELTA);

	CameraHeightFraction = vmath::clamp(CameraHeightFraction, 0.0f, 1.0f);

	//*(clouds->GetfCloudFactor())= CameraHeightFraction;

	BindFboSkyDistant();

	SkyDistant_Shader->Start();

	SkyDistant_Shader->LoadInvProjX_Uniform(MyWin32::gInvProjectionMatrix);
	SkyDistant_Shader->LoadInvViewX_Uniform(MyWin32::gInvViewMatrix);
	SkyDistant_Shader->LoadEarthCenter_Uniform(vmath::vec3(0.0f, (GLfloat)-AtmoScatter->model().EarthRadius, 0.0f));
	SkyDistant_Shader->LoadEyePosition_Uniform(Cam.GetCameraPos()-vec3(0.0f,400.0f* CameraHeightFraction,0.0f));//Y=400 planeClouds
	SkyDistant_Shader->LoadSunPosition_Uniform(SunPos);
	SkyDistant_Shader->LoadNearFar_Uniform(vmath::vec2((GLfloat)MyWin32::gNearFarFrustum[0], (GLfloat)MyWin32::gNearFarFrustum[1]));
	double sunAngularSize = AtmoScatter->model().SunAngularRadius;
	SkyDistant_Shader->LoadSunSize_Uniform(vmath::vec2(float(tan(sunAngularSize)), float(cos(sunAngularSize))));
	SkyDistant_Shader->LoadExposure_Uniform(MyOpenGL::fExposure);

	AtmoScatter->model().SetProgramUniforms(SkyDistant_Shader->GetShaderProgram(), 1, 2, 0, 0);

	glDrawArrays(GL_QUADS, 0, 4);

	SkyDistant_Shader->Stop();
}

void ZzRenderer::Render_SkyFroxel(void)
{

	const GLsizei FroxelTextureSize = 64;
	const GLsizei WorkGroupSize = 8;

	const GLuint FinalComputeSize = INT_CEIL(FroxelTextureSize, WorkGroupSize);

	SkyFroxel_Shader->Start();

	SkyFroxel_Shader->LoadInvProjX_Uniform(MyWin32::gInvProjectionMatrix);
	SkyFroxel_Shader->LoadInvViewX_Uniform(MyWin32::gInvViewMatrix);
	SkyFroxel_Shader->LoadEarthCenter_Uniform(vmath::vec3(0.0f,(GLfloat)-AtmoScatter->model().EarthRadius,0.0f));
	SkyFroxel_Shader->LoadEyePosition_Uniform(Cam.GetCameraPos());
	SkyFroxel_Shader->LoadSunPosition_Uniform(SunPos);
	SkyFroxel_Shader->LoadNearFar_Uniform(vmath::vec2((GLfloat)MyWin32::gNearFarFrustum[0], (GLfloat)MyWin32::gNearFarFrustum[1]));

	AtmoScatter->model().SetProgramUniforms(SkyFroxel_Shader->GetShaderProgram(), 1, 2, 0, 0);

	glBindImageTexture(0, noise.GetTexureID_FroxelTransmittance(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
	glBindImageTexture(1, noise.GetTexureID_FroxelInScatter(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);

	glDispatchCompute(FinalComputeSize, FinalComputeSize, FinalComputeSize);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);


	//glBindTexture(GL_TEXTURE_3D, noise.GetTexureID_FroxelTransmittance());
	//glGenerateTextureMipmap(noise.GetTexureID_FroxelTransmittance());
	//glBindTexture(GL_TEXTURE_3D, 0);

	//glBindTexture(GL_TEXTURE_3D, noise.GetTexureID_FroxelInScatter());
	//glGenerateTextureMipmap(noise.GetTexureID_FroxelInScatter());
	//glBindTexture(GL_TEXTURE_3D, 0);

	SkyFroxel_Shader->Stop();
}

//CullIndirect
/*
void ZzRenderer::Render_ModelsPBR(void)
{

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------
	//TO ADD TREES (RAY CAST)
	////SpeedTree
	//if (TRUE == MyWin32::addObject)
	//{
	//	FILE* fp = NULL;
	//	fopen_s(&fp, "treeData.zz", "a");

	////	RayCaster::fRotAngleY =( rand() / float(RAND_MAX)) * 360.0f;//not working as intented(solve later)
	////	RayCaster::fUniformScale =(( rand() / float(RAND_MAX)) * 0.0340f)+0.04f;//not working as intented(solve later)


	//	if (fp)
	//	{
	//		fprintf(fp, "Z %f %f %f %f %f %f %f\n", RayCaster::WorldPos[0], RayCaster::WorldPos[1], RayCaster::WorldPos[2], RayCaster::fRotAngleX, RayCaster::fRotAngleY, RayCaster::fRotAngleZ, RayCaster::fUniformScale);
	//		fclose(fp);
	//	}

	//	PerInstanceData temp;
	//	temp.WorldPos = RayCaster::WorldPos;
	//	temp.fRotAngleX = RayCaster::fRotAngleX;
	//	temp.fRotAngleY = RayCaster::fRotAngleY;
	//	temp.fRotAngleZ = RayCaster::fRotAngleZ;
	//	temp.fUniformScale = RayCaster::fUniformScale;

	//	cullIndirect.AddPerInstanceMatrix(temp);

	//	MyWin32::addObject = FALSE;
	//}


	//CullIndirect
	// Bind and clear atomic counter
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, cullIndirect.GetBuffers().parameters);
	glClearBufferSubData(GL_ATOMIC_COUNTER_BUFFER, GL_R32UI, 0, sizeof(GLuint), GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);

	// Bind shader storage buffers
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, cullIndirect.GetBuffers().drawCandidates);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, cullIndirect.GetBuffers().drawCommands);

	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	CullIndirectShader.Start();
	CullIndirectShader.LoadProjXUniform(MyWin32::gViewProjectionMatrixForDepth);

	glDispatchCompute(INT_CEIL(cullIndirect.GetCandidateCount(), 16) , 1, 1);

	glMemoryBarrier(GL_COMMAND_BARRIER_BIT);

	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, cullIndirect.GetBuffers().drawCommands);
	glBindBuffer(GL_PARAMETER_BUFFER_ARB, cullIndirect.GetBuffers().parameters);

	CullIndirectShader.Stop();


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);

	//to Place trees
	//TEST   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//passThroughShader.Start();

	//vmath::mat4 modelMatrix = vmath::mat4::identity();
	//modelMatrix = vmath::translate(RayCaster::WorldPos) * vmath::rotate(RayCaster::fRotAngleZ, 0.0f, 0.0f, 1.0f) * vmath::rotate(RayCaster::fRotAngleY, 0.0f, 1.0f, 0.0f) * vmath::rotate(RayCaster::fRotAngleX, 1.0f, 0.0f, 0.0f) * vmath::scale(RayCaster::fUniformScale);

	////matrices
	//passThroughShader.LoadModelViewProjXUniform(MyWin32::gViewProjectionMatrixForDepth * modelMatrix);
	//palm->Render();
	//passThroughShader.Stop();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	PBR_SpeedTreeShader.Start();

	//matrices
	PBR_SpeedTreeShader.LoadModelViewProjXUniform(MyWin32::gViewProjectionMatrixForDepth );
//	PBR_SpeedTreeShader.LoadModelXUniform(modelMatrix);
	//vectors
	PBR_SpeedTreeShader.LoadSunPositionUniform(SunPos);
	PBR_SpeedTreeShader.LoadEyePositionUniform(Cam.GetCameraPos());
	//samplers
	PBR_SpeedTreeShader.LoadAlbedoTextureUniform(GL_TEXTURE0, 0, palm->getAlbedoMapTextureID());
	PBR_SpeedTreeShader.LoadNormalTextureUniform(GL_TEXTURE1, 1, palm->getNormalMapTextureID());
	PBR_SpeedTreeShader.LoadSubsurfaceTextureUniform(GL_TEXTURE2, 2, palm->getSubsurfaceMapTextureID());
	PBR_SpeedTreeShader.LoadBlueNoiseTextureUniform(GL_TEXTURE3, 3,noise.GetTextureID_BlueNoise());

	GLdouble SunTheta = vmath::dot(vmath::normalize(this->SunPos), vec3(0.0f, 1.0f, 0.0f));
	PBR_SpeedTreeShader.Loadv3LightColorUniform(palm->getLightColor(SunTheta)*(palm->getLightColor2(SunTheta)));//(*terrain.Getv3LightColor() /////*palm->getLightColor2(SunTheta)

	//palm->Render();
	glBindVertexArray(palm->GetVao());
	glMultiDrawArraysIndirectCount(GL_TRIANGLES, 0, 0,GLsizei(cullIndirect.GetCandidateCount()), 0);

	//triangle.Render();

	PBR_SpeedTreeShader.Stop();

	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}
*/
void ZzRenderer::Render_GaneshaPBR(void)
{
	//GANESHA
	PBR_Shader.Start();

	vmath::mat4 modelMatrix = vmath::mat4::identity();
	modelMatrix = vmath::translate(vmath::vec3(0.0f,0.0f,0.0f)) * vmath::rotate(RayCaster::fRotAngleZ, 0.0f, 0.0f, 1.0f) * vmath::rotate(RayCaster::fRotAngleY, 0.0f, 1.0f, 0.0f) * vmath::rotate(RayCaster::fRotAngleX, 1.0f, 0.0f, 0.0f) * vmath::scale( 270.0f);


	//matrices
	PBR_Shader.LoadViewProjXUniform(MyWin32::gViewProjectionMatrix * modelMatrix);
	PBR_Shader.LoadModelXUniform(modelMatrix);

	//floats
	PBR_Shader.LoadTimeUniform(MyOpenGL::fTime);
	//vectors
	PBR_Shader.LoadSunPositionUniform(SunPos);
	PBR_Shader.LoadEyePositionUniform(Cam.GetCameraPos());
	PBR_Shader.LoadTintColorUniform(vmath::vec3(1.000f, 0.914f, 0.711f));
	//samplers
	PBR_Shader.LoadAlbedoTextureUniform(GL_TEXTURE0, 0, ganesha->getAlbedoMapTextureID());
	PBR_Shader.LoadNormalTextureUniform(GL_TEXTURE1, 1, ganesha->getNormalMapTextureID());
	PBR_Shader.LoadRoughnessTextureUniform(GL_TEXTURE2, 2, ganesha->getRoughnessMapTextureID());
	PBR_Shader.LoadAmbientOcclusionTextureUniform(GL_TEXTURE3, 3, ganesha->getAmbientOcclusionMapTextureID());
	ganesha->Render();

	PBR_Shader.Stop();
}

void ZzRenderer::Render_PBR_RockAsset(void)
{
	//GANESHA
	PBR_Shader.Start();
	
	vmath::mat4 modelMatrix = vmath::mat4::identity();
	modelMatrix = vmath::translate(vmath::vec3(-2.47264981f, 16.8710556f, 75.5039673f)) * vmath::rotate(0.0f, 0.0f, 0.0f, 1.0f) * vmath::rotate(155.844f, 0.0f, 1.0f, 0.0f) * vmath::rotate(0.0f, 1.0f, 0.0f, 0.0f) * vmath::scale(0.221f);


	//matrices
	PBR_Shader.LoadViewProjXUniform(MyWin32::gViewProjectionMatrix );
	PBR_Shader.LoadModelXUniform(modelMatrix);

	//floats
	PBR_Shader.LoadTimeUniform(MyOpenGL::fTime);
	//vectors
	PBR_Shader.LoadSunPositionUniform(SunPos);
	PBR_Shader.LoadEyePositionUniform(Cam.GetCameraPos());
	PBR_Shader.LoadTintColorUniform(vmath::vec3(1.000f, 0.914f, 0.711f));
	//samplers
	PBR_Shader.LoadAlbedoTextureUniform(GL_TEXTURE0, 0, MossyRocks->getAlbedoMapTextureID());
	PBR_Shader.LoadNormalTextureUniform(GL_TEXTURE1, 1, MossyRocks->getNormalMapTextureID());
	PBR_Shader.LoadRoughnessTextureUniform(GL_TEXTURE2, 2, MossyRocks->getRoughnessMapTextureID());
	MossyRocks->Render();
	PBR_Shader.Stop();
}

void ZzRenderer::Render_FinalCloudsGround(GLfloat VolumetricCoverage,GLfloat HighAltitudeCoverage,GLfloat timeMultiplier ,GLuint WeatherMap)
{
	vmath::mat4 ViewMatrix = Cam.GetViewMatrix();
	vmath::vec3 CameraPos = Cam.GetCameraPos();//Cam.GetInitialCameraPosition() for infinite cloud distance

	//RenderClouds
	{
		//update cloud texture quarter res
		clouds->RenderComputeCloudsGround(noise.GetTextureID_PerlinWorley3D(),
			noise.GetTextureID_Worley32(),
			WeatherMap,
			noise.GetTextureID_BlueNoise(),
			ViewMatrix,
			CameraPos,
			SunPos, VolumetricCoverage, HighAltitudeCoverage, timeMultiplier,(GLfloat)AtmoScatter->model().EarthRadius);

		//clouds->RenderTemporalUpscaleQuad( ViewMatrix, CameraPos);
	}
}

void ZzRenderer::Render_FinalClouds(GLfloat VolumetricCoverage,GLfloat HighAltitudeCoverage,GLfloat timeMultiplier ,GLuint WeatherMap)
{
	vmath::mat4 ViewMatrix = Cam.GetViewMatrix();
	vmath::vec3 CameraPos = Cam.GetCameraPos();//Cam.GetInitialCameraPosition() for infinite cloud distance

	//RenderClouds
	{
		//update cloud texture quarter res
		clouds->RenderComputeClouds(noise.GetTextureID_PerlinWorley3D(),
			noise.GetTextureID_Worley32(),
			WeatherMap,
			noise.GetTextureID_BlueNoise(),
			ViewMatrix,
			CameraPos,
			SunPos, VolumetricCoverage, HighAltitudeCoverage, timeMultiplier,(GLfloat)AtmoScatter->model().EarthRadius);

	//	clouds->RenderTemporalUpscaleQuad( ViewMatrix, CameraPos);//realtime
	}
}

void ZzRenderer::Render_FinalCloudsSpace(GLfloat VolumetricCoverage, GLfloat HighAltitudeCoverage, GLfloat timeMultiplier, GLuint WeatherMap)
{
	vmath::mat4 ViewMatrix = Cam.GetViewMatrix();
	vmath::vec3 CameraPos = Cam.GetCameraPos();//Cam.GetInitialCameraPosition() for infinite cloud distance

	//RenderClouds
	{
		//update cloud texture quarter res
		clouds->RenderComputeCloudsSpace(noise.GetTextureID_PerlinWorley3D(),
			noise.GetTextureID_Worley32(),
			WeatherMap,
			noise.GetTextureID_BlueNoise(),
			ViewMatrix,
			CameraPos,
			SunPos, VolumetricCoverage, HighAltitudeCoverage, timeMultiplier, (GLfloat)AtmoScatter->model().EarthRadius);

		//clouds->RenderTemporalUpscaleQuad(ViewMatrix, CameraPos);
	}
}

//void ZzRenderer::Render_GodRays(void)
//{
//	radialBlur.UpdateLightAreaMask(fboScene->GetFbo_Color_Texture(1),clouds->GetCloudTemporalUpscaledTexture());
//	radialBlur.RenderComputeGodRay(radialBlur.GetTextureID_LightAreaMask(),noise.GetTextureID_BlueNoise(),this->ScreenSunPos);//LightArea
//}

//void ZzRenderer::Render_ParticleSystem(void)
//{
//
//	ParticleSystemShader.Start();
//
//	ParticleSystemShader.LoadModelViewProjXUniform(MyWin32::gViewProjectionMatrixForDepth);
//	ParticleSystemShader.LoadAlbedoTextureUniform(GL_TEXTURE0, 0, particleSystem.getTextureID_WindParticle());
//	ParticleSystemShader.LoadfTime_Uniform(MyOpenGL::fTime *0.0f);
//	particleSystem.Render();
//
//	ParticleSystemShader.Stop();
//}

//void ZzRenderer::Render_SmokeParticleSystem(void)
//{
//
//	SmokeParticleSystemShader.Start();
//
//	SmokeParticleSystemShader.LoadModelViewProjXUniform(MyWin32::gViewProjectionMatrixForDepth);
//	SmokeParticleSystemShader.LoadAlbedoTextureUniform(GL_TEXTURE0, 0, particleSystem.getTextureID_WindParticle());
//	SmokeParticleSystemShader.LoadfTime_Uniform(MyOpenGL::fTime*4);
//	smokeParticleSystem.Render();
//
//	SmokeParticleSystemShader.Stop();
//}

//void ZzRenderer::Render_RainParticleSystem(void)
//{
//
//	RainParticleSystemShader.Start();
//
//	RainParticleSystemShader.LoadModelViewProjXUniform(MyWin32::gViewProjectionMatrixForDepth);
//	RainParticleSystemShader.LoadAlbedoTextureUniform(GL_TEXTURE0, 0, rainParticleSystem.getTextureID_RainParticle());
//	RainParticleSystemShader.LoadfTime_Uniform(MyOpenGL::fTime * 0.0f);
//	rainParticleSystem.Render();
//
//	RainParticleSystemShader.Stop();
//}


void ZzRenderer::Render_PaleBlueDotCreditSceneOffline(HWND hwnd)
{
	static GLfloat animFactor = 0.0f;
	GLfloat FinalAnimFactor = ((animFactor > 1.0f) ? 1.0f : animFactor);

	MyWin32::gNearFarFrustum = vmath::dvec2(0.1, 20000.0);
	MyWin32::fovY = 60.0f;
	MyOpenGL::fExposure = 10.0f;
	ZzRenderer::sunAngle = 110.0f;

	GLfloat mixFactor1 = vmath::clamp(2.0f * vmath::vec1(FinalAnimFactor - 0.0f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];
	GLfloat mixFactor2 = vmath::clamp(2.0f * vmath::vec1(FinalAnimFactor - 0.5f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];


	vmath::vec3 CamPos1 = vmath::vec3(614.017822f, -2.17490196f, 93.9992981f);
	vmath::vec3 CamPos2 = vmath::vec3(329.819336f, 7.30674934f, -135.583572f);
	vmath::vec3 CamPos3 = vmath::vec3(-1.75024438f, 96.1140137f, -119.224724f);
	GLfloat Pitch1 = -4.50048351f;
	GLfloat Pitch2 = -7.79762363f;
	GLfloat Pitch3 = -39.5159225f;
	GLfloat Yaw1 = 195.710449f;
	GLfloat Yaw2 = 174.557373f;
	GLfloat Yaw3 = 89.2691879f;

	vmath::vec3 FinalCamPos = vmath::mix(vmath::mix(CamPos1, CamPos2, mixFactor1), CamPos3, mixFactor2);
	GLfloat FinalPitch = vmath::mix(vmath::mix(Pitch1, Pitch2, mixFactor1), Pitch3, smoothstep(vec1(0.0f), vec1(1.0f), vec1(mixFactor2))[0]);
	GLfloat FinalYaw = vmath::mix(vmath::mix(Yaw1, Yaw2, mixFactor1), Yaw3, smoothstep(vec1(0.0f), vec1(1.0f), vec1(mixFactor2))[0]);

	Cam.SetCameraPos(FinalCamPos);
	Cam.SetPitch(FinalPitch);
	Cam.SetYaw(FinalYaw);

	this->Update(hwnd);

	if (FinalAnimFactor >= 1.0f)
	{
		RayCaster::fUniformScale += (0.25f / 60.0f);
	}

//	RayCaster::fUniformScale = FinalAnimFactor;
	//static bool initializeCamPos = false;
	//if (initializeCamPos==false)
	//{
	//	Cam.SetCameraPos(vmath::vec3(0.0f, 70.0f, 0.0f));
	//	initializeCamPos = true;
	//}


	//All gBuffer Objects
	DefferedRenderer.BindDefferedFbo();
	{
		this->Render_TerrainBasic3D(GL_FALSE);
		//rayCast
		//if (FALSE == MyWin32::isGUI)
		//{
		//	myRayCaster.getWorldSpacePosFromScreenSpace();
		//}
		
		//this->Render_PBR_RockAsset();

		//this->Render_DefferedSpeedTreeInstanced(GL_FALSE);
		//this->Render_Axis3D();
	}

	//this->Render_SkyFroxel();
	this->Render_SkyDistant();

	if (MyWin32::iFrameID == 1)
	{
		clouds->RenderComputeCloudShadowMap((GLfloat)AtmoScatter->model().EarthRadius, Cam.GetLightViewMatrix(), this->SunWorldPos, noise.GetTextureID_PerlinWorley3D());
	}

	this->Render_FinalCloudsGround(1.0f,0.0f,  FinalAnimFactor*3.0f,clouds->GetCloudWeatherMapTileableTexture());

	//sky
	this->Render_BasicSkyPBR();
	//temporalAA.CalculateMaxVelocity(temporalAA.GetCameraVelocityTexture());

	//if (MyWin32::TAA)
	//{
	//	this->BindFboTemporalAA();
	//	temporalAA.TemporalAntiAlias(Cam.GetViewMatrix(), fboScene->GetFbo_Color_Texture(0), DefferedRenderer.GetTextureID_DepthBuffer(), noise.GetCurrentHalton23(), noise.GetPreviousHalton23());//remove *0.0f
	//	temporalAA.ResolveToPreviousFrame(fboTemporalAA->GetFbo_Color_Texture(0), temporalAA.GetAntiAliasedTexture(), vmath::uvec2(MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight));

	//	this->Render_FinalQuad(temporalAA.GetAntiAliasedTexture());
	//	//	this->Render_FinalQuadBartWronskiDownsample(temporalAA.GetAntiAliasedTexture());
	//	//	this->Render_Compositor(temporalAA.GetAntiAliasedTexture(), fboScene->GetFbo_Color_Texture(1), fboPostProcess->GetFbo_Color_Texture(0));
	//}
	//else
	//{
	//	//TAAU 
	//	this->BindFboTemporalUpscale4K();
	//	temporalAA.TemporalUpscale(Cam.GetViewMatrix(), fboScene->GetFbo_Color_Texture(0), 0, MyWin32::iBlockPixelID);
	//	temporalAA.ResolveToPreviousFrame(fboTemporalUpscale4k->GetFbo_Color_Texture(0), temporalAA.GetTemporalUpscaled4kTexture(), fboTemporalUpscale4k->GetFboResolution());

	//	//	this->Render_FinalQuadBartWronskiDownsample(temporalAA.GetTemporalUpscaled4kTexture());
	//	//	this->Render_FinalQuad(temporalAA.GetTemporalUpscaled4kTempTexture());
	//	this->Render_Compositor(temporalAA.GetTemporalUpscaled4kTexture(),fboSkyDistant->GetFbo_Color_Texture(0) , fboPostProcess->GetFbo_Color_Texture(0));//clouds->GetCloudTemporalUpscaledTexture()
	//}

	this->Render_Compositor(fboSceneOffline->GetFbo_Color_Texture(0), fboSkyDistant->GetFbo_Color_Texture(0),0);//clouds->GetCloudTemporalUpscaledTexture()

	//this->Render_FinalQuad(fboSceneOffline->GetFbo_Color_Texture(0));

	GLfloat timeInSeconds = 13.0f;
	animFactor += (1.0f / 60.0f) * (1.0f / timeInSeconds);

	vmath::vec3 CamPos = Cam.GetCameraPos();
	GLfloat Pitch = Cam.GetPitch();
	GLfloat Yaw = Cam.GetYaw();
	vmath::vec3 WorldPos = RayCaster::WorldPos;
}


void ZzRenderer::Render_PaleBlueDotSceneLifeOffline(HWND hwnd)
{
	static GLfloat animFactor = 0.0f;
	GLfloat FinalAnimFactor = ((animFactor > 1.0f) ? 1.0f : animFactor);

	MyWin32::gNearFarFrustum = vmath::dvec2(0.01, 200.0);
	MyWin32::fovY = 60.0f;
	MyOpenGL::fExposure = 10.0f;
	ZzRenderer::sunAngle = 175.727f;
	RayCaster::fUniformScale = FinalAnimFactor;
	//static bool initializeCamPos = false;
	//if (initializeCamPos==false)
	//{
	//	Cam.SetCameraPos(vmath::vec3(0.0f, 70.0f, 0.0f));
	//	initializeCamPos = true;
	//}
	
	Cam.SetCameraPos(vmath::mix(vmath::vec3(-2.45339465f, 18.4665794f, 78.2292099f), vmath::vec3(-2.36733460f, 18.1630287f, 74.5054092f), FinalAnimFactor*0.74f));
	Cam.SetPitch(vmath::mix(-9.08924675f, -0.302169174f, FinalAnimFactor* 0.74f));
    Cam.SetYaw(vmath::mix(-87.3462601f, -89.5661316f, FinalAnimFactor* 0.74f));

//	Cam.SetCameraPos(vmath::mix(vmath::vec3(-2.45339465f, 18.4665794f, 78.2292099f), vmath::vec3(-2.07185864f, 20.3095627f, 57.4183502f), FinalAnimFactor));
//	Cam.SetPitch(vmath::mix(-9.08924675f, -14.7481966f, FinalAnimFactor));
//  Cam.SetYaw(vmath::mix(-87.3462601f, -97.9823837f, FinalAnimFactor));

	//All gBuffer Objects
	DefferedRenderer.BindDefferedFbo();
	{
		this->Render_TerrainBasic3D(GL_FALSE);
		//rayCast
		if (FALSE == MyWin32::isGUI)
		{
			myRayCaster.getWorldSpacePosFromScreenSpace();
		}
		
		this->Render_PBR_RockAsset();

		this->Render_DefferedSpeedTreeInstanced(GL_FALSE);
		//this->Render_Axis3D();
	}

	this->Render_SkyFroxel();
	this->Render_SkyDistant();
	this->Render_FinalCloudsGround(1.0f,0.0f,  FinalAnimFactor*3.0f,clouds->GetCloudWeatherMapTileableTexture());

	//sky
	this->Render_BasicSkyPBR();
	//temporalAA.CalculateMaxVelocity(temporalAA.GetCameraVelocityTexture());

	//if (MyWin32::TAA)
	//{
	//	this->BindFboTemporalAA();
	//	temporalAA.TemporalAntiAlias(Cam.GetViewMatrix(), fboScene->GetFbo_Color_Texture(0), DefferedRenderer.GetTextureID_DepthBuffer(), noise.GetCurrentHalton23(), noise.GetPreviousHalton23());//remove *0.0f
	//	temporalAA.ResolveToPreviousFrame(fboTemporalAA->GetFbo_Color_Texture(0), temporalAA.GetAntiAliasedTexture(), vmath::uvec2(MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight));

	//	this->Render_FinalQuad(temporalAA.GetAntiAliasedTexture());
	//	//	this->Render_FinalQuadBartWronskiDownsample(temporalAA.GetAntiAliasedTexture());
	//	//	this->Render_Compositor(temporalAA.GetAntiAliasedTexture(), fboScene->GetFbo_Color_Texture(1), fboPostProcess->GetFbo_Color_Texture(0));
	//}
	//else
	//{
	//	//TAAU 
	//	this->BindFboTemporalUpscale4K();
	//	temporalAA.TemporalUpscale(Cam.GetViewMatrix(), fboScene->GetFbo_Color_Texture(0), 0, MyWin32::iBlockPixelID);
	//	temporalAA.ResolveToPreviousFrame(fboTemporalUpscale4k->GetFbo_Color_Texture(0), temporalAA.GetTemporalUpscaled4kTexture(), fboTemporalUpscale4k->GetFboResolution());

	//	//	this->Render_FinalQuadBartWronskiDownsample(temporalAA.GetTemporalUpscaled4kTexture());
	//	//	this->Render_FinalQuad(temporalAA.GetTemporalUpscaled4kTempTexture());
	//	this->Render_Compositor(temporalAA.GetTemporalUpscaled4kTexture(),fboSkyDistant->GetFbo_Color_Texture(0) , fboPostProcess->GetFbo_Color_Texture(0));//clouds->GetCloudTemporalUpscaledTexture()
	//}

	this->Render_Compositor(fboSceneOffline->GetFbo_Color_Texture(0), fboSkyDistant->GetFbo_Color_Texture(0),0);//clouds->GetCloudTemporalUpscaledTexture()

	//this->Render_FinalQuad(fboSceneOffline->GetFbo_Color_Texture(0));

	GLfloat timeInSeconds = 24.0f;
	animFactor += (1.0f / 60.0f) * (1.0f / timeInSeconds);

	vmath::vec3 CamPos = Cam.GetCameraPos();
	GLfloat Pitch = Cam.GetPitch();
	GLfloat Yaw = Cam.GetYaw();
	vmath::vec3 WorldPos = RayCaster::WorldPos;
}


void ZzRenderer::Render_PaleBlueDotSceneAfterLifeOffline(HWND hwnd)
{
	static GLfloat animFactor = 0.0f;
	GLfloat FinalAnimFactor = ((animFactor > 1.0f) ? 1.0f : animFactor);
	//GLfloat FinalAnimFactor =1.0f;

	MyWin32::gNearFarFrustum = vmath::dvec2(0.01, 200.0);
	MyWin32::fovY = 70.0f;
	MyOpenGL::fExposure = 10.0f;
	ZzRenderer::sunAngle = 175.727f;

	//static bool initializeCamPos = false;
	//if (initializeCamPos==false)
	//{
	//	Cam.SetCameraPos(vmath::vec3(12701.9336f, 25072.6973f, 114503.141f));
	//	Cam.SetPitch(-36.4476624f);
	//	Cam.SetYaw(-95.3401489f);
	//	initializeCamPos = true;
	//}
	

	//GLfloat mixFactor1 = vmath::clamp(5.0f * vmath::vec1(FinalAnimFactor - 0.0f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];
	//GLfloat mixFactor2 = vmath::clamp(5.0f * vmath::vec1(FinalAnimFactor - 0.2f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];
	//GLfloat mixFactor3 = vmath::clamp(5.0f * vmath::vec1(FinalAnimFactor - 0.4f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];
	//GLfloat mixFactor4 = vmath::clamp(5.0f * vmath::vec1(FinalAnimFactor - 0.6f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];
	//GLfloat mixFactor5 = vmath::clamp(5.0f * vmath::vec1(FinalAnimFactor - 0.8f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];

	GLfloat mixFactor1 = vmath::clamp(5.0f * vmath::vec1(FinalAnimFactor - 0.0f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];
	GLfloat mixFactor2 = vmath::clamp(5.0f * vmath::vec1(FinalAnimFactor - 0.2f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];
	GLfloat mixFactor3 = vmath::clamp(5.0f * vmath::vec1(FinalAnimFactor - 0.4f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];
	GLfloat mixFactor4 = vmath::clamp(5.0f * vmath::vec1(FinalAnimFactor - 0.6f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];
	GLfloat mixFactor5 = vmath::clamp(5.0f * vmath::vec1(FinalAnimFactor - 0.8f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];

	GLfloat mixFactorExtra = (mixFactor1 + mixFactor2 + mixFactor3) / 3.0f;
//	mixFactor1 = 1.0f;
//	mixFactor2 = 1.0f;

	//Camera
	{
		vmath::vec3 CamPos1 = vmath::vec3(-8.11575603f, 16.7454128f + 10.44f * mixFactor1, 57.4067535f);
		vmath::vec3 CamPos2 = vmath::vec3(-318.717102f, 954.923279f, -608.455933f);
		vmath::vec3 CamPos3 = vmath::vec3(-1578.51331f, 1272.84937f, 10.5811958f);
		vmath::vec3 CamPos4 = vmath::vec3(12701.9336f, 25072.6973f, 114503.141f);
		vmath::vec3 CamPos5 = vmath::vec3(144873.281f, 105929.328f, 196235.891f);
		vmath::vec3 CamPosFinal = vmath::mix(vmath::mix(vmath::mix(CamPos2, CamPos3, mixFactorExtra), CamPos4, mixFactor4), CamPos5, mixFactor5);
		//vmath::vec3 CamPosFinal = vmath::mix(vmath::mix(vmath::mix(vmath::mix(CamPos1, CamPos2, mixFactor2), CamPos3, mixFactor3), CamPos4, mixFactor4),CamPos5, mixFactor5);

		GLfloat Pitch1 = -6.99836111f;
		GLfloat Pitch2 = -7.34846544f;
		GLfloat Pitch3 = -40.9974136f;
		GLfloat Pitch4 = -36.4476624f;
		GLfloat Pitch5 = -34.0558395f;
		GLfloat PitchFinal = vmath::mix(vmath::mix(vmath::mix(Pitch2, Pitch3, mixFactorExtra), Pitch4, mixFactor4), Pitch5, mixFactor5);
	//	GLfloat PitchFinal = vmath::mix(vmath::mix(vmath::mix(vmath::mix(Pitch1, Pitch2,mixFactor2), Pitch3, mixFactor3),Pitch4, mixFactor4),Pitch5,mixFactor5);

		GLfloat Yaw1 = -90.6313553f;
		GLfloat Yaw2 = -94.1960754f;
		GLfloat Yaw3 = -107.808243f;
		GLfloat Yaw4 = -95.3401489f;
		GLfloat Yaw5 = -127.736794f;
		GLfloat YawFinal = vmath::mix(vmath::mix(vmath::mix(Yaw2, Yaw3, mixFactorExtra), Yaw4, mixFactor4), Yaw5, mixFactor5);
	//	GLfloat YawFinal = vmath::mix(vmath::mix(vmath::mix(vmath::mix(Yaw1, Yaw2, mixFactor2), Yaw3, mixFactor3),Yaw4, mixFactor4),Yaw5,mixFactor5);

		Cam.SetCameraPos(CamPosFinal);
		Cam.SetPitch(PitchFinal);
		Cam.SetYaw(YawFinal);
	}

	earth.UpdateEarthCloudRotationDelusion();

		//All gBuffer Objects
	DefferedRenderer.BindDefferedFbo();
	{
		//this->Render_TerrainBasic3D(GL_FALSE);
		//rayCast;
		//if (FALSE == MyWin32::isGUI)
		//{
		//	myRayCaster.getWorldSpacePosFromScreenSpace();
		//}

	//	this->Render_PBR_RockAsset();

	//	this->Render_DefferedSpeedTreeInstanced(GL_FALSE);
		//this->Render_Axis3D();
	}

//	this->Render_SkyFroxel();
	this->Render_SkyDistant();
//	this->Render_FinalCloudsGround(1.0f, 0.0f, FinalAnimFactor * 3.0f, clouds->GetCloudWeatherMapTileableTexture());
	this->Render_FinalCloudsSpace(1.0f, 0.0f, 1.0f, clouds->GetCloudWeatherMapTileableTexture());

	//sky


	//if (FinalAnimFactor > 0.4)
	{
		this->BindSceneOfflineFbo();
		this->Render_SkyPBR(7.0f, vmath::vec3(0.6f, 1.0f, 1.0f), vmath::vec3(0.253f, 1.000f, 0.152f), 0.0f, vmath::vec2(1.0));
	}
	//else
	//{
	//	this->Render_BasicSkyPBR();
	//}



	//temporalAA.CalculateMaxVelocity(temporalAA.GetCameraVelocityTexture());

	//if (MyWin32::TAA)
	//{
	//	this->BindFboTemporalAA();
	//	temporalAA.TemporalAntiAlias(Cam.GetViewMatrix(), fboScene->GetFbo_Color_Texture(0), DefferedRenderer.GetTextureID_DepthBuffer(), noise.GetCurrentHalton23(), noise.GetPreviousHalton23());//remove *0.0f
	//	temporalAA.ResolveToPreviousFrame(fboTemporalAA->GetFbo_Color_Texture(0), temporalAA.GetAntiAliasedTexture(), vmath::uvec2(MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight));

	//	this->Render_FinalQuad(temporalAA.GetAntiAliasedTexture());
	//	//	this->Render_FinalQuadBartWronskiDownsample(temporalAA.GetAntiAliasedTexture());
	//	//	this->Render_Compositor(temporalAA.GetAntiAliasedTexture(), fboScene->GetFbo_Color_Texture(1), fboPostProcess->GetFbo_Color_Texture(0));
	//}
	//else
	//{
	//	//TAAU 
	//	this->BindFboTemporalUpscale4K();
	//	temporalAA.TemporalUpscale(Cam.GetViewMatrix(), fboScene->GetFbo_Color_Texture(0), 0, MyWin32::iBlockPixelID);
	//	temporalAA.ResolveToPreviousFrame(fboTemporalUpscale4k->GetFbo_Color_Texture(0), temporalAA.GetTemporalUpscaled4kTexture(), fboTemporalUpscale4k->GetFboResolution());

	//	//	this->Render_FinalQuadBartWronskiDownsample(temporalAA.GetTemporalUpscaled4kTexture());
	//	//	this->Render_FinalQuad(temporalAA.GetTemporalUpscaled4kTempTexture());
	//	this->Render_Compositor(temporalAA.GetTemporalUpscaled4kTexture(),fboSkyDistant->GetFbo_Color_Texture(0) , fboPostProcess->GetFbo_Color_Texture(0));//clouds->GetCloudTemporalUpscaledTexture()
	//}

	this->Render_CompositorSpaceOffline(fboSceneOffline->GetFbo_Color_Texture(0),fboSkyDistant->GetFbo_Color_Texture(0), 0,GL_TRUE, mixFactor4);//clouds->GetCloudTemporalUpscaledTexture()
	//this->Render_Compositor(fboSceneOffline->GetFbo_Color_Texture(0), fboSkyDistant->GetFbo_Color_Texture(0), 0);//clouds->GetCloudTemporalUpscaledTexture()

	//this->Render_FinalQuad(fboSceneOffline->GetFbo_Color_Texture(0));

	GLfloat timeInSeconds = 23.3f;
	animFactor += (1.0f / 60.0f) * (1.0f / timeInSeconds);

	vmath::vec3 CamPos = Cam.GetCameraPos();
	GLfloat Pitch = Cam.GetPitch();
	GLfloat Yaw = Cam.GetYaw();
	vmath::vec3 WorldPos = RayCaster::WorldPos;
}


void ZzRenderer::Render_PaleBlueDotSceneFinalSceneOffline(HWND hwnd)
{
	static GLfloat animFactor = 0.0f;
	static GLfloat animFactorSlow = 0.0f;
	static GLfloat animFactorFinalScene = 0.0f;

	static GLuint FramesDelay = 0;

	GLfloat FinalAnimFactor = ((animFactor > 1.0f) ? 1.0f : animFactor);
	GLfloat FinalAnimFactorSlow = ((animFactorSlow > 1.0f) ? 1.0f : animFactorSlow);
	GLfloat FinalAnimFactorFinalScene = ((animFactorFinalScene > 1.0f) ? 1.0f : animFactorFinalScene);

	GLfloat LastFactor = 1.0f-abs((FinalAnimFactorFinalScene - 0.5f) * 2.0f);

	//GLfloat FinalAnimFactor =0.65+0.12f*RayCaster::fUniformScale;

	GLfloat mixFactor1 = vmath::clamp(2.0f * vmath::vec1(FinalAnimFactor - 0.0f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];
	GLfloat mixFactor2 = vmath::clamp(2.0f * vmath::vec1(FinalAnimFactor - 0.5f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];
	//	GLfloat mixFactor3 = vmath::clamp(4.0f * vmath::vec1(FinalAnimFactor - 0.5f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];
	//	GLfloat mixFactor4 = vmath::clamp(4.0f * vmath::vec1(FinalAnimFactor - 0.75f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];
	GLfloat mixFactor2Slow = vmath::clamp(2.0f * vmath::vec1(FinalAnimFactorSlow - 0.5f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];

	//GLfloat mixFactor5 = vmath::clamp(5.0f * vmath::vec1(FinalAnimFactor - 0.8f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];

	if (mixFactor2 > 0.9999f)
	{
		FramesDelay += 1;
	}
	if (FramesDelay>100)
	{
		animFactorFinalScene += (1.2f / 60.0f);
	}
	
	RayCaster::fUniformScale = pow(mixFactor2Slow * 1.7f,1.7f);
	RayCaster::fRotAngleZ = vmath::mix(60.0f, 180.0f, vmath::clamp(FinalAnimFactorFinalScene*2.0f,0.0f,1.0f));

	MyWin32::gNearFarFrustum = vmath::dvec2(0.01, 200.0);
	MyWin32::fovY = 70.0f;
	MyOpenGL::fExposure = vmath::mix(10.0f, 3.0f, mixFactor1);
	ZzRenderer::sunAngle = vmath::mix(175.727f, 44.0f,vmath::clamp( mixFactor1*1.5f,0.0f,1.0f));

	//static bool initializeCamPos = false;
	//if (initializeCamPos==false)
	//{
	//	Cam.SetCameraPos(vmath::vec3(16645543.0f, 38914524.0f, 48053960.0f));
	//	Cam.SetPitch(-46.0777931f);
	//	Cam.SetYaw(-90.9728699f);
	//	initializeCamPos = true;
	//}



//	mixFactor1 = 1.0f;
//	mixFactor2 = 0.0f;

	//Camera
	{

		vmath::vec3 CamPos1 = vmath::vec3(144873.281f, 105929.328f, 196235.891f);
		vmath::vec3 CamPos2 = vmath::vec3(1763993.25f, 4391384.00f, 6111584.00f);
		vmath::vec3 CamPos3 = vmath::vec3(16645543.0f, 38914524.0f, 48053960.0f);
		vmath::vec3 CamPosFinal = vmath::mix(vmath::mix(CamPos1, CamPos2, mixFactor1), CamPos3, mixFactor2Slow * 3.0f);

		GLfloat Pitch1 = -34.0558395f;
		GLfloat Pitch2 = -35.9486694f;
		GLfloat Pitch3 = -46.0777931f;
		GLfloat Pitch4 = -50.7680206f;
		GLfloat Pitch5 = -51.7680206f;
		GLfloat PitchFinal = vmath::mix(vmath::mix(Pitch1, Pitch2, mixFactor1), Pitch3, mixFactor2Slow);
		//PitchFinal = vmath::mix(PitchFinal, Pitch5, mixFactor4Slow);

		GLfloat Yaw1 = -127.736794f;
		GLfloat Yaw2 = -111.642921f;
		GLfloat Yaw3 = -90.9728699f;
		GLfloat Yaw4 = -83.1774826f;
		GLfloat Yaw5 = -81.1774826f;
		GLfloat YawFinal = vmath::mix(vmath::mix(Yaw1, Yaw2, mixFactor1), Yaw3, mixFactor2Slow);
		//YawFinal = vmath::mix(YawFinal, Yaw5, mixFactor4Slow);

		Cam.SetCameraPos(CamPosFinal);
		Cam.SetPitch(PitchFinal);
		Cam.SetYaw(YawFinal);
	}

	earth.UpdateEarthCloudRotationDelusion();

	this->Update(hwnd);

	//All gBuffer Objects
	DefferedRenderer.BindDefferedFbo();
	{
		//	this->Render_TerrainBasic3D();
			//rayCast;
			//if (FALSE == MyWin32::isGUI)
			//{
			//	myRayCaster.getWorldSpacePosFromScreenSpace();
			//}

		//	this->Render_PBR_RockAsset();

		//	this->Render_DefferedSpeedTreeInstanced(GL_FALSE);
			//this->Render_Axis3D();
	}

	if (FramesDelay < 440)
	{
		//this->Render_SkyFroxel();
		//this->Render_SkyDistant();
		//	this->Render_FinalCloudsGround(1.0f, 0.0f, FinalAnimFactor * 3.0f, clouds->GetCloudWeatherMapTileableTexture());
		//this->Render_FinalCloudsSpace(1.0f, 0.0f, 1.0f, clouds->GetCloudWeatherMapTileableTexture());

		//sky
		this->BindSceneOfflineFbo();
		this->Render_SkyPBR(vmath::mix(7.0f, 2.1f, FinalAnimFactor), vmath::vec3(0.6f, 1.0f, 1.0f), vmath::vec3(0.253f, 1.000f, 0.152f), 0.0f, vmath::vec2(1.0));

		//Starfield
		this->BindFbo4kExtra();
		{
			//MyWin32::fovY = 40.0f;
			MyWin32::gNearFarFrustum = vmath::dvec2(10.0, 20000010.0);
			Update(hwnd);
			this->Render_StarField();
		}
		{
			//MyWin32::fovY = 40.0f;
			MyWin32::gNearFarFrustum = vmath::dvec2(10.0, 20000000000.0);
			Update(hwnd);
			GLfloat AnamorphicSize = 3500000.0f + 500000.0f * LastFactor;
			this->Render_SingleBillboard(noise.GetTextureID_SunGlare(), vmath::vec3(0.0f, -(GLfloat)AtmoScatter->model().EarthRadius, 0.0f), vmath::vec3(AnamorphicSize, AnamorphicSize*0.2f, AnamorphicSize), pow(LastFactor, 0.5f) * 2.4f);//
			this->Render_SingleBillboard(noise.GetTextureID_BlueGlow(), vmath::vec3(0.0f, -(GLfloat)AtmoScatter->model().EarthRadius, 0.0f), vmath::vec3(200000.0f), vmath::clamp(mixFactor1 - 0.3f, 0.0f, 1.0f) );//
		}


		ResolveFBO(fbo4kExtra->GetFbo(), fbo4kFinal->GetFbo(), GL_COLOR_ATTACHMENT0);

		//if (mixFactor2 > 0.0f)
		{
			this->Render_CompositorSpaceOffline(fboSceneOffline->GetFbo_Color_Texture(0), fbo4kFinal->GetFbo_Color_Texture(0), 0, GL_TRUE, 0.0f);//clouds->GetCloudTemporalUpscaledTexture()
		}
		//else
		//{
		//	this->Render_CompositorSpaceOffline(fboSceneOffline->GetFbo_Color_Texture(0), fboSkyDistant->GetFbo_Color_Texture(0), 0, GL_TRUE, 0.0f);//clouds->GetCloudTemporalUpscaledTexture()
		//}

		GLfloat timeInSeconds = 11.0f;
		animFactor += (1.0f / 60.0f) * (1.0f / timeInSeconds);

		if (mixFactor2 > 0.0f)
		{
			animFactorSlow += (1.0f / 60.0f) * (1.0f / timeInSeconds) * (1.0f - mixFactor2);
		}
		else
		{
			animFactorSlow += (1.0f / 60.0f) * (1.0f / timeInSeconds);
		}
	}
	else
	{
		this->Render_CompositorSpaceOffline(0, 0, 0, GL_TRUE, 0.0f);//clouds->GetCloudTemporalUpscaledTexture()
	}



	vmath::vec3 CamPos = Cam.GetCameraPos();
	GLfloat Pitch = Cam.GetPitch();
	GLfloat Yaw = Cam.GetYaw();
	vmath::vec3 WorldPos = RayCaster::WorldPos;


}



void ZzRenderer::Render_PaleBlueDotGroundScene()
{
	MyWin32::gNearFarFrustum = vmath::dvec2(0.01, 200.0);
	MyWin32::fovY = 70.0f;

	static bool initializeCamPos = false;

	if (initializeCamPos==false)
	{
		Cam.SetCameraPos(vmath::vec3(0.0f, 70.0f, 0.0f));
		initializeCamPos = true;
	}

	//All gBuffer Objects
	DefferedRenderer.BindDefferedFbo();
	{
		this->Render_TerrainBasic3D(GL_FALSE);
		//rayCast
		if (FALSE == MyWin32::isGUI)
		{
			myRayCaster.getWorldSpacePosFromScreenSpace();
		}

		this->Render_DefferedSpeedTreeInstanced(GL_FALSE);
		//this->Render_Axis3D();
	}

	this->Render_SkyFroxel();
	this->Render_SkyDistant();
	this->Render_FinalCloudsGround(1.0f,0.0f,1.0f,clouds->GetCloudWeatherMapTileableTexture());

	//sky
	this->BindSceneFbo();
	this->Render_BasicSkyPBR();
	temporalAA.CalculateMaxVelocity(temporalAA.GetCameraVelocityTexture());

	if (MyWin32::TAA)
	{
		this->BindFboTemporalAA();
		temporalAA.TemporalAntiAlias(Cam.GetViewMatrix(), fboScene->GetFbo_Color_Texture(0), DefferedRenderer.GetTextureID_DepthBuffer(), noise.GetCurrentHalton23(), noise.GetPreviousHalton23());//remove *0.0f
		temporalAA.ResolveToPreviousFrame(fboTemporalAA->GetFbo_Color_Texture(0), temporalAA.GetAntiAliasedTexture(), vmath::uvec2(MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight));

		this->Render_FinalQuad(temporalAA.GetAntiAliasedTexture());
		//	this->Render_FinalQuadBartWronskiDownsample(temporalAA.GetAntiAliasedTexture());
		//	this->Render_Compositor(temporalAA.GetAntiAliasedTexture(), fboScene->GetFbo_Color_Texture(1), fboPostProcess->GetFbo_Color_Texture(0));
	}
	else
	{
		//TAAU 
		this->BindFboTemporalUpscale4K();
		temporalAA.TemporalUpscale(Cam.GetViewMatrix(), fboScene->GetFbo_Color_Texture(0), 0, MyWin32::iBlockPixelID);
		temporalAA.ResolveToPreviousFrame(fboTemporalUpscale4k->GetFbo_Color_Texture(0), temporalAA.GetTemporalUpscaled4kTexture(), fboTemporalUpscale4k->GetFboResolution());

		//	this->Render_FinalQuadBartWronskiDownsample(temporalAA.GetTemporalUpscaled4kTexture());
		//	this->Render_FinalQuad(temporalAA.GetTemporalUpscaled4kTempTexture());
		this->Render_Compositor(temporalAA.GetTemporalUpscaled4kTexture(),fboSkyDistant->GetFbo_Color_Texture(0) , fboPostProcess->GetFbo_Color_Texture(0));//clouds->GetCloudTemporalUpscaledTexture()
	}


//	this->Render_FinalQuad(clouds->GetCloudTemporalUpscaledTexture());
}

void ZzRenderer::Render_PaleBlueDotBeginScene()
{
	MyWin32::gNearFarFrustum = vmath::dvec2(10.0, 20000010.0);
	//All gBuffer Objects
	Cam.SetCameraPos(vmath::vec3(-92766.4609f, -6599.75879f, -34873.4805f));
	Cam.SetPitch(-29.8565922f);
	Cam.SetYaw(-335.585632f);
	ZzRenderer::sunAngle = 143.0f;

	DefferedRenderer.BindDefferedFbo();
	{
		//this->Render_Terrain(MyWin32::gLightViewProjectionMatrix);
		//this->Render_TerrainLOD2();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//	this->Render_EarthLODIndia();
	//	this->Render_EarthLODEverest();
	//	this->Render_Axis3D();
		glDisable(GL_BLEND);
	}
	//CloudFbo
//	clouds->RenderComputeCloudShadowMap((GLfloat)AtmoScatter->model().EarthRadius, Cam.GetLightViewMatrix(), this->SunWorldPos, noise.GetTextureID_PerlinWorley3D());
//	this->Render_SkyDistant();
//	this->Render_FinalClouds(RayCaster::fUniformScale, 0.0f, 1.0f, clouds->GetCloudWeatherMapTexture());//0.67 coverageVolume
//	this->Render_FinalQuad(clouds->GetCloudPrevShadowMapTexture());

	//Final Render
	this->Render_SkyPBR(5.0f,vmath::vec3(0.8f,1.0f,1.0f),vmath::vec3(1.0f), 0.0f,vmath::vec2(1.0));

	//TAA Velocity
	//this->BindFboVelocity();
	//temporalAA.CalculateVelocity(Cam.GetViewMatrix(), DefferedRenderer.GetTextureID_DepthBuffer(), noise.GetCurrentHalton23(), Cam.GetCameraPos());//remove *0.0f
	//temporalAA.CalculateMaxVelocity(fboVelocity->GetFbo_Color_Texture(0));

	temporalAA.CalculateMaxVelocity(temporalAA.GetCameraVelocityTexture());


	if (MyWin32::TAA)
	{
		this->BindFboTemporalAA();
		temporalAA.TemporalAntiAlias(Cam.GetViewMatrix(), fboScene->GetFbo_Color_Texture(0), DefferedRenderer.GetTextureID_DepthBuffer(), noise.GetCurrentHalton23(), noise.GetPreviousHalton23());//remove *0.0f
		temporalAA.ResolveToPreviousFrame(fboTemporalAA->GetFbo_Color_Texture(0), temporalAA.GetAntiAliasedTexture(), vmath::uvec2(MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight));

		this->Render_FinalQuad(temporalAA.GetAntiAliasedTexture());

	}
	else
	{
		//TAAU 
		this->BindFboTemporalUpscale4K();
		temporalAA.TemporalUpscale(Cam.GetViewMatrix(), fboScene->GetFbo_Color_Texture(0), 0, MyWin32::iBlockPixelID);
		temporalAA.ResolveToPreviousFrame(fboTemporalUpscale4k->GetFbo_Color_Texture(0), temporalAA.GetTemporalUpscaled4kTexture(), fboTemporalUpscale4k->GetFboResolution());

		//postProcessFbo
		this->Render_SunGlare(noise.GetTextureID_SunGlare(), ZzRenderer::ToSunVector * 1475700000.0f, 1700000000.0f, vmath::vec3(RayCaster::fUniformScale, Cam.GetPitch(), Cam.GetYaw()));

		//	this->Render_FinalQuadBartWronskiDownsample(temporalAA.GetTemporalUpscaled4kTexture());
		//	this->Render_FinalQuad(temporalAA.GetTemporalUpscaled4kTempTexture());
		this->Render_CompositorSpace(temporalAA.GetTemporalUpscaled4kTexture(), fboSkyDistant->GetFbo_Color_Texture(0), fboPostProcess->GetFbo_Color_Texture(0));

	//	//OfflineRender
	//	glReadnPixels(0, 0, 1920, 1080, GL_BGR, GL_UNSIGNED_BYTE,sizeof(GL_UNSIGNED_BYTE)*1920*1080*3, img->ptr());
	//	cv::flip(*img, *Frame,0);
	////	cv::imshow("bocha", *Frame);
	//	VideoWriter.write(*Frame);
	}

//	this->Render_FinalQuad(fboScene->GetFbo_Color_Texture(0),uvec2(1920,1080));

//	vec3 CamPos = Cam.GetCameraPos();
//	float pitch = Cam.GetPitch();
//	float yaw = Cam.GetYaw();

	//	this->Render_FinalQuad(clouds->GetCloudTemporalUpscaledTexture());
}

void ZzRenderer::Render_PaleBlueDotBeginSceneOffline(HWND hwnd, GLboolean isFbo4k)
{
	static GLfloat animFactor=0.0f;
	GLfloat FinalAnimFactor = ((animFactor > 1.0f) ? 1.0f : animFactor);

	MyWin32::fovY = 70.0f;
	MyWin32::gNearFarFrustum = vmath::dvec2(10.0, 20000010.0);
	//All gBuffer Objects
	Cam.SetCameraPos(vmath::vec3(-92766.4609f, -6599.75879f, -34873.4805f)-(vmath::vec3(-92766.4609f, -6599.75879f, -34873.4805f)-vmath::vec3(0.0f,-(GLfloat)AtmoScatter->model().EarthRadius, 0.0f))*0.0f  );
	Cam.SetPitch(-29.8565922f);
	Cam.SetYaw(-335.585632f);
	ZzRenderer::sunAngle = vmath::mix(-33.35f,143.0f, 1.0f);

	//Earth3D
	earth.UpdateEarthRotation(FinalAnimFactor);

	//Main Update
	this->Update(hwnd);

	//SkyPBR
	this->BindSceneOfflineFbo();
	this->Render_SkyPBR(5.0f, vmath::vec3(0.764f,1.0f,1.0f), vmath::vec3(1.0f), 0.0f, vmath::vec2(1.0));

		//postProcessFbo
	//this->Render_SunGlare(noise.GetTextureID_SunGlare(), ZzRenderer::ToSunVector * 1475700000.0f, 1700000000.0f, vmath::vec3(RayCaster::fUniformScale, Cam.GetPitch(), Cam.GetYaw()));


	this->Render_CompositorSpaceOffline(fboSceneOffline->GetFbo_Color_Texture(0), fboSkyDistant->GetFbo_Color_Texture(0), fboPostProcess->GetFbo_Color_Texture(0), isFbo4k,0.0f);

	GLfloat timeInSeconds = 22.0f;
	animFactor += (1.0f / 60.0f) * (1.0f / timeInSeconds);


	//this->Render_FinalQuadBartWronskiDownsample(fboSceneOffline->GetFbo_Color_Texture(0));//offline
	// 
#ifdef RECORD_DEMO
	//OfflineRender
	{
		glReadnPixels(0, 0, 3840, 2160, GL_BGR, GL_UNSIGNED_BYTE, sizeof(GL_UNSIGNED_BYTE) * 3840 * 2160 * 3, img_First->ptr());
		cv::flip(*img_First, *Frame_First, 0);
		VideoWriter.write(*Frame_First);
	}
#endif // RECORD_DEMO
	//	cv::imshow("bocha", *Frame);
}

void ZzRenderer::Render_PaleBlueDotSceneFirstOffline(HWND hwnd, GLboolean isFbo4k)
{
	static GLfloat animFactor = 0.0f;
	GLfloat FinalAnimFactor = ((animFactor > 1.0f) ? 1.0f : animFactor);

	MyWin32::fovY = 70.0f;
	MyWin32::gNearFarFrustum = vmath::dvec2(10.0, 20000010.0);
	//All gBuffer Objects
//	Cam.SetCameraPos(vmath::vec3(32166.1250f, 13484.1104f, -42716.6250f));
	Cam.SetCameraPos(vmath::vec3(32166.1250f, 13484.1104f, -42716.6250f) - (vmath::vec3(32166.1250f, 13484.1104f, -42716.6250f) - vmath::vec3(0.0f, -(GLfloat)AtmoScatter->model().EarthRadius, 0.0f)) * 0.05f* FinalAnimFactor);
	Cam.SetPitch(-46.9261856f);
	Cam.SetYaw(-237.742004f);
	ZzRenderer::sunAngle = 37.577f;

	//Earth3D
	earth.UpdateEarthCloudRotationAggregate(0.72f+FinalAnimFactor*0.3f);

	//Main Update
	this->Update(hwnd);

	//All gBuffer Objects
	DefferedRenderer.BindDefferedFbo();
	{
		//this->Render_Terrain(MyWin32::gLightViewProjectionMatrix);
		//this->Render_TerrainLOD2();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		this->Render_EarthLODIndia(vmath::vec3(1.0f),1);
		//	this->Render_EarthLODEverest();
		//	this->Render_Axis3D();
		glDisable(GL_BLEND);
	}

	//CloudFbo
//	clouds->RenderComputeCloudShadowMap((GLfloat)AtmoScatter->model().EarthRadius, Cam.GetLightViewMatrix(), this->SunWorldPos, noise.GetTextureID_PerlinWorley3D());
//	this->Render_FinalClouds(RayCaster::fUniformScale, 0.0f, 1.0f, clouds->GetCloudWeatherMapTexture());//0.67 coverageVolume

	//SkyPBR
	this->BindSceneOfflineFbo();
	this->Render_SkyPBR(5.0f, vmath::vec3(0.824f,1.0f,1.0f), vmath::vec3(0.253f, 1.000f, 0.152f), FinalAnimFactor, vmath::vec2(1.0));

	this->Render_CompositorSpaceOffline(fboSceneOffline->GetFbo_Color_Texture(0), fboSkyDistant->GetFbo_Color_Texture(0), 0, isFbo4k,0.0f);

	//this->Render_FinalQuad(clouds->GetCloudTemporalUpscaledTexture(), vmath::uvec2(1920, 1080));

	GLfloat timeInSeconds = 16.9f;
	animFactor += (1.0f / 60.0f) * (1.0f / timeInSeconds);

	vmath::vec3 CamPos = Cam.GetCameraPos();
	GLfloat Pitch = Cam.GetPitch();
	GLfloat Yaw = Cam.GetYaw();
}

void ZzRenderer::Render_PaleBlueDotSceneSecondOffline(HWND hwnd, GLboolean isFbo4k)
{
	static GLfloat animFactor = 0.0f;
	GLfloat FinalAnimFactor = ((animFactor > 1.0f) ? 1.0f : animFactor);

	MyWin32::fovY = 50.0f;
	MyWin32::gNearFarFrustum = vmath::dvec2(10.0, 20000010.0);
	//All gBuffer Objects
	Cam.SetCameraPos(vmath::vec3(79549.4219f - 1000.0f * FinalAnimFactor, -29842.0742f , 16575.0254f - 4000.0f * FinalAnimFactor));
	Cam.SetPitch(0.569158554f);
	Cam.SetYaw(-160.643417f);
	ZzRenderer::sunAngle = 162.192f;

	//Earth3D
	earth.UpdateEarthRotationDefault();

	//Main Update
	this->Update(hwnd);


	//All gBuffer Objects
	DefferedRenderer.BindDefferedFbo();
	{
		//this->Render_Terrain(MyWin32::gLightViewProjectionMatrix);
		//this->Render_TerrainLOD2();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			this->Render_EarthLODIndia(vmath::vec3(1.0f),1);
		//	this->Render_EarthLODEverest();
		//	this->Render_Axis3D();
		glDisable(GL_BLEND);
	}

	//SkyPBR
	this->BindSceneOfflineFbo();
	this->Render_SkyPBR(5.0f, vmath::vec3(1.000f, 0.745f, 0.745f), vmath::vec3(0.187f, 0.745f, 0.011f), 0.0f, vmath::vec2(1.0));

	//postProcessFbo
//	this->Render_SunGlare(noise.GetTextureID_SunGlare(), ZzRenderer::ToSunVector * 1475700000.0f, 1700000000.0f, vmath::vec3(RayCaster::fUniformScale, Cam.GetPitch(), Cam.GetYaw()));


	this->Render_CompositorSpaceOffline(fboSceneOffline->GetFbo_Color_Texture(0), fboSkyDistant->GetFbo_Color_Texture(0), fboPostProcess->GetFbo_Color_Texture(0), isFbo4k,0.0f);

	GLfloat timeInSeconds = 10.9f;
	animFactor += (1.0f / 60.0f) * (1.0f / timeInSeconds);


	//this->Render_FinalQuadBartWronskiDownsample(fboSceneOffline->GetFbo_Color_Texture(0));//offline
	
//	vmath::vec3 CamPos = Cam.GetCameraPos();
//	GLfloat Pitch = Cam.GetPitch();
//	GLfloat Yaw = Cam.GetYaw();

}

void ZzRenderer::Render_PaleBlueDotSceneTeacherOffline(HWND hwnd, GLboolean isFbo4k)
{
	static GLfloat animFactor = 0.0f;
	GLfloat FinalAnimFactor = ((animFactor > 1.0f) ? 1.0f : animFactor);

	MyWin32::fovY = 40.0f;
	MyWin32::gNearFarFrustum = vmath::dvec2(10.0, 20000010.0);
	//All gBuffer Objects
	Cam.SetCameraPos(vmath::mix(vmath::vec3(-7384.19727f, 24738.3281f, 12081.1953f),vmath::vec3(-298.390015f, 24738.3281f, 8565.13281f), FinalAnimFactor));
	Cam.SetPitch(-66.7215652f);
	Cam.SetYaw(-116.391052f);
	ZzRenderer::sunAngle = 151.057f;

	//Earth3D
	earth.UpdateEarthCloudRotationTeacher(FinalAnimFactor *0.4f);

	//Main Update
	this->Update(hwnd);

		//All gBuffer Objects
	DefferedRenderer.BindDefferedFbo();
	{
		//this->Render_Terrain(MyWin32::gLightViewProjectionMatrix);
		//this->Render_TerrainLOD2();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		this->Render_EarthLODIndia(vmath::vec3(0.125f, 0.554f, 0.314f),0);
		//	this->Render_EarthLODEverest();
		//	this->Render_Axis3D();
		glDisable(GL_BLEND);
	}

		//CloudFbo
//	clouds->RenderComputeCloudShadowMap((GLfloat)AtmoScatter->model().EarthRadius, Cam.GetLightViewMatrix(), this->SunWorldPos, noise.GetTextureID_PerlinWorley3D());
//	this->Render_FinalClouds(RayCaster::fUniformScale, 0.0f, 1.0f, clouds->GetCloudWeatherMapTexture());//0.67 coverageVolume
	//SkyPBR
	this->BindSceneOfflineFbo();
	this->Render_SkyPBR(3.16f, vmath::vec3(0.6f, 1.0f, 1.0f), *clouds->Getv3SunColor(), 0.0f, vmath::vec2(1.0));

	//postProcessFbo
//	this->Render_SunGlare(noise.GetTextureID_SunGlare(), ZzRenderer::ToSunVector * 1475700000.0f, 1700000000.0f, vmath::vec3(RayCaster::fUniformScale, Cam.GetPitch(), Cam.GetYaw()));


	this->Render_CompositorSpaceOffline(fboSceneOffline->GetFbo_Color_Texture(0), fboSkyDistant->GetFbo_Color_Texture(0), fboPostProcess->GetFbo_Color_Texture(0), isFbo4k,0.0f);

	GLfloat timeInSeconds = 19.6f;
	animFactor += (1.0f / 60.0f) * (1.0f / timeInSeconds);


	//this->Render_FinalQuadBartWronskiDownsample(fboSceneOffline->GetFbo_Color_Texture(0));//offline
	// 

	//	cv::imshow("bocha", *Frame);

	vmath::vec3 CamPos = Cam.GetCameraPos();
	GLfloat Pitch = Cam.GetPitch();
	GLfloat Yaw = Cam.GetYaw();
}

void ZzRenderer::Render_PaleBlueDotSceneBloodRiverOffline(HWND hwnd, GLboolean isFbo4k)
{
	static GLfloat animFactor = 0.0f;
	GLfloat FinalAnimFactor = ((animFactor > 1.0f) ? 1.0f : animFactor);
	//GLfloat FinalAnimFactor =RayCaster::fUniformScale;

	MyWin32::fovY = 70.0f;
	MyWin32::gNearFarFrustum = vmath::dvec2(10.0, 20000010.0);
	//All gBuffer Objects
	Cam.SetCameraPos(vmath::mix(vmath::vec3(335.170197f, 4964.31738f, 36032.1172f), vmath::vec3(20893.7832f, 4964.31738f, 14694.7168f), FinalAnimFactor*0.89f));
	Cam.SetPitch(-41.5789337f);
	Cam.SetYaw(-136.071457f);
	ZzRenderer::sunAngle = 168.326f;
	RayCaster::fUniformScale = FinalAnimFactor;
	//Main Update
	this->Update(hwnd);

	earth.UpdateEarthRotationDefault();

	//All gBuffer Objects
	DefferedRenderer.BindDefferedFbo();
	{
		//this->Render_Terrain(MyWin32::gLightViewProjectionMatrix);
		//this->Render_TerrainLOD2();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		this->Render_EarthLODIndia(vmath::vec3(0.125f, 0.554f, 0.314f),0);
		//	this->Render_EarthLODEverest();
		//	this->Render_Axis3D();
		glDisable(GL_BLEND);
	}

	//CloudFbo
//	clouds->RenderComputeCloudShadowMap((GLfloat)AtmoScatter->model().EarthRadius, Cam.GetLightViewMatrix(), this->SunWorldPos, noise.GetTextureID_PerlinWorley3D());
//	this->Render_FinalClouds(RayCaster::fUniformScale, 0.0f, 1.0f, clouds->GetCloudWeatherMapTexture());//0.67 coverageVolume
	//SkyPBR
	this->BindSceneOfflineFbo();
	this->Render_SkyPBR(7.0f, vmath::vec3(0.75f,1.0f,1.0f), vmath::vec3(0.060f, 0.077f, 0.255f), 10.0f * 0.446f + FinalAnimFactor * 3.0f, vmath::vec2(1.0f, 1.0f));

	//postProcessFbo
//	this->Render_SunGlare(noise.GetTextureID_SunGlare(), ZzRenderer::ToSunVector * 1475700000.0f, 1700000000.0f, vmath::vec3(RayCaster::fUniformScale, Cam.GetPitch(), Cam.GetYaw()));


	this->Render_CompositorSpaceOffline(fboSceneOffline->GetFbo_Color_Texture(0), fboSkyDistant->GetFbo_Color_Texture(0), 0, isFbo4k,0.0f);

	GLfloat timeInSeconds = 25.0f;
	animFactor += (1.0f / 60.0f) * (1.0f / timeInSeconds);


	//this->Render_FinalQuadBartWronskiDownsample(fboSceneOffline->GetFbo_Color_Texture(0));//offline


	//	cv::imshow("bocha", *Frame);

	vmath::vec3 CamPos = Cam.GetCameraPos();
	GLfloat Pitch = Cam.GetPitch();
	GLfloat Yaw = Cam.GetYaw();
}

void ZzRenderer::Render_PaleBlueDotSceneFarEarthOffline(HWND hwnd, GLboolean isFbo4k)
{
	static GLfloat animFactor = 0.0f;
	static GLfloat animFactorSlow = 0.0f;
	GLfloat FinalAnimFactor = ((animFactor > 1.0f) ? 1.0f : animFactor);
	GLfloat FinalAnimFactorForSlow = ((animFactorSlow > 1.0f) ? 1.0f : animFactorSlow);

	GLfloat timeInSeconds = 22.8f;
	GLfloat factorAmount = (1.0f / 60.0f) * (1.0f / timeInSeconds);

	MyWin32::fovY = 40.0f;
	MyWin32::gNearFarFrustum = vmath::dvec2(10.0, 20000010.0);

	GLfloat SlowFactor = (1.0f-vmath::smoothstep(vmath::vec1(0.4f), vmath::vec1(0.94f), vmath::vec1(FinalAnimFactorForSlow))[0])*1.5f;
	//Camera
	Cam.SetCameraPos(vmath::mix( vmath::vec3(302430.875f, 803677.625f, 127792.516f), vmath::vec3(76512.9141f, 149796.125f, 12634.2148f), FinalAnimFactor*0.8f));
	Cam.SetPitch(-68.8037262f);
	Cam.SetYaw(-152.990417f);
	ZzRenderer::sunAngle = 155.991f;

	//Main Update
	this->Update(hwnd);

	//All gBuffer Objects
	DefferedRenderer.BindDefferedFbo();
	{
		//this->Render_Terrain(MyWin32::gLightViewProjectionMatrix);
		//this->Render_TerrainLOD2();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		this->Render_EarthLODIndia(vmath::vec3(0.125f, 0.554f, 0.314f),0);
		//	this->Render_EarthLODEverest();
		//	this->Render_Axis3D();
		glDisable(GL_BLEND);
	}

	//CloudFbo
//	clouds->RenderComputeCloudShadowMap((GLfloat)AtmoScatter->model().EarthRadius, Cam.GetLightViewMatrix(), this->SunWorldPos, noise.GetTextureID_PerlinWorley3D());
//	this->Render_FinalCloudsSpace(RayCaster::fUniformScale, 0.0f, 1.0f, clouds->GetCloudWeatherMapTexture());//0.67 coverageVolume
	//SkyPBR
	this->BindSceneOfflineFbo();
	this->Render_SkyPBR(7.0f, vmath::vec3(0.6f, 1.0f, 1.0f), vmath::vec3(0.253f, 1.000f, 0.152f), 0.0f, vmath::vec2(1.0));

	//postProcessFbo
//	this->Render_SunGlare(noise.GetTextureID_SunGlare(), ZzRenderer::ToSunVector * 1475700000.0f, 1700000000.0f, vmath::vec3(RayCaster::fUniformScale, Cam.GetPitch(), Cam.GetYaw()));

	//Starfield
	this->BindFbo4kExtra();
	this->Render_StarField();
	ResolveFBO(fbo4kExtra->GetFbo(), fbo4kFinal->GetFbo(), GL_COLOR_ATTACHMENT0);

	this->Render_CompositorSpaceOffline(fboSceneOffline->GetFbo_Color_Texture(0), fbo4kFinal->GetFbo_Color_Texture(0), 0, isFbo4k,0.0f);

	animFactor += (factorAmount* SlowFactor);
	animFactorSlow += (factorAmount);


	//this->Render_FinalQuadBartWronskiDownsample(fboSceneOffline->GetFbo_Color_Texture(0));//offline
	//


	//	cv::imshow("bocha", *Frame);

	vmath::vec3 CamPos = Cam.GetCameraPos();
	GLfloat Pitch = Cam.GetPitch();
	GLfloat Yaw = Cam.GetYaw();
}

void ZzRenderer::Render_StarField(void)
{

	StarFieldShader.Start();

	static GLfloat TranslateX = 0.0f;
	GLfloat incFactor = 300.0f ;
	TranslateX += (RayCaster::fUniformScale > 0.143f) ? (incFactor) : 0.0f;

	StarFieldShader.LoadModelViewProjXUniform(MyWin32::gProjectionMatrix*vmath::rotate(45.0f,0.0f,0.0f,1.0f) * vmath::translate(TranslateX, 0.0f, 600000.0f + 1000000.0f * RayCaster::fUniformScale));
	StarFieldShader.LoadIntensityFactorUniform(RayCaster::fUniformScale);

	glDrawArraysInstanced(GL_POINTS, 0, 1, 300000);

	StarFieldShader.Stop();
}

void ZzRenderer::Render_PaleBlueDotSceneFractionOffline(HWND hwnd, GLboolean isFbo4k)
{
	static GLfloat animFactor = 0.0f;
	GLfloat FinalAnimFactor = ((animFactor > 1.0f) ? 1.0f : animFactor);

	MyWin32::fovY = 40.0f;
	MyWin32::gNearFarFrustum = vmath::dvec2(10.0, 20000010.0);
	//All gBuffer Objects
	Cam.SetCameraPos(vmath::vec3(-1009632.62f, -496651.7255f, -313732.312f+ 200000.0f * (1.0f-FinalAnimFactor)));
	Cam.SetPitch(24.4601669f);
	Cam.SetYaw(-344.792908f);
	ZzRenderer::sunAngle = 26.476f;

	//Main Update
	this->Update(hwnd);

	//All gBuffer Objects
	DefferedRenderer.BindDefferedFbo();
	{
		//this->Render_Terrain(MyWin32::gLightViewProjectionMatrix);
		//this->Render_TerrainLOD2();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//this->Render_EarthLODIndia(vmath::vec3(0.125f, 0.554f, 0.314f));
		//	this->Render_EarthLODEverest();
		//	this->Render_Axis3D();
		glDisable(GL_BLEND);
	}

	//CloudFbo
//	clouds->RenderComputeCloudShadowMap((GLfloat)AtmoScatter->model().EarthRadius, Cam.GetLightViewMatrix(), this->SunWorldPos, noise.GetTextureID_PerlinWorley3D());
//	this->Render_FinalCloudsSpace(RayCaster::fUniformScale, 0.0f, 1.0f, clouds->GetCloudWeatherMapTexture());//0.67 coverageVolume
	//SkyPBR
	this->BindSceneOfflineFbo();
	this->Render_SkyPBR(7.0f, vmath::vec3(0.6f, 1.0f, 1.0f), *clouds->Getv3SunColor(), FinalAnimFactor, vmath::vec2(1.0));

	//postProcessFbo
//	this->Render_SunGlare(noise.GetTextureID_SunGlare(), ZzRenderer::ToSunVector * 1475700000.0f, 1700000000.0f, vmath::vec3(RayCaster::fUniformScale, Cam.GetPitch(), Cam.GetYaw()));


	//Starfield
	this->BindFbo4kExtra();
	this->Render_StarField();

	this->Render_SingleBillboard(noise.GetTextureID_BlueGlow(), vec3(0.0, -(GLfloat)AtmoScatter->model().EarthRadius, 0.0f), vmath::vec3(80000.0f ), 1.5f);

	ResolveFBO(fbo4kExtra->GetFbo(), fbo4kFinal->GetFbo(), GL_COLOR_ATTACHMENT0);

	this->Render_CompositorSpaceOffline(fboSceneOffline->GetFbo_Color_Texture(0), fbo4kFinal->GetFbo_Color_Texture(0),0, isFbo4k,0.0f);

	GLfloat timeInSeconds = 22.1f;
	animFactor += (1.0f / 60.0f) * (1.0f / timeInSeconds);


	//this->Render_FinalQuadBartWronskiDownsample(fboSceneOffline->GetFbo_Color_Texture(0));//offline
	// 
	//	cv::imshow("bocha", *Frame);

	vmath::vec3 CamPos = Cam.GetCameraPos();
	GLfloat Pitch = Cam.GetPitch();
	GLfloat Yaw = Cam.GetYaw();
}

void ZzRenderer::Render_PaleBlueDotSceneLonelySpeckOffline(HWND hwnd, GLboolean isFbo4k)//1200 + 60
{
	static GLfloat animFactor = 0.0f;
	GLfloat FinalAnimFactor = ((animFactor > 1.0f) ? 1.0f : animFactor);

	MyWin32::fovY = 70.0f;
	MyWin32::gNearFarFrustum = vmath::dvec2(10.0, 20000010.0);
	//All gBuffer Objects
	MyOpenGL::fExposure = 10.0f;

	GLfloat mixFactor1 = vmath::clamp(vmath::vec1(FinalAnimFactor * 2.0f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];
	GLfloat mixFactor2 = 2.0f * vmath::clamp(vmath::vec1(FinalAnimFactor - 0.5f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];

	//CameraPosition
	vmath::vec3 Cam1 = vmath::vec3(-4252.05469f, 3710.12744f, -4244.91943f);
	vmath::vec3 Cam2 = vmath::vec3(-2191.32690f, 1403.13477f, -2077.30054f);
	vmath::vec3 Cam3 = vmath::vec3(24.8724747f, 787.992554f, -1390.52368f);
	vmath::vec3 FinalCam = vmath::mix(Cam1, Cam2, mixFactor1);
	FinalCam = vmath::mix(FinalCam, Cam3, mixFactor2);
	//Pitch
	GLfloat Pitch1 = -11.33990145f;
	GLfloat Pitch2 = -14.1053200f;
	GLfloat Pitch3 = 0.0f;
	GLfloat FinalPitch = vmath::mix(Pitch1, Pitch2, FinalAnimFactor);
//	 FinalPitch = vmath::mix(FinalPitch, Pitch3, mixFactor2);

	 //Yaw
	GLfloat Yaw1 = 102.319427f;
	GLfloat Yaw2 = 122.213913f;
	GLfloat Yaw3 = 0.0f;
	GLfloat FinalYaw = vmath::mix(Yaw1, Yaw2, FinalAnimFactor);
//	FinalYaw = vmath::mix(FinalYaw, Yaw3, mixFactor2);

	Cam.SetCameraPos(vmath::vec3(2676.20410f, 2646.03003f, -5174.63721f));
	Cam.SetPitch(FinalPitch);
	Cam.SetYaw(FinalYaw);
	//ZzRenderer::sunAngle = vmath::mix(18.943f, 11.674f, FinalAnimFactor);
	ZzRenderer::sunAngle =  11.674f;


	//Earth3D
	earth.UpdateEarthCloudRotationDelusion();
	//earth.UpdateEarthRotationDefault();

	//Main Update
	this->Update(hwnd);

	//All gBuffer Objects
	DefferedRenderer.BindDefferedFbo();
	{
		//this->Render_Terrain(MyWin32::gLightViewProjectionMatrix);
		//this->Render_TerrainLOD2();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		this->Render_EarthLODIndia(vmath::vec3(0.125f, 0.554f, 0.314f),1);
		this->Render_EarthLODEverest();
		//	this->Render_Axis3D();
		glDisable(GL_BLEND);
	}

	//CloudFbo
	clouds->RenderComputeCloudShadowMap((GLfloat)AtmoScatter->model().EarthRadius, Cam.GetLightViewMatrix(), this->SunWorldPos, noise.GetTextureID_PerlinWorley3D());
	this->Render_FinalClouds(RayCaster::fUniformScale, 0.0f, 1.0f, clouds->GetCloudWeatherMapTexture());//0.67 coverageVolume
	//SkyPBR
	this->BindSceneOfflineFbo();
	this->Render_SkyPBR(5.0f, vmath::vec3(0.6f, 1.0f, 1.0f), *clouds->Getv3SunColor(), 0.0f, vmath::vec2(6.0f* 0.498f,0.0f));//0.494f delusion

	//postProcessFbo
	this->Render_SunGlare(noise.GetTextureID_SunGlare(), ZzRenderer::ToSunVector * 1475700000.0f, 2700000000.0f, vmath::vec3(RayCaster::fUniformScale, Cam.GetPitch(), Cam.GetYaw()));

	this->Render_CompositorSpaceOffline(fboSceneOffline->GetFbo_Color_Texture(0), fboSkyDistant->GetFbo_Color_Texture(0), fboPostProcess->GetFbo_Color_Texture(0), isFbo4k,0.0f);

	GLfloat timeInSeconds = 13.4f;
	animFactor += (1.0f / 60.0f) * (1.0f / timeInSeconds);


	//this->Render_FinalQuadBartWronskiDownsample(fboSceneOffline->GetFbo_Color_Texture(0));//offline
	// 

	//	cv::imshow("bocha", *Frame);

	vmath::vec3 CamPos = Cam.GetCameraPos();
	GLfloat Pitch = Cam.GetPitch();
	GLfloat Yaw = Cam.GetYaw();
}

void ZzRenderer::Render_PaleBlueDotSceneDelusionOffline(HWND hwnd, GLboolean isFbo4k)
{
	static GLfloat animFactor = 0.0f;
	GLfloat FinalAnimFactor = ((animFactor > 1.0f) ? 1.0f : animFactor);
	//GLfloat FinalAnimFactor = 0.15f;

	MyWin32::fovY = 70.0f;
	MyWin32::gNearFarFrustum = vmath::dvec2(10.0, 20000010.0);
	//All gBuffer Objects

	GLfloat mixFactor1 = vmath::clamp(vmath::vec1(FinalAnimFactor * 2.0f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];
	GLfloat mixFactor2 = 2.0f * vmath::clamp(vmath::vec1(FinalAnimFactor - 0.5f), vmath::vec1(0.0f), vmath::vec1(1.0f))[0];


	//mixFactor1 = vmath::smoothstep(vmath::vec1(0.0f), vmath::vec1(1.0f), vmath::vec1(mixFactor1))[0];
	//mixFactor2 = vmath::smoothstep(vmath::vec1(0.0f), vmath::vec1(1.0f), vmath::vec1(mixFactor2))[0];

	//CameraPosition
	vmath::vec3 Cam1 = vmath::vec3(-4252.05469f, 3710.12744f, -4244.91943f);
	vmath::vec3 Cam2 = vmath::vec3(-2191.32690f, 1403.13477f, -2077.30054f);
	vmath::vec3 Cam3 = vmath::vec3(24.8724747f, 787.992554f, -1390.52368f);
	vmath::vec3 FinalCam = vmath::mix(Cam1, Cam2, mixFactor1);
	FinalCam = vmath::mix(FinalCam, Cam3, mixFactor2);
	//Pitch
	GLfloat Pitch1 = -42.5145721f;
	GLfloat Pitch2 = -29.8564529f;
	GLfloat Pitch3 = -36.4503479f;
	GLfloat FinalPitch = vmath::mix(Pitch1, Pitch2, mixFactor1);
	 FinalPitch = vmath::mix(FinalPitch, Pitch3, mixFactor2);

	 //Yaw
	GLfloat Yaw1 = 52.9477310f;
	GLfloat Yaw2 = 30.3528938f;
	GLfloat Yaw3 = 6.51341772f;
	GLfloat FinalYaw = vmath::mix(Yaw1, Yaw2, mixFactor1);
	FinalYaw = vmath::mix(FinalYaw, Yaw3, mixFactor2);

	Cam.SetCameraPos(FinalCam);
	Cam.SetPitch(FinalPitch-30.0f*0.26f*mixFactor2);
	Cam.SetYaw(FinalYaw);
	ZzRenderer::sunAngle = 28.643f;
	//ZzRenderer::sunAngle = 160.643f;

	//Earth3D
	earth.UpdateEarthCloudRotationDelusion();
	//earth.UpdateEarthRotationDefault();

	//Main Update
	this->Update(hwnd);

	//All gBuffer Objects
	DefferedRenderer.BindDefferedFbo();
	{
		//this->Render_Terrain(MyWin32::gLightViewProjectionMatrix);
		//this->Render_TerrainLOD2();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		this->Render_EarthLODIndia(vmath::vec3(0.125f, 0.554f, 0.314f),1);
		this->Render_EarthLODEverest();
		//	this->Render_Axis3D();
		glDisable(GL_BLEND);
	}

	//CloudFbo
	if (MyWin32::iFrameID == 1)
	{
		clouds->RenderComputeCloudShadowMap((GLfloat)AtmoScatter->model().EarthRadius, Cam.GetLightViewMatrix(), this->SunWorldPos, noise.GetTextureID_PerlinWorley3D());
	}

	this->Render_FinalClouds(RayCaster::fUniformScale, 0.0f, 1.0f, clouds->GetCloudWeatherMapTexture());//0.67 coverageVolume
	//SkyPBR
	this->BindSceneOfflineFbo();
	this->Render_SkyPBR(5.0f, vmath::vec3(0.6f, 1.0f, 1.0f), *clouds->Getv3SunColor(), 0.0f, vmath::vec2(6.0f*0.455f,0.0f));

	//postProcessFbo
//	this->Render_SunGlare(noise.GetTextureID_SunGlare(), ZzRenderer::ToSunVector * 1475700000.0f, 1700000000.0f, vmath::vec3(RayCaster::fUniformScale, Cam.GetPitch(), Cam.GetYaw()));

	this->Render_CompositorSpaceOffline(fboSceneOffline->GetFbo_Color_Texture(0), fboSkyDistant->GetFbo_Color_Texture(0), 0, isFbo4k,0.0f);

	GLfloat timeInSeconds = 23.5f;
	animFactor += (1.0f / 60.0f) * (1.0f / timeInSeconds);


	//this->Render_FinalQuadBartWronskiDownsample(fboSceneOffline->GetFbo_Color_Texture(0));//offline
	// 

	//	cv::imshow("bocha", *Frame);

	vmath::vec3 CamPos = Cam.GetCameraPos();
	GLfloat Pitch = Cam.GetPitch();
	GLfloat Yaw = Cam.GetYaw();
}

void ZzRenderer::Render_PaleBlueDotScenePlaneCloudsOffline(HWND hwnd, GLboolean isFbo4k)
{
	static GLfloat animFactor = 0.0f;
	GLfloat FinalAnimFactor = ((animFactor > 1.0f) ? 1.0f : animFactor);

	MyWin32::fovY = 70.0f;
	MyWin32::gNearFarFrustum = vmath::dvec2(10.0, 20000010.0);
	//All gBuffer Objects
//	Cam.SetCameraPos(vmath::mix(vmath::vec3(335.170197f, 4964.31738f, 36032.1172f), vmath::vec3(20893.7832f, 4964.31738f, 14694.7168f), FinalAnimFactor));
//	Cam.SetPitch(-41.5789337f);
//	Cam.SetYaw(-136.071457f);
//	ZzRenderer::sunAngle = 168.326f;

	//Main Update
	this->Update(hwnd);

	//All gBuffer Objects
	DefferedRenderer.BindDefferedFbo();
	{
		//this->Render_Terrain(MyWin32::gLightViewProjectionMatrix);
		//this->Render_TerrainLOD2();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//this->Render_EarthLODIndia(vmath::vec3(0.125f, 0.554f, 0.314f));
		//	this->Render_EarthLODEverest();
		//	this->Render_Axis3D();
		glDisable(GL_BLEND);
	}

	//CloudFbo
//	clouds->RenderComputeCloudShadowMap((GLfloat)AtmoScatter->model().EarthRadius, Cam.GetLightViewMatrix(), this->SunWorldPos, noise.GetTextureID_PerlinWorley3D());
	this->Render_FinalCloudsSpace(RayCaster::fUniformScale, 0.0f, 1.0f, clouds->GetCloudWeatherMapTileableTexture());//0.67 coverageVolume
	//SkyPBR
	this->BindSceneOfflineFbo();
//	this->Render_SkyPBR(3.16f, vmath::vec3(0.6f, 1.0f, 1.0f), *clouds->Getv3SunColor(), 0.0f, vmath::vec2(1.0));

	//postProcessFbo
//	this->Render_SunGlare(noise.GetTextureID_SunGlare(), ZzRenderer::ToSunVector * 1475700000.0f, 1700000000.0f, vmath::vec3(RayCaster::fUniformScale, Cam.GetPitch(), Cam.GetYaw()));
	
	this->Render_SkyDistant();


	this->Render_CompositorSpaceOffline(fboSceneOffline->GetFbo_Color_Texture(0), fboSkyDistant->GetFbo_Color_Texture(0), 0, isFbo4k,0.0f);

	GLfloat timeInSeconds = 24.0f;
	animFactor += (1.0f / 60.0f) * (1.0f / timeInSeconds);


	//this->Render_FinalQuadBartWronskiDownsample(fboSceneOffline->GetFbo_Color_Texture(0));//offline
	// 

	//	cv::imshow("bocha", *Frame);

	vmath::vec3 CamPos = Cam.GetCameraPos();
	GLfloat Pitch = Cam.GetPitch();
	GLfloat Yaw = Cam.GetYaw();
}

void ZzRenderer::Render_SingleBillboard(GLuint TextureID, vmath::vec3 position, vmath::vec3 size,GLfloat Intensity)
{
	//this->BindFboPostProcess();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	SingleBillboardShader.Start();

	SingleBillboardShader.Loadv3ScaleUniform(size);
	SingleBillboardShader.LoadfIntensityUniform(Intensity);
	SingleBillboardShader.LoadSamplerBillboardUniform(GL_TEXTURE0,0, TextureID);

	vmath::mat4 ModelViewMatrix = Cam.GetViewMatrix() * vmath::translate(position);
	ModelViewMatrix[0][0] = 1.0f;
	ModelViewMatrix[0][1] = 0.0f;
	ModelViewMatrix[0][2] = 0.0f;
	ModelViewMatrix[1][0] = 0.0f;
	ModelViewMatrix[1][1] = 1.0f;
	ModelViewMatrix[1][2] = 0.0f;
	ModelViewMatrix[2][0] = 0.0f;
	ModelViewMatrix[2][1] = 0.0f;
	ModelViewMatrix[2][2] = 1.0f;
	SingleBillboardShader.LoadModelViewProjXUniform(MyWin32::gProjectionMatrix* ModelViewMatrix * vmath::rotate(RayCaster::fRotAngleZ, 0.0f, 0.0f, 1.0f));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisable(GL_BLEND);
}

void ZzRenderer::Render_SunGlare(GLuint TextureID,vmath::vec3 position, GLfloat size,vmath::vec3 fFactorPitchYaw)
{
	this->BindFboPostProcess();

	SunGlareShader.Start();

	SunGlareShader.Loadv3ScaleUniform(vmath::vec3(size));
	SunGlareShader.LoadFactorPitchYawUniform(fFactorPitchYaw);
	SunGlareShader.LoadSamplerStockFlareUniform(GL_TEXTURE0, 0, TextureID);

	vmath::mat4 ModelViewMatrix = Cam.GetViewMatrix() * vmath::translate(position);
	ModelViewMatrix[0][0] = 1.0f;
	ModelViewMatrix[0][1] = 0.0f;
	ModelViewMatrix[0][2] = 0.0f;
	ModelViewMatrix[1][0] = 0.0f;
	ModelViewMatrix[1][1] = 1.0f;
	ModelViewMatrix[1][2] = 0.0f;
	ModelViewMatrix[2][0] = 0.0f;
	ModelViewMatrix[2][1] = 0.0f;
	ModelViewMatrix[2][2] = 1.0f;
	SunGlareShader.LoadModelViewProjXUniform(MyWin32::gProjectionMatrixForDepth * ModelViewMatrix);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	SunGlareShader.Stop();
}

//OpenCV

//bool ZzRenderer::Render_Video(const GLchar* VideoFileName,unsigned int animationSpeedDelay)
//{
//	static unsigned int FrameCounter = 0;
//
//	if (FrameCounter == 0)
//	{
//		VideoCap.release();
//
//		if (!VideoCap.open(VideoFileName))
//		{
//			MessageBox(NULL, "Error opening video stream or file (OpenCV)", "Stream Error!", MB_OK);
//		}
//
//	}
//
//
//	int totalFramesInVideoFile = (int)VideoCap.get(cv::CAP_PROP_FRAME_COUNT);
//
//	if (FrameCounter > (totalFramesInVideoFile * animationSpeedDelay))
//	{
//		FrameCounter = 0;
//		return true;
//	}
//
//	BindDefaultFbo();
//
//	cv::Mat frame;
//
//	bool hasFrame = false;
//	
//	if (FrameCounter % animationSpeedDelay == 0)
//	{
//		hasFrame = VideoCap.read(frame);
//	}
//
//
//	if (hasFrame)
//	{
//		flip(frame, frame, 0);
//
//		glBindTexture(GL_TEXTURE_2D, VideoFrameTextureID);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
//		glBindTexture(GL_TEXTURE_2D, 0);
//
//	}
//
//
//
//	finalQuadShader.Start();
//	finalQuadShader.LoadSampler2DTexture0_Uniform(GL_TEXTURE0, 0, VideoFrameTextureID);
//	glDrawArrays(GL_QUADS, 0, 4);
//	finalQuadShader.Stop();
//
//	FrameCounter += 1;
//
//
//	return false;
//}

void ZzRenderer::Render_SingleScene(cv::VideoCapture Cap_First, cv::VideoCapture Cap_Second ,GLuint TotalFrames,GLuint CurrentFrame ,GLuint BlueNoiseTextureID)
{
	static GLfloat mixFactor = 0.0f;

	if ((CurrentFrame < (TotalFrames - 60))&& Cap_First.isOpened())
	{
		Cap_First.read(*img_First);
		cv::flip(*img_First, *Frame_First, 0);
	}
	else if(Cap_Second.isOpened())
	{
		Cap_First.read(*img_First);
		cv::flip(*img_First, *Frame_First, 0);

		Cap_Second.read(*img_Second);
		cv::flip(*img_Second, *Frame_Second, 0);

		mixFactor += (1.0f / 60.0f);

	}
	else
	{
		Cap_First.read(*img_First);
		cv::flip(*img_First, *Frame_First, 0);
		mixFactor = 0.0f;
	}


	glBindTexture(GL_TEXTURE_2D, VideoFrameTextureID_First);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 3840, 2160, 0, GL_BGR, GL_UNSIGNED_BYTE, Frame_First->ptr());
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, VideoFrameTextureID_Second);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 3840, 2160, 0, GL_BGR, GL_UNSIGNED_BYTE, Frame_Second->ptr());
	glBindTexture(GL_TEXTURE_2D, 0);

	this->BindFbo4k();
	Render_TwoTextureBlendQuad(VideoFrameTextureID_First, VideoFrameTextureID_Second, mixFactor, BlueNoiseTextureID);

	//OfflineRender
	{
		glReadnPixels(0, 0, 3840, 2160, GL_BGR, GL_UNSIGNED_BYTE, sizeof(GL_UNSIGNED_BYTE) * 3840 * 2160 * 3, img_First->ptr());
		cv::flip(*img_First, *Frame_First, 0);

		//VideoCap.read(*Frame);

		VideoWriter.write(*Frame_First);
	}

	if (CurrentFrame == (TotalFrames - 1))
	{
		mixFactor = 0.0f;
	}
}

void ZzRenderer::Render_SingleSceneCredit(cv::VideoCapture Cap_First, cv::VideoCapture Cap_Second, GLuint TotalFrames, GLuint CurrentFrame, GLuint BlueNoiseTextureID)
{
	static GLfloat mixFactor = 1.0f;

	{
		Cap_First.read(*img_First);
		cv::flip(*img_First, *Frame_First, 0);

	}

	mixFactor -= (1.0f / 60.0f);
	mixFactor = clamp(mixFactor, 0.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, VideoFrameTextureID_First);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 3840, 2160, 0, GL_BGR, GL_UNSIGNED_BYTE, Frame_First->ptr());
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, VideoFrameTextureID_Second);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 3840, 2160, 0, GL_BGR, GL_UNSIGNED_BYTE, Frame_Second->ptr());
	glBindTexture(GL_TEXTURE_2D, 0);

	this->BindFbo4k();
	Render_TwoTextureBlendQuad(VideoFrameTextureID_First, 0, mixFactor, BlueNoiseTextureID);

	//OfflineRender
	{
		glReadnPixels(0, 0, 3840, 2160, GL_BGR, GL_UNSIGNED_BYTE, sizeof(GL_UNSIGNED_BYTE) * 3840 * 2160 * 3, img_First->ptr());
		cv::flip(*img_First, *Frame_First, 0);

		//VideoCap.read(*Frame);

		VideoWriter.write(*Frame_First);
	}

	if (CurrentFrame == (TotalFrames - 1))
	{
		mixFactor = 0.0f;
	}
}

void ZzRenderer::Render_CreatePaleBlueDotVideo(void)
{
	static bool bEveryOneWeLoveDone = false;
	static bool bAggregateDone = false;
	static bool bCoupleInLoveDone = false;
	static bool bTeacherOfMoralDone = false;
	static bool bBloodRiverDone = false;
	static bool bFractionDotDone = false;
	static bool bDelusionDone = false;
	static bool bLonelySpeckDone = false;
	static bool bLifeDone = false;
	static bool bAfterLifeDone = false;
	static bool bFinalSceneDone = false;
	static bool bCreditDone = false;

	if (bCreditDone == false)
	{
		GLsizei First_Total_Frames = (GLsizei)VideoCapCredit.get(cv::CAP_PROP_FRAME_COUNT);
		GLsizei First_Current_FrameID = (GLsizei)VideoCapCredit.get(cv::CAP_PROP_POS_FRAMES);

		if (First_Current_FrameID < First_Total_Frames)
		{
			this->Render_SingleSceneCredit(VideoCapCredit, VideoCapEmpty, First_Total_Frames, First_Current_FrameID, noise.GetTextureID_BlueNoise());
		}
		else
		{
			//black silence 2 sec
			for (int i = 0; i < 120; i++)
			{
				BindFbo4k();
				Render_TwoTextureBlendQuad(0,0,0,0);

				glReadnPixels(0, 0, 3840, 2160, GL_BGR, GL_UNSIGNED_BYTE, sizeof(GL_UNSIGNED_BYTE) * 3840 * 2160 * 3, img_First->ptr());
				cv::flip(*img_First, *Frame_First, 0);

				VideoWriter.write(*Frame_First);
			}

			bCreditDone = true;
		}
	}
	else
	{

		if (bEveryOneWeLoveDone == false)
		{
			GLsizei First_Total_Frames = (GLsizei)VideoCapEveryoneLove.get(cv::CAP_PROP_FRAME_COUNT);
			GLsizei First_Current_FrameID = (GLsizei)VideoCapEveryoneLove.get(cv::CAP_PROP_POS_FRAMES);

			if (First_Current_FrameID < First_Total_Frames)
			{
				this->Render_SingleScene(VideoCapEveryoneLove, VideoCapAggregate, First_Total_Frames, First_Current_FrameID, noise.GetTextureID_BlueNoise());
			}
			else
			{
				bEveryOneWeLoveDone = true;
			}
		}
		else if (bAggregateDone == false)
		{
			GLsizei First_Total_Frames = (GLsizei)VideoCapAggregate.get(cv::CAP_PROP_FRAME_COUNT);
			GLsizei First_Current_FrameID = (GLsizei)VideoCapAggregate.get(cv::CAP_PROP_POS_FRAMES);

			if (First_Current_FrameID < First_Total_Frames)
			{
				this->Render_SingleScene(VideoCapAggregate, VideoCapCoupleInLove, First_Total_Frames, First_Current_FrameID, noise.GetTextureID_BlueNoise());
			}
			else
			{
				bAggregateDone = true;
			}
		}
		else if (bCoupleInLoveDone == false)
		{
			GLsizei First_Total_Frames = (GLsizei)VideoCapCoupleInLove.get(cv::CAP_PROP_FRAME_COUNT);
			GLsizei First_Current_FrameID = (GLsizei)VideoCapCoupleInLove.get(cv::CAP_PROP_POS_FRAMES);

			if (First_Current_FrameID < First_Total_Frames)
			{
				this->Render_SingleScene(VideoCapCoupleInLove, VideoCapTeacherOfMoral, First_Total_Frames, First_Current_FrameID, noise.GetTextureID_BlueNoise());
			}
			else
			{
				bCoupleInLoveDone = true;
			}
		}
		else if (bTeacherOfMoralDone == false)
		{

			GLsizei First_Total_Frames = (GLsizei)VideoCapTeacherOfMoral.get(cv::CAP_PROP_FRAME_COUNT);
			GLsizei First_Current_FrameID = (GLsizei)VideoCapTeacherOfMoral.get(cv::CAP_PROP_POS_FRAMES);

			if (First_Current_FrameID < First_Total_Frames)
			{
				this->Render_SingleScene(VideoCapTeacherOfMoral, VideoCapBloodRiver, First_Total_Frames, First_Current_FrameID, noise.GetTextureID_BlueNoise());
			}
			else
			{
				bTeacherOfMoralDone = true;
			}
		}
		else if (bBloodRiverDone == false)
		{
			GLsizei First_Total_Frames = (GLsizei)VideoCapBloodRiver.get(cv::CAP_PROP_FRAME_COUNT);
			GLsizei First_Current_FrameID = (GLsizei)VideoCapBloodRiver.get(cv::CAP_PROP_POS_FRAMES);

			if (First_Current_FrameID < First_Total_Frames)
			{
				this->Render_SingleScene(VideoCapBloodRiver, VideoCapFractionOfDot, First_Total_Frames, First_Current_FrameID, noise.GetTextureID_BlueNoise());
			}
			else
			{
				bBloodRiverDone = true;
			}
		}
		else if (bFractionDotDone == false)
		{

			GLsizei First_Total_Frames = (GLsizei)VideoCapFractionOfDot.get(cv::CAP_PROP_FRAME_COUNT);
			GLsizei First_Current_FrameID = (GLsizei)VideoCapFractionOfDot.get(cv::CAP_PROP_POS_FRAMES);

			if (First_Current_FrameID < First_Total_Frames)
			{
				this->Render_SingleScene(VideoCapFractionOfDot, VideoCapDelusion, First_Total_Frames, First_Current_FrameID, noise.GetTextureID_BlueNoise());
			}
			else
			{
				bFractionDotDone = true;
			}

		}
		else if (bDelusionDone == false)
		{

			GLsizei First_Total_Frames = (GLsizei)VideoCapDelusion.get(cv::CAP_PROP_FRAME_COUNT);
			GLsizei First_Current_FrameID = (GLsizei)VideoCapDelusion.get(cv::CAP_PROP_POS_FRAMES);

			if (First_Current_FrameID < First_Total_Frames)
			{
				this->Render_SingleScene(VideoCapDelusion, VideoCapLonelySpeck, First_Total_Frames, First_Current_FrameID, noise.GetTextureID_BlueNoise());
			}
			else
			{
				bDelusionDone = true;
			}

		}
		else if (bLonelySpeckDone == false)
		{

			GLsizei First_Total_Frames = (GLsizei)VideoCapLonelySpeck.get(cv::CAP_PROP_FRAME_COUNT);
			GLsizei First_Current_FrameID = (GLsizei)VideoCapLonelySpeck.get(cv::CAP_PROP_POS_FRAMES);

			if (First_Current_FrameID < First_Total_Frames)
			{
				this->Render_SingleScene(VideoCapLonelySpeck, VideoCapLife, First_Total_Frames, First_Current_FrameID, noise.GetTextureID_BlueNoise());
			}
			else
			{
				bLonelySpeckDone = true;
			}

		}
		else if (bLifeDone == false)
		{

			GLsizei First_Total_Frames = (GLsizei)VideoCapLife.get(cv::CAP_PROP_FRAME_COUNT);
			GLsizei First_Current_FrameID = (GLsizei)VideoCapLife.get(cv::CAP_PROP_POS_FRAMES);

			if (First_Current_FrameID < First_Total_Frames)
			{
				this->Render_SingleScene(VideoCapLife, VideoCapAfterLife, First_Total_Frames, First_Current_FrameID, noise.GetTextureID_BlueNoise());
			}
			else
			{
				bLifeDone = true;
			}

		}
		else if (bAfterLifeDone == false)
		{

			GLsizei First_Total_Frames = (GLsizei)VideoCapAfterLife.get(cv::CAP_PROP_FRAME_COUNT);
			GLsizei First_Current_FrameID = (GLsizei)VideoCapAfterLife.get(cv::CAP_PROP_POS_FRAMES);

			if (First_Current_FrameID < First_Total_Frames)
			{
				this->Render_SingleScene(VideoCapAfterLife, VideoCapEmpty, First_Total_Frames, First_Current_FrameID, noise.GetTextureID_BlueNoise());
			}
			else
			{
				bAfterLifeDone = true;
			}

		}
		else if (bFinalSceneDone == false)
		{

			GLsizei First_Total_Frames = (GLsizei)VideoCapFinalScene.get(cv::CAP_PROP_FRAME_COUNT);
			GLsizei First_Current_FrameID = (GLsizei)VideoCapFinalScene.get(cv::CAP_PROP_POS_FRAMES);

			if (First_Current_FrameID < First_Total_Frames)
			{
				this->Render_SingleScene(VideoCapFinalScene, VideoCapEmpty, First_Total_Frames, First_Current_FrameID, noise.GetTextureID_BlueNoise());
			}
			else
			{
				bFinalSceneDone = true;
			}

		}

	}
	
	

	Render_FinalQuad(fbo4k->GetFbo_Color_Texture(0));
}

void ZzRenderer::Render_FinalQuad(GLuint texture)
{
	BindDefaultFbo();

	finalQuadShader.Start();
	finalQuadShader.LoadSampler2DTexture0_Uniform(GL_TEXTURE0, 0, texture);
	glDrawArrays(GL_QUADS, 0, 4);
	finalQuadShader.Stop();
}

void ZzRenderer::Render_FinalQuad(GLuint texture,uvec2 WidthHeight)
{
	BindDefaultFbo(WidthHeight);

	finalQuadShader.Start();
	finalQuadShader.LoadSampler2DTexture0_Uniform(GL_TEXTURE0, 0, texture);
	glDrawArrays(GL_QUADS, 0, 4);
	finalQuadShader.Stop();
}

void ZzRenderer::Render_FinalQuadBartWronskiDownsample(GLuint texture)
{
	BindFbo4k();

	FinalQuadBartWronskiDownsampleShader.Start();
	FinalQuadBartWronskiDownsampleShader.LoadSampler2DTexture0_Uniform(GL_TEXTURE0, 0, texture);
	glDrawArrays(GL_QUADS, 0, 4);
	FinalQuadBartWronskiDownsampleShader.Stop();
}

void ZzRenderer::Render_FinalQuadBartWronskiDownsample(GLuint texture, uvec2 WidthHeight)
{
	BindDefaultFbo(WidthHeight);

	FinalQuadBartWronskiDownsampleShader.Start();
	FinalQuadBartWronskiDownsampleShader.LoadSampler2DTexture0_Uniform(GL_TEXTURE0, 0, texture);
	glDrawArrays(GL_QUADS, 0, 4);
	FinalQuadBartWronskiDownsampleShader.Stop();
}

void ZzRenderer::Render_TwoTextureBlendQuad(GLuint TextureID_0, GLuint TextureID_1, GLfloat factor, GLuint BlueNoiseTextureID)
{
	twoTextureQuadShader.Start();

	twoTextureQuadShader.LoadSampler2DTexture0_Uniform(GL_TEXTURE0, 0, TextureID_0);
	twoTextureQuadShader.LoadSampler2DTexture1_Uniform(GL_TEXTURE1, 1, TextureID_1);
	twoTextureQuadShader.LoadSampler2DBlueNoise_Uniform(GL_TEXTURE2, 2, BlueNoiseTextureID);

	twoTextureQuadShader.LoadBlendFactor(factor);
	twoTextureQuadShader.LoadDitherFactor(RayCaster::fUniformScale);
	twoTextureQuadShader.LoadFrameID_Uniform(MyWin32::iFrameID);

	glDrawArrays(GL_QUADS, 0, 4);

	twoTextureQuadShader.Stop();
}


void ZzRenderer::Render_SkyPBR(GLfloat Exposure,vmath::vec3 Tint, vmath::vec3 WaterTint,GLfloat AnimFactor,vmath::vec2 Cloud2D_Scale_Visibility)
{
	SkyPBR_Shader->Start();
	//float
	SkyPBR_Shader->LoadFactorUniform(RayCaster::fUniformScale);
	SkyPBR_Shader->LoadTimeUniform(AnimFactor);
	SkyPBR_Shader->LoadExposureUniform(Exposure);
	//matrices
	SkyPBR_Shader->LoadInvProjXUniform(MyWin32::gInvProjectionMatrix);
	SkyPBR_Shader->LoadPrevViewProjXUniform(MyWin32::gProjectionMatrix*MyWin32::gPreviousViewMatrix);
	SkyPBR_Shader->LoadInvViewXUniform(MyWin32::gInvViewMatrix);
	SkyPBR_Shader->Loadm4LightViewProjXUniform(MyWin32::gLightViewProjectionMatrix);

	//	vmath::mat4 rotMatrix = vmath::rotate(352.757f , 6.587f, 116.379f);//my location on earth
	SkyPBR_Shader->LoadEarthRotationXUniform( earth.GetEarthIndiaRotUVMatrix());//vmath::rotate(17.143f, 0.0f, 1.0f, 0.0f)*vmath::rotate(67.0f,1.0f,0.0f,0.0f)*
	SkyPBR_Shader->LoadEarthCloudRotationXUniform(earth.GetEarthCloudRotUVMatrix());
	//vec2
	double sunAngularSize = AtmoScatter->model().SunAngularRadius;
	SkyPBR_Shader->LoadSunSizeUniform(vmath::vec2(float(tan(sunAngularSize)), float(cos(sunAngularSize))));
	SkyPBR_Shader->LoadCloudScaleVisibilityUniform(Cloud2D_Scale_Visibility);
	//vec3
	SkyPBR_Shader->LoadEyePositionUniform(Cam.GetCameraPos());
	SkyPBR_Shader->LoadSunPositionUniform(ZzRenderer::SunPos);
	SkyPBR_Shader->LoadEarthCenterUniform(vmath::vec3(0.0f, GLfloat(-AtmoScatter->model().EarthRadius), 0.0f));
	SkyPBR_Shader->LoadColorPickerUniform(Tint);
	SkyPBR_Shader->LoadWaterTintUniform(WaterTint);
	//Sampler2D
	SkyPBR_Shader->LoadWorldSpacePositionTextureUniform(GL_TEXTURE0, 0, DefferedRenderer.GetTextureID_WorldSpacePosition());
	SkyPBR_Shader->LoadWorldSpaceNormalTextureUniform(GL_TEXTURE1, 1, DefferedRenderer.GetTextureID_WorldSpaceNormal());
	SkyPBR_Shader->LoadAlbedoAOTextureUniform(GL_TEXTURE2, 2, DefferedRenderer.GetTextureID_AlbedoAO());
	SkyPBR_Shader->LoadSpecRoughAlphaDitherTextureUniform(GL_TEXTURE3, 3, DefferedRenderer.GetTextureID_SpecRoughAlphaDither());
	SkyPBR_Shader->LoadDefferedVelocityTextureUniform(GL_TEXTURE4, 4, DefferedRenderer.GetTextureID_DefferedVelocity());

	AtmoScatter->model().SetProgramUniforms(SkyPBR_Shader->GetShaderProgram(), 5, 6, 7, 8);

	SkyPBR_Shader->LoadEarthAlbedoSpecTextureUniform(GL_TEXTURE9, 9, earth.getTextureID_EarthAlbedoSpec());
	SkyPBR_Shader->LoadEarthNormalTextureUniform(GL_TEXTURE10, 10, earth.getTextureID_EarthNormal());
	SkyPBR_Shader->LoadEarthCloudsTextureUniform(GL_TEXTURE11, 11,earth.getTextureID_EarthClouds());

	SkyPBR_Shader->LoadEarthLodBlendMapTextureUniform(GL_TEXTURE12, 12, earth.getTextureID_EarthLodBlendMap());
	SkyPBR_Shader->LoadEarthWaterNormalUniform(GL_TEXTURE13, 13, earth.getTextureID_EarthWaterNormalMap());

	//Post Process
	SkyPBR_Shader->LoadSunGlareTextureUniform(GL_TEXTURE14, 14, earth.getTextureID_MoonAlbedo());
	SkyPBR_Shader->LoadWhirlNoiseTextureUniform(GL_TEXTURE15, 15, noise.GetTextureID_WhirlNoise());
	SkyPBR_Shader->LoadBlueNoiseTextureUniform(GL_TEXTURE16, 16, noise.GetTextureID_BlueNoise());
	SkyPBR_Shader->LoadWhirlTextureUniform(GL_TEXTURE17, 17, noise.GetTextureID_WhirlTexture());
	SkyPBR_Shader->LoadCloudsTextureUniform(GL_TEXTURE18, 18, 0);
	SkyPBR_Shader->LoadCloudShadowMapUniform(GL_TEXTURE19, 19, clouds->GetCloudShadowMapTexture());//temporallyUpscaled

	//glBindImageTexture(0, temporalAA.GetCameraVelocityTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG32F);
	//glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glDrawArrays(GL_QUADS, 0, 4);
	SkyPBR_Shader->Stop();
}

void ZzRenderer::Render_BasicSkyPBR(void)
{
	this->BindSceneOfflineFbo();
	BasicSkyPBR_Shader->Start();
	//float
	BasicSkyPBR_Shader->LoadArtistFactor_Uniform(RayCaster::fUniformScale);
	BasicSkyPBR_Shader->LoadTime_Uniform(MyOpenGL::fTime);
	BasicSkyPBR_Shader->LoadExposure_Uniform(MyOpenGL::fExposure);
	//matrices
	BasicSkyPBR_Shader->LoadInvProjX_Uniform(MyWin32::gInvProjectionMatrix);
	BasicSkyPBR_Shader->LoadPrevViewProjX_Uniform(MyWin32::gProjectionMatrix * MyWin32::gPreviousViewMatrix);
	BasicSkyPBR_Shader->LoadInvViewX_Uniform(MyWin32::gInvViewMatrix);
	BasicSkyPBR_Shader->Loadm4LightViewProjX_Uniform(MyWin32::gLightViewProjectionMatrix);

	//vec2
	double sunAngularSize = AtmoScatter->model().SunAngularRadius;
	BasicSkyPBR_Shader->LoadSunSize_Uniform(vmath::vec2(float(tan(sunAngularSize)), float(cos(sunAngularSize))));
	//vec3
	BasicSkyPBR_Shader->LoadEyePosition_Uniform(Cam.GetCameraPos());
	BasicSkyPBR_Shader->LoadSunPosition_Uniform(ZzRenderer::SunPos);
	BasicSkyPBR_Shader->LoadEarthCenter_Uniform(vmath::vec3(0.0f, GLfloat(-AtmoScatter->model().EarthRadius), 0.0f));
	BasicSkyPBR_Shader->LoadColorPicker_Uniform(*clouds->Getv3SunColor());
	//Sampler2D
	BasicSkyPBR_Shader->LoadWorldSpacePositionTexture_Uniform(GL_TEXTURE0, 0, DefferedRenderer.GetTextureID_WorldSpacePosition());
	BasicSkyPBR_Shader->LoadWorldSpaceNormalTexture_Uniform(GL_TEXTURE1, 1, DefferedRenderer.GetTextureID_WorldSpaceNormal());
	BasicSkyPBR_Shader->LoadAlbedoAOTexture_Uniform(GL_TEXTURE2, 2, DefferedRenderer.GetTextureID_AlbedoAO());
	BasicSkyPBR_Shader->LoadSpecRoughAlphaDitherTexture_Uniform(GL_TEXTURE3, 3, DefferedRenderer.GetTextureID_SpecRoughAlphaDither());
	BasicSkyPBR_Shader->LoadDefferedVelocityTexture_Uniform(GL_TEXTURE4, 4, DefferedRenderer.GetTextureID_DefferedVelocity());

	AtmoScatter->model().SetProgramUniforms(BasicSkyPBR_Shader->GetShaderProgram(), 5, 6, 7, 8);

	//Post Process
	BasicSkyPBR_Shader->LoadBlueNoiseTexture_Uniform(GL_TEXTURE9, 9, noise.GetTextureID_BlueNoise());
	BasicSkyPBR_Shader->LoadCloudsTexture_Uniform(GL_TEXTURE10, 10, 0);
	BasicSkyPBR_Shader->LoadCloudShadowMap_Uniform(GL_TEXTURE11, 11, clouds->GetCloudShadowMapTexture());//temporallyUpscaled
	BasicSkyPBR_Shader->LoadFroxelTransmittance_Uniform(GL_TEXTURE12, 12, noise.GetTexureID_FroxelTransmittance());//
	BasicSkyPBR_Shader->LoadFroxelInScatter_Uniform(GL_TEXTURE13, 13, noise.GetTexureID_FroxelInScatter());//

	glBindImageTexture(0, temporalAA.GetCameraVelocityTexture(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG32F);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glDrawArrays(GL_QUADS, 0, 4);
	BasicSkyPBR_Shader->Stop();
}

void ZzRenderer::Render_Compositor(GLuint TextureID_Deffered, GLuint TextureID_Background, GLuint TextureID_Flare)
{
	BindFbo4k();

	CompositorShader.Start();

	CompositorShader.LoadSampler2D_Background_Uniform(GL_TEXTURE0, 0, TextureID_Background);
	CompositorShader.LoadSampler2D_Deffered_Uniform(GL_TEXTURE1, 1, TextureID_Deffered);
	CompositorShader.LoadSampler2D_Flare_Uniform(GL_TEXTURE2, 2, TextureID_Flare);
	CompositorShader.LoadSampler2D_Clouds_Uniform(GL_TEXTURE3, 3, clouds->GetCloudCurrFrameLinearTexture());
	CompositorShader.LoadSampler2D_FarClouds_Uniform(GL_TEXTURE4, 4, clouds->GetCloudCurrFrameFarCloudsTexture());
	CompositorShader.LoadSampler2D_Extra_Uniform(GL_TEXTURE5, 5, fboSceneOffline->GetFbo_Color_Texture(1));

#ifdef TEMPORAL_UPSCALE_8K
	CompositorShader.Loadv2InvResolution_Uniform(1.0f/vmath::vec2(3840.0f*2.0f,2160.0f*2.0f));
#else
	CompositorShader.Loadv2InvResolution_Uniform(1.0f / vmath::vec2(3840.0f, 2160.0f));
#endif // TEMPORAL_UPSCALE_8K


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	CompositorShader.Stop();
}

void ZzRenderer::Render_CompositorSpaceOffline(GLuint TextureID_Deffered, GLuint TextureID_Background, GLuint TextureID_Flare,GLboolean isFbo4k , GLfloat GroundSpaceFactorMultiplier)
{
#ifdef RECORD_DEMO
	BindFbo4k();
#else
//	BindDefaultFbo(vmath::uvec2(1920, 1080));
#endif // RECORD_DEMO

	GLfloat  EARTH_RADIUS = (GLfloat)AtmoScatter->model().EarthRadius;

	GLfloat sphereInnerRadius = EARTH_RADIUS * 0.0031446541f;
	GLfloat sphereOuterRadius = 2.0f * EARTH_RADIUS * 0.002f;

	GLfloat SPHERE_INNER_RADIUS = (EARTH_RADIUS + sphereInnerRadius);
	GLfloat SPHERE_OUTER_RADIUS = (SPHERE_INNER_RADIUS + sphereOuterRadius);
	GLfloat SPHERE_DELTA = (SPHERE_OUTER_RADIUS - SPHERE_INNER_RADIUS);

	vmath::vec3 EarthCenter = vmath::vec3(0.0, -EARTH_RADIUS, 0.0);

	GLfloat CameraHeightFraction = (vmath::length(Cam.GetCameraPos() - EarthCenter) - SPHERE_INNER_RADIUS) / (SPHERE_DELTA);

	CameraHeightFraction = vmath::clamp(CameraHeightFraction, 0.0f, 1.0f);

	CompositorSpaceShader.Start();

	CompositorSpaceShader.LoadSampler2D_Background_Uniform(GL_TEXTURE0, 0, TextureID_Background);
	CompositorSpaceShader.LoadSampler2D_Deffered_Uniform(GL_TEXTURE1, 1, TextureID_Deffered);
	CompositorSpaceShader.LoadSampler2D_Flare_Uniform(GL_TEXTURE2, 2, TextureID_Flare);
	CompositorSpaceShader.LoadSampler2D_Clouds_Uniform(GL_TEXTURE3, 3, clouds->GetCloudCurrFrameLinearTexture());
	CompositorSpaceShader.LoadSampler2D_FarClouds_Uniform(GL_TEXTURE4, 4, clouds->GetCloudCurrFrameFarCloudsTexture());
	CompositorSpaceShader.LoadSampler2D_Extra_Uniform(GL_TEXTURE5, 5, fboSceneOffline->GetFbo_Color_Texture(1));
	CompositorSpaceShader.LoadfMixSpaceGround_Uniform(CameraHeightFraction*(1.0f - GroundSpaceFactorMultiplier));
#ifdef TEMPORAL_UPSCALE_8K
	CompositorSpaceShader.Loadv2InvResolution_Uniform(1.0f / vmath::vec2(3840.0f * 2.0f, 2160.0f * 2.0f));
#else
	CompositorShader.Loadv2InvResolution_Uniform(1.0f / vmath::vec2(3840.0f, 2160.0f));
#endif // TEMPORAL_UPSCALE_8K


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	CompositorSpaceShader.Stop();
}

void ZzRenderer::Render_CompositorSpace(GLuint TextureID_Deffered, GLuint TextureID_Background, GLuint TextureID_Flare)
{
	BindDefaultFbo();

	CompositorSpaceShader.Start();

	CompositorSpaceShader.LoadSampler2D_Background_Uniform(GL_TEXTURE0, 0, TextureID_Background);
	CompositorSpaceShader.LoadSampler2D_Deffered_Uniform(GL_TEXTURE1, 1, TextureID_Deffered);
	CompositorSpaceShader.LoadSampler2D_Flare_Uniform(GL_TEXTURE2, 2, TextureID_Flare);
	CompositorSpaceShader.LoadSampler2D_Clouds_Uniform(GL_TEXTURE3, 3, clouds->GetCloudTemporalUpscaledTexture());
	CompositorSpaceShader.LoadSampler2D_FarClouds_Uniform(GL_TEXTURE4, 4, clouds->GetCloudTemporalUpscaledFrameExtra());
	CompositorSpaceShader.LoadSampler2D_Extra_Uniform(GL_TEXTURE5, 5, fboScene->GetFbo_Color_Texture(1));

#ifdef TEMPORAL_UPSCALE_8K
	CompositorSpaceShader.Loadv2InvResolution_Uniform(1.0f / vmath::vec2(3840.0f * 2.0f, 2160.0f * 2.0f));
#else
	CompositorSpaceShader.Loadv2InvResolution_Uniform(1.0f / vmath::vec2(3840.0f, 2160.0f));
#endif // TEMPORAL_UPSCALE_8K


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	CompositorSpaceShader.Stop();
}

void ZzRenderer::Render_DefferedSkyPBR(void)
{

	MyWin32::gNearFarFrustum = vmath::dvec2(10.0, 20000010.0);
	//All gBuffer Objects
	DefferedRenderer.BindDefferedFbo();
	{
		//this->Render_Terrain(MyWin32::gLightViewProjectionMatrix);
		//this->Render_TerrainLOD2();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		this->Render_EarthLODIndia(vmath::vec3(1.0f),1);
		this->Render_EarthLODEverest();
		this->Render_Axis3D();
		glDisable(GL_BLEND);
	}
	//CloudFbo
	clouds->RenderComputeCloudShadowMap((GLfloat)AtmoScatter->model().EarthRadius, Cam.GetLightViewMatrix(), this->SunWorldPos, noise.GetTextureID_PerlinWorley3D());
	this->Render_FinalClouds(RayCaster::fUniformScale, 0.0f, 1.0f, clouds->GetCloudWeatherMapTexture());//0.67 coverageVolume
//	this->Render_FinalQuad(clouds->GetCloudPrevShadowMapTexture());

	//Final Render
	this->Render_SkyPBR(5.0f, *clouds->Getv3SunColor(), vmath::vec3(1.0f), 0.0f, vmath::vec2(1.0));

	//TAA Velocity
	//this->BindFboVelocity();
	//temporalAA.CalculateVelocity(Cam.GetViewMatrix(), DefferedRenderer.GetTextureID_DepthBuffer(), noise.GetCurrentHalton23(), Cam.GetCameraPos());//remove *0.0f
	//temporalAA.CalculateMaxVelocity(fboVelocity->GetFbo_Color_Texture(0));
	temporalAA.CalculateMaxVelocity(temporalAA.GetCameraVelocityTexture());


	if (MyWin32::TAA)
	{
		this->BindFboTemporalAA();
		temporalAA.TemporalAntiAlias(Cam.GetViewMatrix(), fboScene->GetFbo_Color_Texture(0), DefferedRenderer.GetTextureID_DepthBuffer(), noise.GetCurrentHalton23(), noise.GetPreviousHalton23());//remove *0.0f
		temporalAA.ResolveToPreviousFrame(fboTemporalAA->GetFbo_Color_Texture(0), temporalAA.GetAntiAliasedTexture(), vmath::uvec2(MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight));

		this->Render_FinalQuad(temporalAA.GetAntiAliasedTexture());
	//	this->Render_FinalQuadBartWronskiDownsample(temporalAA.GetAntiAliasedTexture());
	//	this->Render_Compositor(temporalAA.GetAntiAliasedTexture(), fboScene->GetFbo_Color_Texture(1), fboPostProcess->GetFbo_Color_Texture(0));
	}
	else
	{
		//TAAU 
		this->BindFboTemporalUpscale4K();
		temporalAA.TemporalUpscale(Cam.GetViewMatrix(), fboScene->GetFbo_Color_Texture(0), 0, MyWin32::iBlockPixelID);
		temporalAA.ResolveToPreviousFrame(fboTemporalUpscale4k->GetFbo_Color_Texture(0), temporalAA.GetTemporalUpscaled4kTexture(), fboTemporalUpscale4k->GetFboResolution());

		//postProcessFbo
		this->Render_SunGlare(noise.GetTextureID_SunGlare(), ZzRenderer::ToSunVector * 1475700000.0f, 1700000000.0f, vmath::vec3(RayCaster::fUniformScale, Cam.GetPitch(), Cam.GetYaw()));

	//	this->Render_FinalQuadBartWronskiDownsample(temporalAA.GetTemporalUpscaled4kTexture());
	//	this->Render_FinalQuad(temporalAA.GetTemporalUpscaled4kTempTexture());
		this->Render_Compositor(temporalAA.GetTemporalUpscaled4kTexture(), fboSkyDistant->GetFbo_Color_Texture(0), fboPostProcess->GetFbo_Color_Texture(0));
	}


	//this->Render_Compositor(temporalAA.GetAntiAliasedTexture(), fboScene->GetFbo_Color_Texture(0), fboPostProcess->GetFbo_Color_Texture(0));
	//Test
	//this->Render_FinalQuad(DefferedRenderer.GetTextureID_DepthBuffer());
	//this->Render_FinalQuad(DefferedRenderer.GetTextureID_AlbedoAO());
}

//OpenCV

//void ZzRenderer::Render_TitleVideo(void)
//{
//	static bool astroDone = false;
//	static unsigned int localCounter = 0;
//
//	if (localCounter > 60)
//	{
//		if (astroDone == false)
//		{
//			astroDone = Render_Video("Resources/Video/Astromedicomp.mp4", 3);
//		}
//
//	}
//	localCounter += 1;
//}

//void ZzRenderer::Render_TestScene(HWND hwnd)
//{
//	
//
//	static GLint localCounter = 0;
//
//	ZzRenderer::sunAngle = 161.0f;
//
//	//Shadow------------------------------------------------------------
//	vmath::mat4 modelMatrix = vmath::mat4::identity();
//
//	vmath::mat4 lightView = vmath::lookat(-vmath::normalize(SunPos) * 7000.0f, vmath::vec3(0.0f, 0.0f, 0.0f), vmath::vec3(0.0f, 1.0f, 0.0f));
//	vmath::mat4 orthoGraphic = vmath::ortho(-5000.0f * 0.225f *3.0f, 5000.0f * 0.335f * 3.0f, -5000.0f * 0.041f * 3.0f, 5000.0f * 0.130f * 3.0f, 10000.0f, -10000.0f );
//	vmath::mat4 lightSpaceMatrix = orthoGraphic * lightView;
//
//	this->BindFboShadowMap();
//	//this->BindSceneFbo();
//	{
//		//GANESHA
//
//		modelMatrix = vmath::translate(vmath::vec3(4000.0f, -100.0f, 0.0f)) * vmath::rotate(RayCaster::fRotAngleZ, 0.0f, 0.0f, 1.0f) * vmath::rotate(RayCaster::fRotAngleY, 0.0f, 1.0f, 0.0f) * vmath::rotate(RayCaster::fRotAngleX, 1.0f, 0.0f, 0.0f) * vmath::scale(270.0f);
//
//		PBR_Shader.Start();
//
//		//matrices
//		PBR_Shader.LoadModelViewProjXUniform(lightSpaceMatrix * modelMatrix);
//
//		ganesha->Render();
//
//		PBR_Shader.Stop();
//
//
//		//RAT
//		rat_sleep.SetCurrentAnimation("rat_idle");
//		modelMatrix = vmath::translate(vmath::vec3(-1200.0f, 89.92f, 0.0f)) * vmath::rotate(RayCaster::fRotAngleZ, 0.0f, 0.0f, 1.0f) * vmath::rotate(280.734f, 0.0f, 1.0f, 0.0f) * vmath::rotate(RayCaster::fRotAngleX, 1.0f, 0.0f, 0.0f) * vmath::scale(100.257f);
//
//		PBR_AnimatedShader.Start();
//
//		PBR_AnimatedShader.LoadModelXUniform(modelMatrix);
//		PBR_AnimatedShader.LoadViewXUniform(lightView);
//		PBR_AnimatedShader.LoadProjXUniform(orthoGraphic);
//
//		std::map<std::string, AnimMesh>& sleep = rat_sleep.Get_m_meshesRef();
//
//		static GLdouble fTime_sleep = 10100.0f;
//
//		GLdouble Time = MyOpenGL::fDeltaTime * 1000.0;
//
//		//	static GLint sleepDelay = 0;
//
//		for (auto& mesh : sleep)
//		{
//			mesh.second.ComputeSkeletonTransforms(fTime_sleep, "rat_idle");
//			PBR_AnimatedShader.Loadm4JointTransformXUniform(mesh.second.GetJointMatricesPointer().get(), mesh.second.GetNumBones());
//			mesh.second.ZzRender();
//		}
//
//		fTime_sleep += Time * 0.25;
//
//
//	}
//
//
//	//SkyFbo
////	this->Render_SkyDome();
//
//	//Render SceneFbo
//	{
//
//		//this->BindSceneFbo();
//		this->BindFboMSAA8();
//	//	this->Render_Ground(0, vmath::vec3(0.983f, 1.000f, 0.912f), 0.0f, lightSpaceMatrix);
//		//rayCast
//		//if (FALSE == MyWin32::isGUI)
//		//{
//		//	myRayCaster.getWorldSpacePosFromScreenSpace(hwnd);
//		//}
//
//		//	this->Render_Mouse_RunStopIdle(vec3(0.0f), 0.0f, 1.0f,  14.0f*50.0f);
//		//	this->Render_Mouse_WalkIdle(vec3(0.0f), 0.0f, 1.0f, 20.0f* 50.0f);
//		//	this->Render_Mouse_IdleWalk(RayCaster::WorldPos, 280.734f, 1.257f, 560.0f* 1.0f);
//		this->Render_Mouse_Eat(vmath::vec3(-1200.0f, 89.92f, 0.0f), 280.734f, 100.257f, 14.0f * 20.0f);
//		this->Render_GaneshaPBR();
//
//	}
//
//	//CloudFbo
//	this->Render_FinalClouds(0.0f, 0.0f, 1.0f);//0.67 coverageVolume
//
//	MSAA8_toSceneFbo();
//
//	//PostProcess
////	this->Render_GodRays();
//
//	//SceneBlend
//	this->RenderSceneBlendPostProcessQuad(0.0f, lightSpaceMatrix);
//
//	//	this->PostProcess_toDefaultFbo(GL_COLOR_ATTACHMENT0);
//
//	this->BindDefaultFbo();
//
//
//	this->Render_PostProcessFboToBlack(0);//blackBlendFactorIn + blackBlendFactorOut
//
//	localCounter += 1;
//
//}

//void ZzRenderer::Render_Mouse_Eat(vmath::vec3 worldPos, GLfloat rotateY, GLfloat scale, GLfloat duration)
//{
//
//
//	rat_eat1.SetCurrentAnimation("rat_idle");
//
//	vmath::mat4 modelMatrix = vmath::mat4::identity();
//	modelMatrix = vmath::translate(worldPos) * vmath::rotate(RayCaster::fRotAngleZ, 0.0f, 0.0f, 1.0f) * vmath::rotate(rotateY, 0.0f, 1.0f, 0.0f) * vmath::rotate(RayCaster::fRotAngleX, 1.0f, 0.0f, 0.0f) * vmath::scale(scale);
//
//	PBR_AnimatedShader.Start();
//
//	PBR_AnimatedShader.LoadModelXUniform(modelMatrix);
//	PBR_AnimatedShader.LoadViewXUniform(Cam.GetViewMatrix());
//	PBR_AnimatedShader.LoadProjXUniform(MyWin32::gProjectionMatrixForDepth);
//
//	std::map<std::string, AnimMesh>& eat = rat_eat1.Get_m_meshesRef();
//
//	PBR_AnimatedShader.LoadAlbedoTextureUniform(GL_TEXTURE0, 0, rat_shading.getAlbedoMapTextureID());
//	PBR_AnimatedShader.LoadNormalTextureUniform(GL_TEXTURE1, 1, rat_shading.getNormalMapTextureID());
//	PBR_AnimatedShader.LoadAmbientOcclusionTextureUniform(GL_TEXTURE2, 2, rat_shading.getAmbientOcclusionMapTextureID());
//	PBR_AnimatedShader.LoadRoughnessTextureUniform(GL_TEXTURE3, 3, rat_shading.getRoughnessMapTextureID());
//
//	PBR_AnimatedShader.LoadEyePositionUniform(Cam.GetCameraPos());
//	PBR_AnimatedShader.LoadSunPositionUniform(this->SunPos);
//
//	PBR_AnimatedShader.LoadAmbientFactorUniform(0.3f);
//
//	GLdouble SunTheta = vmath::dot(vmath::normalize(this->SunPos), vec3(0.0f, 1.0f, 0.0f));
//	PBR_AnimatedShader.LoadLightColorUniform(terrain.getLightColor(SunTheta));
//
//	static GLdouble fTime_eat = 0.0f;
//
//	GLdouble Time = MyOpenGL::fDeltaTime * 1000.0;
//
//	{
//		for (auto& mesh : eat)
//		{
//			mesh.second.ComputeSkeletonTransforms(fTime_eat, "rat_idle");
//			PBR_AnimatedShader.Loadm4JointTransformXUniform(mesh.second.GetJointMatricesPointer().get(), mesh.second.GetNumBones());
//			mesh.second.ZzRender();
//		}
//
//		fTime_eat += Time;
//	}
//
//}

//FrameBuffer
void ZzRenderer::ResizeFramebuffers(GLsizei Width, GLsizei Height)
{
	if (MyWin32::bRecentResize)
	{
		if (clouds)
		clouds->ResizeFramebuffers(Width, Height);

		if(fboMSAA)
		fboMSAA->ResizeFramebuffer(Width, Height);

		if (fboMSAA8)
		fboMSAA8->ResizeFramebuffer(Width, Height);

		if (fboMSAA16)
		fboMSAA16->ResizeFramebuffer(Width, Height);

	//	if(fboScene)
	//	fboScene->ResizeFramebuffer(Width, Height);

		if(fboSkyDistant)
		fboSkyDistant->ResizeFramebuffer(Width, Height);

		if (fboTemporalAA)
		fboTemporalAA->ResizeFramebuffer(Width, Height);

		if(fboPostProcess)
		fboPostProcess->ResizeFramebuffer(Width, Height);

		if(fboVelocity)
		fboVelocity->ResizeFramebuffer(Width, Height);

		//radialBlur.ResizeFramebuffers(Width, Height);
	}
	MyWin32::bRecentResize = FALSE;
}

void ZzRenderer::ResolveFBO(GLuint src, GLuint dst,GLenum readBuffer)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, src);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst);
	glReadBuffer(readBuffer);
	glDrawBuffer(readBuffer);
	glBlitFramebuffer(0, 0, 3840, 2160, 0, 0, 3840, 2160, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	//glBlitFramebuffer(0, 0, 3840, 2160, 0, 0, 3840, 2160, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void ZzRenderer::UpdateSunPosition(void)
{
	//static float sunAngle = 0.0f;
	//static float sunAngle = 10.0f;

//	sunAngle += MyOpenGL::fDeltaTime *1.5f;//speedMultiplier

	if (ZzRenderer::sunAngle > 360.0f)
	{
		ZzRenderer::sunAngle -= ZzRenderer::sunAngle;
	}

	//Pi/180=0.01745329251

	ZzRenderer::SunPos[0] = 0.0f;
	ZzRenderer::SunPos[1] = sinf(sunAngle * 0.01745329251f) * 100000000.f;
	ZzRenderer::SunPos[2] = cosf(sunAngle * 0.01745329251f) * 100000000.f;

	//SunPos = SunPos / vmath::length(SunPos);
	//SunPos = vmath::normalize(SunPos);
	ZzRenderer::ToSunVector = vmath::normalize(ZzRenderer::SunPos);
	ZzRenderer::SunWorldPos = (ToSunVector * 200000.0f);
}

void ZzRenderer::UpdateScreenSunPosition(void)
{
	vmath::vec4 ScreenSun = vmath::vec4(this->SunPos,1.0f) *(MyWin32::gViewProjectionMatrix).transpose();

	ScreenSun[3] = 1.0f / ScreenSun[3];

	this->ScreenSunPos[0] = (ScreenSun[0] * ScreenSun[3]) * 0.5f + 0.5f;
	this->ScreenSunPos[1] = (ScreenSun[1] * ScreenSun[3]) * 0.5f + 0.5f;
}

//void ZzRenderer::RenderSceneBlendPostProcessQuad(GLfloat fVignette,vmath::mat4 lightSpaceMatrix)
//{
//	glBindFramebuffer(GL_FRAMEBUFFER, fboPostProcess->GetFbo());
//	glClear(GL_COLOR_BUFFER_BIT );
//
//	SceneBlendPostProcessShader.Start();
//
//	//vectors
//	{
//		SceneBlendPostProcessShader.Loadv3SunPos_Uniform(this->SunPos);
//		SceneBlendPostProcessShader.Loadv3EyeDir_Uniform(this->Cam.GetCameraDirection());
//		SceneBlendPostProcessShader.Loadv3EyePos_Uniform(this->Cam.GetCameraPos());
//	}
//	//matrices
//	{
//		SceneBlendPostProcessShader.Loadm4InvProjX_Uniform(vmath::inverse(MyWin32::gProjectionMatrix));
//		SceneBlendPostProcessShader.Loadm4InvViewX_Uniform(vmath::inverse(Cam.GetViewMatrix()));
//		SceneBlendPostProcessShader.Loadm4LightSpaceX_Uniform(lightSpaceMatrix);
//	}
//
//	//scene
//	{
//		//clouds
//		SceneBlendPostProcessShader.LoadSampler2D_CloudTextureUniform(GL_TEXTURE0, 0, clouds->GetCloudTemporalUpscaledTexture());//full res frame
//		//sky
//		SceneBlendPostProcessShader.LoadSampler2D_SkyTextureUniform(GL_TEXTURE1, 1, fboSky->GetFbo_Color_Texture(0));
//		//scene
//		SceneBlendPostProcessShader.LoadSampler2D_SceneTextureUniform(GL_TEXTURE2, 2, fboScene->GetFbo_Color_Texture(0));
//		//sceneMask
//		SceneBlendPostProcessShader.LoadSampler2D_SceneMaskTextureUniform(GL_TEXTURE3, 3, fboScene->GetFbo_Color_Texture(1));
//	}
//	//postProcess
//	{
//		SceneBlendPostProcessShader.LoadSampler2D_GodRaysUniform(GL_TEXTURE4, 4,radialBlur.GetTextureID_RadialBlur());
//	}
//	//extra
//	{
//		SceneBlendPostProcessShader.LoadSampler2D_ShadowMapTextureUniform(GL_TEXTURE5, 5, clouds->GetCloudShadowMapTexture());//clouds.GetCloudTemporalUpscaledDepth() //fboShadowMap->GetFbo_Depth_Texture()
//		SceneBlendPostProcessShader.LoadSampler2D_DepthTextureUniform(GL_TEXTURE6, 6, fboScene->GetFbo_Depth_Texture());//clouds.GetCloudTemporalUpscaledDepth()
//		SceneBlendPostProcessShader.LoadfVignette_Uniform(RayCaster::fUniformScale);
//	}
//	glDrawArrays(GL_QUADS, 0, 4);
//	SceneBlendPostProcessShader.Stop();
//}

void ZzRenderer::GetLastFrameData(void)
{
	MyWin32::gPreviousViewMatrix = Cam.GetViewMatrix();
	MyWin32::gPreviousInfiniteViewMatrix = Cam.GetInfiniteViewMatrix();
	MyWin32::gPrevCameraPosition = Cam.GetCameraPos();
	MyWin32::gPreviousJitteredProjectionMatrix = MyWin32::gJitteredProjectionMatrix;
	noise.SetPreviousHalton23(noise.GetCurrentHalton23());
}

	//FRAMEBUFFER BIND
void ZzRenderer::BindDefaultFbo(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ZzRenderer::BindDefaultFbo(uvec2 WidthHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WidthHeight[0], WidthHeight[1]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ZzRenderer::BindFboTemporalAA(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboTemporalAA->GetFbo());
	glViewport(0, 0, MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void ZzRenderer::BindFboTemporalUpscale4K(void)
{
	uvec2 res = fboTemporalUpscale4k->GetFboResolution();
	glBindFramebuffer(GL_FRAMEBUFFER, fboTemporalUpscale4k->GetFbo());
	glViewport(0, 0, res[0], res[1]);
	glClear(GL_COLOR_BUFFER_BIT);
}

void ZzRenderer::BindFboPostProcess(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboPostProcess->GetFbo());
	glViewport(0, 0, MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void ZzRenderer::BindFboVelocity(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboVelocity->GetFbo());
	glViewport(0, 0, MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight);
	glClear(GL_COLOR_BUFFER_BIT );
}

void ZzRenderer::BindFbo4k(void)
{
	uvec2 res = fbo4k->GetFboResolution();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo4k->GetFbo());
	glViewport(0, 0, res[0], res[1]);
	glClear(GL_COLOR_BUFFER_BIT );

	fbo4k->DrawBuffers();
}

void ZzRenderer::BindFbo4kExtra(void)
{
	uvec2 res = fbo4kExtra->GetFboResolution();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo4kExtra->GetFbo());
	glViewport(0, 0, res[0], res[1]);
	glClear(GL_COLOR_BUFFER_BIT );

	fbo4kExtra->DrawBuffers();
}

void ZzRenderer::BindFbo4kFinal(void)
{
	uvec2 res = fbo4kFinal->GetFboResolution();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo4kFinal->GetFbo());
	glViewport(0, 0, res[0], res[1]);
	glClear(GL_COLOR_BUFFER_BIT );

	fbo4kFinal->DrawBuffers();
}

void ZzRenderer::BindSceneFbo(void)
{
	uvec2 res = fboScene->GetFboResolution();
	glBindFramebuffer(GL_FRAMEBUFFER, fboScene->GetFbo());
	glViewport(0, 0, res[0], res[1]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	fboScene->DrawBuffers();
}

void ZzRenderer::BindSceneOfflineFbo(void)
{
	uvec2 res = fboSceneOffline->GetFboResolution();
	glBindFramebuffer(GL_FRAMEBUFFER, fboSceneOffline->GetFbo());
	glViewport(0, 0, res[0], res[1]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	fboSceneOffline->DrawBuffers();
}
void ZzRenderer::BindFboMSAA(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboMSAA->GetFbo());
	glViewport(0, 0, MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	fboMSAA->DrawBuffers();
}
void ZzRenderer::BindFboMSAA8(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboMSAA8->GetFbo());
	glViewport(0, 0, MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	fboMSAA8->DrawBuffers();
}
void ZzRenderer::BindFboMSAA16(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboMSAA16->GetFbo());
	glViewport(0, 0, MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	fboMSAA16->DrawBuffers();
}
void ZzRenderer::BindFboSkyDistant(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboSkyDistant->GetFbo());
	glViewport(0, 0, MyWin32::myClientSize.ClientWidth, MyWin32::myClientSize.ClientHeight);
	glClear(GL_COLOR_BUFFER_BIT );
}

void ZzRenderer::BindFboShadowMap(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboShadowMap->GetFbo());
	glViewport(0, 0, 8192, 8192);
	glClear( GL_DEPTH_BUFFER_BIT);
}



void ZzRenderer::Render_All(HWND hwnd)
{
	//ZzRenderer::sunAngle = 2.661f;

	//Shadow
	//this->BindFboShadowMap();
	{
	//	this->Render_TerrainBasic3D(GL_TRUE);
	//	this->Render_DefferedSpeedTreeInstanced(GL_TRUE);
	}

	//SkyFbo
	//this->Render_SkyDome();

	//Render SceneFbo
	{
		//this->BindFboMSAA();
	//	this->BindSceneFbo();
	//	this->Render_Terrain(MyWin32::gLightViewProjectionMatrix);

	//	this->Render_GaneshaPBR();

		//rayCast
		//if (FALSE == MyWin32::isGUI)
		//{
		//	myRayCaster.getWorldSpacePosFromScreenSpace(hwnd);
		//}

		RayCaster::WorldPos;

		//	this->Render_Mouse_RunStopIdle(vec3(0.0f), 0.0f, 1.0f,  14.0f*50.0f);
		//	this->Render_Mouse_WalkIdle(vec3(0.0f), 0.0f, 1.0f, 20.0f* 50.0f);
		//	this->Render_Mouse_IdleWalk(RayCaster::WorldPos, 280.734f, 1.257f, 560.0f* 1.0f);
		//	this->Render_Mouse_Eat(vmath::vec3(-4284.73242f, 9.06565762f, -3435.67896f), 280.734f, 1.257f, 1.0f);

			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//this->Render_Water(0.07f);
			//glDisable(GL_BLEND);

		//	this->Render_ModelsPBR();
		//	this->Render_GaneshaPBR();

	}

	//MAIN DEMO RECORDING

//	if (MyWin32::iFrameID < (1398))
//	{
//		//this->Render_PaleBlueDotSceneFarEarthOffline(hwnd, GL_TRUE);//((21.8+1) * 60) = 1368
//		//this->Render_PaleBlueDotSceneFirstOffline(hwnd, GL_TRUE);//(38.7-21.8) = (16.9*60) = 1014
//		//this->Render_PaleBlueDotSceneSecondOffline(hwnd, GL_TRUE);//(48.6-37.7) = (10.9*60) = 654
//		//this->Render_PaleBlueDotSceneTeacherOffline(hwnd, GL_TRUE);//(67.2-47.6) = (19.6*60) = 1176
//		//this->Render_PaleBlueDotSceneBloodRiverOffline(hwnd, GL_TRUE);//(91.2-66.2) = (25*60) = 1500
//		//this->Render_PaleBlueDotSceneFractionOffline(hwnd, GL_TRUE);//(112.3-90.2) = (22.1*60) = 1326
//		//this->Render_PaleBlueDotSceneDelusionOffline(hwnd, GL_TRUE);//(134.8-111.3) = (23.5*60) = 1410
//		//this->Render_PaleBlueDotSceneLonelySpeckOffline(hwnd, GL_TRUE);//(147.2-133.8) = (13.4*60) =804
//		//this->Render_PaleBlueDotSceneLifeOffline(hwnd);//(170.2-146.2) = (24*60) = 1440
// 	   this->Render_PaleBlueDotSceneAfterLifeOffline(hwnd);//(192.5-169.2) = 23.3*60 = 1398
// 	   //	this->Render_PaleBlueDotSceneFinalSceneOffline(hwnd);//FinalScene //(840+540)
// 	   //	this->Render_PaleBlueDotCreditSceneOffline(hwnd);//Credit //(1020) 13+4
//
//	
//#ifdef RECORD_DEMO
//		//OfflineRender
//		{
//			glReadnPixels(0, 0, 3840, 2160, GL_BGR, GL_UNSIGNED_BYTE, sizeof(GL_UNSIGNED_BYTE) * 3840 * 2160 * 3, img_First->ptr());
//			cv::flip(*img_First, *Frame_First, 0);
//			VideoWriter.write(*Frame_First);
//		}
//#endif // RECORD_DEMO
//
//		Render_FinalQuad(fbo4k->GetFbo_Color_Texture(0));
//	}


//	this->Render_PaleBlueDotSceneDelusionOffline(hwnd, GL_TRUE);//AfterLifeScene
//	this->Render_PaleBlueDotScenePlaneCloudsOffline(hwnd,GL_TRUE);//LifeScene
//	this->Render_PaleBlueDotSceneFinalSceneOffline(hwnd);//FinalScene
//	this->Render_PaleBlueDotSceneAfterLifeOffline(hwnd);
//	this->Render_FinalQuad(fbo4k->GetFbo_Color_Texture(0));
//	this->Render_FinalQuad(clouds->GetCloudShadowMapTexture());


//	Render_FinalQuad(fbo4k->GetFbo_Color_Texture(0));

	//this->Render_PaleBlueDotBeginSceneOffline(hwnd);
	//this->Render_PaleBlueDotSceneFirstOffline(hwnd);
	//this->Render_PaleBlueDotSceneSecondOffline(hwnd);
	//this->Render_PaleBlueDotSceneTeacherOffline(hwnd);
//	this->Render_PaleBlueDotSceneBloodRiverOffline(hwnd);
//	this->Render_PaleBlueDotSceneFarEarthOffline(hwnd);
//	this->Render_PaleBlueDotSceneFractionOffline(hwnd);
//	this->Render_PaleBlueDotSceneDelusionOffline(hwnd,0);
//	this->Render_PaleBlueDotScenePlaneCloudsOffline(hwnd);

	//this->Render_DefferedSkyPBR();//**************************************************************************IMP
	//this->Render_FinalQuad(DefferedRenderer.GetTextureID_DepthBuffer());

	//DepthTest
	//BindDefaultFbo();
	//this->Render_Axis3D();

	this->Render_CreatePaleBlueDotVideo();
	//BindFbo4k();
	//Render_TwoTextureBlendQuad(0, 0,0.0f, noise.GetTextureID_BlueNoise());
	//this->Render_FinalQuad(fbo4k->GetFbo_Color_Texture(0));
}
