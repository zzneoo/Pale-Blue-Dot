#pragma once
class MyTGA_Loader
{
public:
	MyTGA_Loader();
	~MyTGA_Loader();

	typedef struct tagTgaHeader
	{

		GLbyte  ID_Length;
		GLbyte  ColorMapType;
		GLbyte  ImageType;
		// Color map specifications
		GLbyte firstEntryIndex[2];
		GLbyte colorMapLength[2];

		GLbyte colorMapEntrySize;

		//image specification
		GLshort xOrigin;
		GLshort yOrigin;
		GLshort ImageWidth;
		GLshort ImageHeight;
		GLbyte  PixelDepth;
		GLbyte ImageDescriptor;

	}TGA_Header;

	typedef struct TextureData
	{
		GLubyte* data;
		GLenum internalFormatType, enumFormatType;
		GLsizei iWidth, iHeight;
		GLboolean success;
	}TexData;

	static TexData loadTGAdata(const char* fileName);
	static GLuint LoadTGA_texture(const char* fileName, bool bAnisotropicFilter,GLenum filterMode, bool bRepeat);
	static GLuint LoadRaw16_texture(const char* fileName,GLushort width,GLushort height);
	static GLuint loadCubeMapTextures(char* cubeMapTextures[]);

};

