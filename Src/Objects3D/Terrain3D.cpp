#include "Terrain3D.h"

Terrain3D::Terrain3D()
{
	terrainScale = 5.0f;
	//IndicesCount = 6 * (MeshWidth - 1)* (MeshHeight - 1);//for triangles
	IndicesCount = 4 * (MeshWidth - 1)* (MeshHeight - 1);//for quads

	glPatchParameteri(GL_PATCH_VERTICES, 4);//tesselation
	//LoadVao();
	LoadObject3DTextures();

	//SetModelMatrix(vmath::vec3(-terrainScale * 0.5f, 0.0f, -terrainScale*0.5f), 0.0f, vmath::vec3(1.0f, 0.0f, 0.0f), vmath::vec3(terrainScale));
	SetModelMatrix(vmath::vec3(0.0f, 0.0f, 0.0f), 0.0f, vmath::vec3(1.0f, 0.0f, 0.0f), vmath::vec3(terrainScale));
}

Terrain3D::~Terrain3D()
{
	//grass
	{
		if (textureID_Grass)
		{
			glDeleteTextures(1, &textureID_Grass);
			textureID_Grass = 0;
		}
		if (textureID_GrassNormal)
		{
			glDeleteTextures(1, &textureID_GrassNormal);
			textureID_GrassNormal = 0;
		}
		if (textureID_GrassRoughnes)
		{
			glDeleteTextures(1, &textureID_GrassRoughnes);
			textureID_GrassRoughnes = 0;
		}

		if (textureID_GrassAO)
		{
			glDeleteTextures(1, &textureID_GrassAO);
			textureID_GrassAO = 0;
		}
		if (textureID_GrassDisplacement)
		{
			glDeleteTextures(1, &textureID_GrassDisplacement);
			textureID_GrassDisplacement = 0;
		}
	}
	//Sand
	{
		if (textureID_Sand)
		{
			glDeleteTextures(1, &textureID_Sand);
			textureID_Sand = 0;
		}
		if (textureID_SandNormal)
		{
			glDeleteTextures(1, &textureID_SandNormal);
			textureID_GrassNormal = 0;
		}
		if (textureID_SandRoughnes)
		{
			glDeleteTextures(1, &textureID_SandRoughnes);
			textureID_SandRoughnes = 0;
		}

		if (textureID_SandAO)
		{
			glDeleteTextures(1, &textureID_SandAO);
			textureID_SandAO = 0;
		}
		if (textureID_SandDisplacement)
		{
			glDeleteTextures(1, &textureID_SandDisplacement);
			textureID_SandDisplacement = 0;
		}
	}
	//Rock
	{
		if (textureID_Rock)
		{
			glDeleteTextures(1, &textureID_Rock);
			textureID_Rock = 0;
		}
		if (textureID_RockNormal)
		{
			glDeleteTextures(1, &textureID_RockNormal);
			textureID_RockNormal = 0;
		}
		if (textureID_RockRoughnes)
		{
			glDeleteTextures(1, &textureID_RockRoughnes);
			textureID_RockRoughnes = 0;
		}
		if (textureID_RockAO)
		{
			glDeleteTextures(1, &textureID_RockAO);
			textureID_RockAO = 0;
		}
		if (textureID_RockDisplacement)
		{
			glDeleteTextures(1, &textureID_RockDisplacement);
			textureID_RockDisplacement = 0;
		}
	}
	//talus
	{
		if (textureID_Talus)
		{
			glDeleteTextures(1, &textureID_Talus);
			textureID_Talus = 0;
		}
		if (textureID_TalusNormal)
		{
			glDeleteTextures(1, &textureID_TalusNormal);
			textureID_TalusNormal = 0;
		}
		if (textureID_TalusRoughnes)
		{
			glDeleteTextures(1, &textureID_TalusRoughnes);
			textureID_TalusRoughnes = 0;
		}
		if (textureID_TalusAO)
		{
			glDeleteTextures(1, &textureID_TalusAO);
			textureID_TalusAO = 0;
		}
		if (textureID_TalusDisplacement)
		{
			glDeleteTextures(1, &textureID_TalusDisplacement);
			textureID_TalusDisplacement = 0;
		}
	}
	//maps
	{
		if (textureID_LandSWTR)
		{
			glDeleteTextures(1, &textureID_LandSWTR);
			textureID_LandSWTR = 0;
		}
		if (textureID_WaterRBWD)
		{
			glDeleteTextures(1, &textureID_WaterRBWD);
			textureID_WaterRBWD = 0;
		}
		if (textureID_WaterGradient)
		{
			glDeleteTextures(1, &textureID_WaterGradient);
			textureID_WaterGradient = 0;
		}
		if (textureID_Height)
		{
			glDeleteTextures(1, &textureID_Height);
			textureID_Height = 0;
		}
		if (textureID_Normal)
		{
			glDeleteTextures(1, &textureID_Normal);
			textureID_Normal = 0;
		}
	}
}

void Terrain3D::LoadVao(void)
{
	vmath::vec3* Data = NULL;
	GLuint*indices = NULL;

	GLsizei verticesCount = MeshWidth * MeshHeight;

	GLsizei vertexBufferSize = verticesCount * sizeof(vmath::vec3);
	GLsizei IndexBufferSize = IndicesCount * sizeof(GLuint);



	//	Data = (vmath::vec3*)malloc(dataSize);
	Data = new vmath::vec3[verticesCount];
	indices = new GLuint[IndicesCount];

	GLuint i, j;
	GLfloat x, y;

	for (i = 0, y=0.0f; i < MeshHeight; i++, y += (1.0f / MeshHeight))
	{
		for (j = 0, x = 0.0f; j < MeshWidth; j++, x += ((1.0f / MeshWidth)))
		{
			GLuint index = i * MeshWidth + j;
			Data[index][0] = x;
			Data[index][1] = 0.0f;
			Data[index][2] = y;

			
		}
		
	}


	int counter = 0;
	for (int i = 0,j=0; i < IndicesCount-1; i+=4,j+=1) //i+=6 for triangles,i+=4 for quads
	{
	
	//Tiangles
		//for z negative
			//indices[i + 0] = MeshWidth + j + 1;
			//indices[i + 1] = j;
			//indices[i + 2] = j + 1;

			//indices[i + 3] = MeshWidth + j + 1;
			//indices[i + 4] = MeshWidth + j;
			//indices[i + 5] = j;

		//for z positive
			//indices[i + 0] =  j + 1;
			//indices[i + 1] = MeshWidth + j;
			//indices[i + 2] = MeshWidth + j + 1;

			//indices[i + 3] = j + 1;
			//indices[i + 4] = j;
			//indices[i + 5] = MeshWidth + j;


	//Quads
		//for z positive
			indices[i + 0] =  j + 1;
			indices[i + 1] = j;
			indices[i + 2] = MeshWidth + j;
			indices[i + 3] = MeshWidth + j + 1;


		counter += 1;

		if (counter / (MeshWidth - 1) == 1)
		{
			j += 1;
			counter = 0;
		}
		
	}
	//vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//vbo
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, (const GLvoid *)Data, GL_STATIC_DRAW);

	glVertexAttribPointer(ZZNEO_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(ZZNEO_ATTRIB_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//vbo_element
	glGenBuffers(1, &vbo_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, (const GLvoid *)indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (Data)
	{
		delete[] Data;
		Data = NULL;
	}
	if (indices)
	{
		delete[] indices;
		indices = NULL;
	}
}

void Terrain3D::Render(void)
{

//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//	glBindVertexArray(vao);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element);
	//glDrawElements(GL_TRIANGLES, IndicesCount, GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_PATCHES, IndicesCount, GL_UNSIGNED_INT, 0);
	glDrawArraysInstanced(GL_PATCHES, 0, 4, 1024 * 1024);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);

//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Terrain3D::LoadObject3DTextures(void)
{
	//textureID_Height = MyTGA_Loader::LoadTGA_texture(terrainHeightMapTextureFilename,true, GL_LINEAR, false);
	textureID_Height = MyTGA_Loader::LoadRaw16_texture(terrainHeightMapTextureFilename, 8192, 8192);
	textureID_Normal = MyTGA_Loader::LoadTGA_texture(terrainNormalTextureFilename, true, GL_LINEAR, false);

	textureID_WaterGradient= MyTGA_Loader::LoadTGA_texture(terrainWaterGradientTextureFilename, false, GL_LINEAR, false);

	textureID_LandSWTR = MyTGA_Loader::LoadTGA_texture(terrainLandSWTR_TextureFilename, true, GL_LINEAR, false);
	textureID_WaterRBWD = MyTGA_Loader::LoadTGA_texture(terrainWaterRBWD_TextureFilename, true, GL_LINEAR, false);

	//grass
	{
		textureID_Grass = MyTGA_Loader::LoadTGA_texture(terrainGrassTextureFilename, true, GL_LINEAR, true);
		textureID_GrassAO = MyTGA_Loader::LoadTGA_texture(terrainGrassAOTextureFilename, true, GL_LINEAR, true);
		textureID_GrassNormal = MyTGA_Loader::LoadTGA_texture(terrainGrassNormalTextureFilename, true, GL_LINEAR, true);
		textureID_GrassRoughnes = MyTGA_Loader::LoadTGA_texture(terrainGrassRoughnessTextureFilename, true, GL_LINEAR, true);
		textureID_GrassDisplacement = MyTGA_Loader::LoadTGA_texture(terrainGrassDisplacementTextureFilename, true, GL_LINEAR, true);
	}

	//sand
	{
		textureID_Sand = MyTGA_Loader::LoadTGA_texture(terrainSandTextureFilename, true, GL_LINEAR, true);
		textureID_SandAO = MyTGA_Loader::LoadTGA_texture(terrainSandAO_TextureFilename, true, GL_LINEAR, true);
		textureID_SandNormal = MyTGA_Loader::LoadTGA_texture(terrainSandNormalTextureFilename, true, GL_LINEAR, true);
		textureID_SandRoughnes = MyTGA_Loader::LoadTGA_texture(terrainSandRoughnessTextureFilename, true, GL_LINEAR, true);
		textureID_SandDisplacement = MyTGA_Loader::LoadTGA_texture(terrainSandDisplacementTextureFilename, true, GL_LINEAR, true);
	}
	//rock
	{
		textureID_Rock = MyTGA_Loader::LoadTGA_texture(terrainRockTextureFilename, true, GL_LINEAR, true);
		textureID_RockAO = MyTGA_Loader::LoadTGA_texture(terrainRockAO_TextureFilename, true, GL_LINEAR, true);
		textureID_RockNormal = MyTGA_Loader::LoadTGA_texture(terrainRockNormalTextureFilename, true, GL_LINEAR, true);
		textureID_RockRoughnes = MyTGA_Loader::LoadTGA_texture(terrainRockRoughnessTextureFilename, true, GL_LINEAR, true);
		textureID_RockDisplacement = MyTGA_Loader::LoadTGA_texture(terrainRockDisplacementTextureFilename, true, GL_LINEAR, true);
	}
	//talus
	{
		textureID_Talus = MyTGA_Loader::LoadTGA_texture(terrainTalusTextureFilename, true, GL_LINEAR, true);
		textureID_TalusAO = MyTGA_Loader::LoadTGA_texture(terrainTalusAO_TextureFilename, true, GL_LINEAR, true);
		textureID_TalusNormal = MyTGA_Loader::LoadTGA_texture(terrainTalusNormalTextureFilename, true, GL_LINEAR, true);
		textureID_TalusRoughnes = MyTGA_Loader::LoadTGA_texture(terrainTalusRoughnessTextureFilename, true, GL_LINEAR, true);
		textureID_TalusDisplacement = MyTGA_Loader::LoadTGA_texture(terrainTalusDisplacementTextureFilename, true, GL_LINEAR, true);
	}
}

vmath::vec3 Terrain3D::getLightColor(GLdouble sunTheta)
{
	vmath::vec3 first = vmath::mix(vmath::vec3(0.240f, 0.045f, 0.004f), vmath::vec3(0.343f, 0.131f, 0.000f), (float)smoothstep(0.0f, 0.1f, sunTheta));//sunTheta

	vmath::vec3 second = vmath::mix(first, vmath::vec3(0.897f, 0.786f, 0.519f), (float)smoothstep(0.1f, 0.164f, sunTheta));

	vmath::vec3 last = vmath::mix(second, vmath::vec3(1.0f), (float)smoothstep(0.164f, 0.528f, sunTheta));

	return last;
}
