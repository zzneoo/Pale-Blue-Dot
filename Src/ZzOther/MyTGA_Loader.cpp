#include "stdafx.h"
#include "MyTGA_Loader.h"


MyTGA_Loader::MyTGA_Loader()
{
}


MyTGA_Loader::~MyTGA_Loader()
{
}


MyTGA_Loader::TexData MyTGA_Loader::loadTGAdata(const char* fileName)
{

	FILE* tgaPtr = NULL;//for data
	FILE* tgaLog = NULL;//for Log

	TexData tDataInfo;
	GLubyte* data;

	TGA_Header th;
	ZeroMemory(&th, sizeof(th));

	if (fopen_s(&tgaLog, "../Log/TGAlog.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("TgaLog File Can Not Be Created\nExitting ..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		tDataInfo.success = GL_FALSE;
		return(tDataInfo);
	}
	else
	{
		if(tgaLog)
		fprintf(tgaLog, "TgaLog File Is Successfully Opened.\n");
	}


	if (fopen_s(&tgaPtr, fileName, "rb") != 0)
	{
		fprintf(tgaLog, "\n%s:TGA file pointer not found\n", fileName);
		MessageBox(NULL, TEXT(" TGA file pointer not found\nExitting ..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		tDataInfo.success = GL_FALSE;
		return(tDataInfo);
	}

	if(tgaPtr)
	fread(&th, sizeof(th), 1, tgaPtr);//TGA header

	if (th.ImageType != 2 && th.ImageType != 3 || th.ImageWidth <= 0 || th.ImageHeight <= 0)
	{
		if (tgaPtr)
		{
			fclose(tgaPtr);
			tgaPtr = NULL;
		}
		if(tgaLog)
		fprintf(tgaLog, "\n%s:WRONG TGA format type..\n", fileName);
		MessageBox(NULL, TEXT(" WRONG TGA format type..\nExitting ..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		tDataInfo.success = GL_FALSE;
		return(tDataInfo);
	}
	if (tgaLog)
	fprintf(tgaLog, "\nID_Length=%d \nColorMapType= %d\nImageType= %d\nfirstEntryIndex=%d\ncolorMapLength=%d\ncolorMapEntrySize=%d\nxOrigin=%d\nyOrigin=%d\nImageWidth=%d\nImageHeight =%d\nPixelDepth=%d\nImageDescriptor=%d\n", th.ID_Length, th.ColorMapType, th.ImageType,th.firstEntryIndex[1],th.colorMapLength[1],th.colorMapEntrySize,th.xOrigin,th.yOrigin,th.ImageWidth,th.ImageHeight,th.PixelDepth,th.ImageDescriptor);

	if (th.PixelDepth == 8 )
	{
		tDataInfo.internalFormatType = GL_RED;
		tDataInfo.enumFormatType = GL_RED;
	}
	else if (th.PixelDepth == 24)
	{
		tDataInfo.internalFormatType = GL_RGB;
		tDataInfo.enumFormatType = GL_BGR;
	}
	else if (th.PixelDepth == 32)
	{
		tDataInfo.internalFormatType = GL_RGBA;
		tDataInfo.enumFormatType = GL_BGRA;
	}
	else
	{
		if (tgaPtr)
		{
			fclose(tgaPtr);
			tgaPtr = NULL;
		}
		if (tgaLog)
		fprintf(tgaLog, "\n%s:WRONG TGA PixelDepth..\n", fileName);
		MessageBox(NULL, TEXT(" WRONG TGA PixelDepth..\nExitting ..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		tDataInfo.success = GL_FALSE;
		return(tDataInfo);
	}
	tDataInfo.iWidth = (th.ImageWidth);
	tDataInfo.iHeight = (th.ImageHeight);


	GLuint bytesPerPixel = th.PixelDepth / 8;
	GLuint bufferSize = th.ImageWidth * th.ImageHeight * bytesPerPixel * sizeof(GLubyte);
	if (tgaLog)
	fprintf(tgaLog, "\nWidth X height ==%d X %d \n,PixelDepth= %d \n,buffer size=%d\n", th.ImageHeight, th.ImageWidth, th.PixelDepth, bufferSize);

	data = new GLubyte[bufferSize];

	if (!data)
	{
		if (tgaPtr)
		{
			fclose(tgaPtr);
			tgaPtr = NULL;
		}
		
		fprintf(tgaLog, "\n%s:No memory for data found..\n", fileName);
		MessageBox(NULL, TEXT("No memory for data found\nExitting ..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		tDataInfo.success = GL_FALSE;
		return(tDataInfo);
	}
	if (tgaPtr)
	{
		fseek(tgaPtr, sizeof(th), SEEK_SET);
		fread(data, bufferSize, 1, tgaPtr);
	}

	if (tgaPtr)
	{
		fclose(tgaPtr);
		tgaPtr = NULL;
	}
	//if (tgaLog)
	//fprintf(tgaLog, "\n\nlast element=%d\n", data[bufferSize]);

	if (tgaLog)
	{
		fprintf(tgaLog, "TgaLog File Is Successfully Closed.\n");
		fclose(tgaLog);
		tgaLog = NULL;
	}
	tDataInfo.data = (data);
	tDataInfo.success = GL_TRUE;
	return(tDataInfo);
}

GLuint MyTGA_Loader::LoadRaw16_texture(const char* fileName, GLushort width, GLushort height)
{
	GLuint texture;

	FILE* rawPtr = NULL;//data
	FILE* rawLog = NULL;//log

	GLubyte* data = NULL;

	GLuint bufferSize = width * height * 2 * sizeof(GLubyte);

	if (fopen_s(&rawLog, "../Log/Raw16log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Raw16log File Can Not Be Created\nExitting ..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
	}
	else
	{
		if (rawLog)
			fprintf(rawLog, "rawLog File Is Successfully Opened.\n");
	}

	if (fopen_s(&rawPtr, fileName, "rb") != 0)
	{
		if (rawLog)
		fprintf(rawLog, "\n%s:Raw file pointer not found\n", fileName);
		MessageBox(NULL, TEXT(" Taw file pointer not found\nExitting ..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
	}

	data = new GLubyte[bufferSize];

	if (!data)
	{
		if (rawPtr)
		{
			fclose(rawPtr);
			rawPtr = NULL;
		}

		fprintf(rawLog, "\n%s:No memory for data found..\n", fileName);
		MessageBox(NULL, TEXT("No memory for data found\nExitting ..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
	}

	if (rawPtr)
	{
		fseek(rawPtr, 0, SEEK_SET);
		fread(data, bufferSize, 1, rawPtr);
	}

	if (rawPtr)
	{
		fclose(rawPtr);
		rawPtr = NULL;
	}

	if (rawLog)
	{
		fprintf(rawLog, "rawLog File Is Successfully Closed.\n");
		fclose(rawLog);
		rawLog = NULL;
	}


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 2);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, width, height, 0, GL_RED, GL_UNSIGNED_SHORT, (const void*)data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (data)
	{
		delete[] data;
		data = NULL;
	}
	return texture;
}

GLuint MyTGA_Loader::LoadTGA_texture(const char* fileName, bool bAnisotropicFilter, GLenum filterMode, bool bRepeat)
{
	GLuint texture;
	TexData myTextureData;

	myTextureData = loadTGAdata(fileName);

	if (myTextureData.success == GL_FALSE)
	{
		return 0;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, myTextureData.internalFormatType, myTextureData.iWidth, myTextureData.iHeight, 0, myTextureData.enumFormatType, GL_UNSIGNED_BYTE, (const void*)myTextureData.data);

	//cleanUp data
	if (myTextureData.data)
	{
		delete[] myTextureData.data;
		myTextureData.data = NULL;
	}

	if (bAnisotropicFilter == true)
	{
		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_LOD_BIAS, 0);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, fLargest);

	}

	if (bRepeat)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	}



	if (filterMode == GL_NEAREST)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else if (filterMode == GL_LINEAR)
	{
		glGenerateTextureMipmap(texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
		//glGenerateMipmap(GL_TEXTURE_2D);	//can use glGenerateMipmap(GL_TEXTURE_2D) too..
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}


	


	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

//dont remember why kept this
//GLuint LoadTGA_texture(char* fileName, bool bAnisotropicFilter)
//{
//	TexData loadTGAdata(char* Name);
//	GLuint texture;
//	TexData myTextureData;
//
//	myTextureData = loadTGAdata(fileName);
//
//	glGenTextures(1, &texture);
//	glBindTexture(GL_TEXTURE_2D, texture);
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	glTexImage2D(GL_TEXTURE_2D, 0, myTextureData.internalFormatType, myTextureData.iWidth, myTextureData.iHeight, 0, myTextureData.enumFormatType, GL_UNSIGNED_BYTE, (const void*)myTextureData.data);
//
//	if (bAnisotropicFilter == true)
//	{
//		GLfloat fLargest;
//		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &fLargest);
//		glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_LOD_BIAS, 0);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, fLargest);
//
//	}
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
////
////	if (myTextureData.bGreyScale)
////	{
////		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
////	}
////	else
////	{
////		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////		//glGenerateTextureMipmap(texture);//can use glGenerateMipmap(GL_TEXTURE_2D) too..
//////								 //glGenerateMipmap(GL_TEXTURE_2D);
////		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
////	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
////	}
//
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	//if (myTextureData.data)
//	//{
//	//	delete[] myTextureData.data;
//	//	myTextureData.data = NULL;
//	//}
//
//	return texture;
//}

GLuint MyTGA_Loader::loadCubeMapTextures(char* cubeMapTextures[])
{
	GLuint testTextureCube;
	glGenTextures(1, &testTextureCube);
	glBindTexture(GL_TEXTURE_CUBE_MAP, testTextureCube);

	for (int i = 0; i < 6; i++)
	{
		TexData cubeMapData = loadTGAdata(cubeMapTextures[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, cubeMapData.internalFormatType, cubeMapData.iWidth, cubeMapData.iHeight, 0, cubeMapData.enumFormatType, GL_UNSIGNED_BYTE, cubeMapData.data);

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return testTextureCube;
}