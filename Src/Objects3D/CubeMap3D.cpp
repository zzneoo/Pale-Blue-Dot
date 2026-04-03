#include "CubeMap3D.h"

CubeMap3D::CubeMap3D()
{
	textureID_Jungle = 0;

	LoadVao();
	LoadObject3DTextures();
}
CubeMap3D::~CubeMap3D()
{

	if (textureID_Jungle)
	{
		glDeleteTextures(1, &textureID_Jungle);
		textureID_Jungle = 0;
	}
}

void CubeMap3D::LoadVao(void)
{
	const GLfloat CubeSize = 1000;
	const GLfloat vertices[108] =
	{
		//rt
		CubeSize, -CubeSize, -CubeSize,
		CubeSize, -CubeSize,  CubeSize,
		CubeSize,  CubeSize , CubeSize,
		CubeSize,  CubeSize,  CubeSize,
		CubeSize,  CubeSize, -CubeSize,
		CubeSize, -CubeSize, -CubeSize,

		//left
		-CubeSize, -CubeSize,  CubeSize,
		-CubeSize, -CubeSize, -CubeSize,
		-CubeSize,  CubeSize, -CubeSize,
		-CubeSize,  CubeSize, -CubeSize,
		-CubeSize,  CubeSize,  CubeSize,
		-CubeSize, -CubeSize,  CubeSize,


		//up
		-CubeSize,  CubeSize, -CubeSize,
		CubeSize,  CubeSize, -CubeSize,
		CubeSize,  CubeSize , CubeSize,
		CubeSize,  CubeSize,  CubeSize,
		-CubeSize,  CubeSize,  CubeSize,
		-CubeSize,  CubeSize, -CubeSize,

		//dn
		-CubeSize, -CubeSize, -CubeSize,
		-CubeSize, -CubeSize,  CubeSize,
		CubeSize, -CubeSize ,-CubeSize,
		CubeSize, -CubeSize, -CubeSize,
		-CubeSize, -CubeSize,  CubeSize,
		CubeSize, -CubeSize,  CubeSize,


		//ft
		-CubeSize, -CubeSize,  CubeSize,
		-CubeSize,  CubeSize,  CubeSize,
		CubeSize,  CubeSize,  CubeSize,
		CubeSize,  CubeSize,  CubeSize,
		CubeSize, -CubeSize,  CubeSize,
		-CubeSize, -CubeSize,  CubeSize,

		//bk
		-CubeSize,  CubeSize, -CubeSize,
		-CubeSize, -CubeSize, -CubeSize,
		CubeSize, -CubeSize, -CubeSize,
		CubeSize, -CubeSize, -CubeSize,
		CubeSize,  CubeSize, -CubeSize,
		-CubeSize,  CubeSize, -CubeSize


	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(ZZNEO_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(ZZNEO_ATTRIB_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//const unsigned int  triangleIndices[] =
	//{ 0,1,2};

	//glGenBuffers(GL_ELEMENT_ARRAY_BUFFER, &vbo_element);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element);

	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CubeMap3D::Render(void)
{
	glDepthMask(GL_FALSE);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

}

void CubeMap3D::LoadObject3DTextures(void)
{
	char* CubeMapTextureFileNames[6];
	char* CubeMapTextureJungleFileNames[6];

	CubeMapTextureJungleFileNames[0] = (char*)"Resources/Textures/CubeMap/jungle/posx.tga";
	CubeMapTextureJungleFileNames[1] = (char*)"Resources/Textures/CubeMap/jungle/negx.tga";
	CubeMapTextureJungleFileNames[2] = (char*)"Resources/Textures/CubeMap/jungle/posy.tga";
	CubeMapTextureJungleFileNames[3] = (char*)"Resources/Textures/CubeMap/jungle/negy.tga";
	CubeMapTextureJungleFileNames[4] = (char*)"Resources/Textures/CubeMap/jungle/posz.tga";
	CubeMapTextureJungleFileNames[5] = (char*)"Resources/Textures/CubeMap/jungle/negz.tga";

	CubeMapTextureFileNames[0] = (char*)"Resources/Textures/CubeMap/posx.tga";
	CubeMapTextureFileNames[1] = (char*)"Resources/Textures/CubeMap/negx.tga";
	CubeMapTextureFileNames[2] = (char*)"Resources/Textures/CubeMap/posy.tga";
	CubeMapTextureFileNames[3] = (char*)"Resources/Textures/CubeMap/negy.tga";
	CubeMapTextureFileNames[4] = (char*)"Resources/Textures/CubeMap/posz.tga";
	CubeMapTextureFileNames[5] = (char*)"Resources/Textures/CubeMap/negz.tga";

	textureID_Albedo = MyTGA_Loader::loadCubeMapTextures(CubeMapTextureFileNames);
	textureID_Jungle = MyTGA_Loader::loadCubeMapTextures(CubeMapTextureJungleFileNames);
}
