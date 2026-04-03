#pragma once
#include"stdafx.h"
class PostProcessor
{
public:
	PostProcessor();
	virtual ~PostProcessor();

	GLuint GetTextureID(void)
	{
		return TextureID;
	}

protected:
	GLuint TextureID;
};

