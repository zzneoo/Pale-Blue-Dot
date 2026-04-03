#include "ModelPBR_SpeedTreeInstanced.h"
#include "MyOpenGL.h"

ModelPBR_SpeedTreeInstanced::ModelPBR_SpeedTreeInstanced(const char* filePath)
{
	indicesCount = 0;

	MyInstanceModelData.maxInstancesCount = 12000;

	MyInstanceModelData.InstanceModelMatrices = new vmath::mat4[MyInstanceModelData.maxInstancesCount];

	if (true == MyOpenGL::bObject3DInitStatus)
	{
		MyOpenGL::bObject3DInitStatus = MyObjLoader(filePath);
		if (true == MyOpenGL::bObject3DInitStatus)
		{
			CreateInterleavedData();
			LoadVao();
		}
	}

	MyInstanceModelData.CurrentInstanceCount = GetPerInstanceData();//AfterLoadVao
	//MyInstanceModelData.CurrentInstanceCount = 1;//Test

	LoadObject3DTextures();

	//for (GLuint i = 0; i < MyInstanceModelData.maxInstancesCount; i++)
	//{
	//	MyInstanceModelData.InstanceModelMatrices[i] = vmath::rotate(42.0f, 0.0f, 1.0f, 0.0f)* vmath::translate(vmath::vec3((((float)rand() / RAND_MAX)*128.0f)-64.0f,0.0, (((float)rand() / RAND_MAX)*128.0f)-64.0f)) * vmath::rotate(0.0f, 0.0f, 0.0f, 1.0f) * vmath::rotate(((float)rand() / RAND_MAX)*360.0f, 0.0f, 1.0f, 0.0f) * vmath::rotate(270.0f, 1.0f, 0.0f, 0.0f) * vmath::scale(0.002f + ((float)rand() / RAND_MAX) * 0.0005f) ;
	//	MyInstanceModelData.CurrentInstanceCount += 1;
	//}

}

bool ModelPBR_SpeedTreeInstanced::MyObjLoader(const char* filePath)
{
	GLchar ch[256];

	vector< GLuint > vertexIndices;
	vector< GLuint > texCoordIndices;
	vector< GLuint > normalIndices;

	vector< vec3 > temp_vertices;
	vector< vec2 > temp_texCoords;
	vector< vec3 > temp_normals;

	GLuint faceCnt = 0;

	GLfloat x, y, z;

	FILE* objFilePtr = NULL;


	if (fopen_s(&objFilePtr, filePath, "r") != 0)
	{
		MessageBox(NULL, TEXT("OBJ File Can Not Be Loaded\nExitting ..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		return false;
	}

	while (1)
	{
		int res = 0;
		res = fscanf_s(objFilePtr, "%s", ch, (GLuint)_countof(ch));
		if (res == EOF)
			break;


		if (strcmp(ch, "v") == 0)
		{
			vec3 vertex;

			fscanf_s(objFilePtr, "%f %f %f\n", &x, &y, &z);
			vertex = vec3(x, y, z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(ch, "vt") == 0)
		{
			vec2 texCoord;

			fscanf_s(objFilePtr, "%f %f \n", &x, &y);
			texCoord = vec2(x, y);
			temp_texCoords.push_back(texCoord);

		}
		else if (strcmp(ch, "vn") == 0)
		{
			vec3 normal;

			fscanf_s(objFilePtr, "%f %f %f\n", &x, &y, &z);
			normal = vec3(x, y, z);
			temp_normals.push_back(normal);

		}

		else if (strcmp(ch, "f") == 0)
		{
			GLuint vertexIndex[3], uvIndex[3], normalIndex[3];
			if (objFilePtr)
			{
				fscanf_s(objFilePtr, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			texCoordIndices.push_back(uvIndex[0]);
			texCoordIndices.push_back(uvIndex[1]);
			texCoordIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);

			faceCnt += 1;

		}
	}

	if (objFilePtr)
	{
		fclose(objFilePtr);
		objFilePtr = NULL;
	}

	if (temp_texCoords.size() == 0)
	{
		MessageBox(NULL, TEXT("No UVs found\nExitting ..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		return false;
	}

	if (temp_normals.size() == 0)
	{
		MessageBox(NULL, TEXT("No Normals found\nExitting ..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		return false;
	}
	if (temp_vertices.size() == 0)
	{
		MessageBox(NULL, TEXT("No Vertices found\nExitting ..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		return false;
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		GLuint vertexIndexFix = vertexIndices[i];
		GLuint texCoordIndexFix = texCoordIndices[i];
		GLuint normalIndexFix = normalIndices[i];

		modelIndices.push_back(i);
		vec3 vertex = temp_vertices[vertexIndexFix - 1];
		vec2 uv = temp_texCoords[texCoordIndexFix - 1];
		vec3 normal = temp_normals[normalIndexFix - 1];

		vertices.push_back(vertex);
		texCoords.push_back(uv);
		normals.push_back(normal);

	}
	//----------------------------------------------------------------------------------------//


	//Tangents
	MyTangentLoader(vertices, texCoords); //tangent loaderrr

	return true;
}

void ModelPBR_SpeedTreeInstanced::MyTangentLoader(std::vector<vec3> vertices, std::vector<vec2> uvs)
{
	//TANGENT

	vec3 DeltaPos1;
	vec3  DeltaPos2;
	vec2  DeltaUV1;
	vec2  DeltaUV2;
	vec3 PerVerTangent;

	float r = 0.0f;
	int cnt = 0;
	int noOfTriangles = (int)(vertices.size() / 3);

	for (int i = 0; i < noOfTriangles; i++)
	{

		DeltaPos1 = vertices[cnt + 1] - vertices[cnt];
		DeltaPos2 = vertices[cnt + 2] - vertices[cnt];

		DeltaUV1 = uvs[cnt + 1] - uvs[cnt];
		DeltaUV2 = uvs[cnt + 2] - uvs[cnt];

		if ((DeltaUV1[0] * DeltaUV2[1] - DeltaUV1[1] * DeltaUV2[0]) == 0.00f)//divide by zero
		{
			r = 1.0f;
		}
		else
		{
			r = 1.0f / (DeltaUV1[0] * DeltaUV2[1] - DeltaUV1[1] * DeltaUV2[0]);
		}

		PerVerTangent = (DeltaPos1 - DeltaPos2);     //LHS-RHS

		for (int i = 0; i < 3; i++)
		{
			PerVerTangent[i] = r * (DeltaPos1[i] * DeltaUV2[1] - DeltaPos2[i] * DeltaUV1[1]);
		}

		PerVerTangent = vmath::normalize(PerVerTangent);

		for (int i = 0; i < 3; i++)
		{

			tangents.push_back(PerVerTangent);
		}

		cnt += 3;
	}

}

void ModelPBR_SpeedTreeInstanced::CreateInterleavedData(void)
{

	for (size_t i = 0; i < vertices.size(); i++)//change '<' to '!='
	{

		modelInterleavedData.push_back(vertices[i][0]);
		modelInterleavedData.push_back(vertices[i][1]);
		modelInterleavedData.push_back(vertices[i][2]);
		modelInterleavedData.push_back(texCoords[i][0]);
		modelInterleavedData.push_back(texCoords[i][1]);
		modelInterleavedData.push_back(normals[i][0]);
		modelInterleavedData.push_back(normals[i][1]);
		modelInterleavedData.push_back(normals[i][2]);
		modelInterleavedData.push_back(tangents[i][0]);
		modelInterleavedData.push_back(tangents[i][1]);
		modelInterleavedData.push_back(tangents[i][2]);


	}
	vertices.clear();
	texCoords.clear();
	normals.clear();
	tangents.clear();
}

void ModelPBR_SpeedTreeInstanced::LoadObject3DTextures(void)
{
	textureID_Albedo = MyTGA_Loader::LoadTGA_texture("Resources/Textures/SpeedTree/BroadLeaf_Billboard/Albedo.tga", true, GL_LINEAR, true);
	textureID_Normal = MyTGA_Loader::LoadTGA_texture("Resources/Textures/SpeedTree/BroadLeaf_Billboard/Normal.tga", true, GL_LINEAR, true);
	textureID_Subsurface = MyTGA_Loader::LoadTGA_texture("Resources/Textures/SpeedTree/BroadLeaf_Billboard/Subsurface.tga", true, GL_LINEAR, true);
}

void ModelPBR_SpeedTreeInstanced::LoadVao(void)
{
	GLsizei stride = (sizeof(vec3) * 3) + sizeof(vec2);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelInterleavedData.size() * sizeof(GLfloat), (const GLvoid*)&modelInterleavedData[0], GL_STATIC_DRAW);

	glVertexAttribPointer(ZZNEO_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glVertexAttribPointer(ZZNEO_ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat)*3));
	glVertexAttribPointer(ZZNEO_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * 5));
	glVertexAttribPointer(ZZNEO_ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * 8));

	glEnableVertexAttribArray(ZZNEO_ATTRIB_POSITION);
	glEnableVertexAttribArray(ZZNEO_ATTRIB_TEXCOORD);
	glEnableVertexAttribArray(ZZNEO_ATTRIB_NORMAL);
	glEnableVertexAttribArray(ZZNEO_ATTRIB_TANGENT);

	MyInstanceModelData.InstanceModelMatrices[0] = vmath::mat4::identity();

	glGenBuffers(1, &vboPerInstanceModelX);
	glBindBuffer(GL_ARRAY_BUFFER, vboPerInstanceModelX);
	glBufferData(GL_ARRAY_BUFFER, MyInstanceModelData.maxInstancesCount *sizeof(GLfloat)*16, (const GLvoid*)&MyInstanceModelData.InstanceModelMatrices[0], GL_DYNAMIC_DRAW);

	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 16, (void*)0);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 16, (void*)(sizeof(GLfloat) * 4));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 16, (void*)(sizeof(GLfloat) * 8));
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 16, (void*)(sizeof(GLfloat) * 12));

	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnableVertexAttribArray(7);

	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &vbo_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelIndices.size() * sizeof(GLuint), (const GLvoid*)&modelIndices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	indicesCount = (GLuint)modelIndices.size();
	modelIndices.clear();
	modelInterleavedData.clear();
}

void ModelPBR_SpeedTreeInstanced::Render(void)
{

	glBindBuffer(GL_ARRAY_BUFFER, vboPerInstanceModelX);
	glBufferData(GL_ARRAY_BUFFER, MyInstanceModelData.maxInstancesCount * sizeof(GLfloat) * 16, (const GLvoid*)&MyInstanceModelData.InstanceModelMatrices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 16, (void*)0);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 16, (void*)(sizeof(GLfloat) * 4));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 16, (void*)(sizeof(GLfloat) * 8));
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 16, (void*)(sizeof(GLfloat) * 12));

	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnableVertexAttribArray(7);

	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element);
	glDrawElementsInstanced(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0,(GLsizei)MyInstanceModelData.CurrentInstanceCount);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GLuint ModelPBR_SpeedTreeInstanced::GetPerInstanceData(void)
{
	GLchar ch[256];
	FILE* fp = NULL;

	if (fopen_s(&fp, "treeData.zz", "r") != 0)
	{
		MessageBox(NULL, TEXT("PerInstanceData Can Not Be Loaded\nExitting ..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		//	return false;
	}

	GLuint Counter = 0;

	while (1)
	{
		int res = 0;
		if (fp)
		{
			res = fscanf_s(fp, "%s", ch, (GLuint)_countof(ch));
			if (res == EOF)
				break;


			if (strcmp(ch, "Z") == 0 )
			{

				vmath::vec3 WorldPos = vmath::vec3(0.0f);
				GLfloat fRotAngleX = 0.0f;
				GLfloat fRotAngleY = 0.0f;
				GLfloat fRotAngleZ = 0.0f;
				GLfloat fUniformScale = 0.0f;

				fscanf_s(fp, "%f %f %f %f %f %f %f\n", &WorldPos[0], &WorldPos[1], &WorldPos[2], &fRotAngleX, &fRotAngleY, &fRotAngleZ, &fUniformScale);

				MyInstanceModelData.InstanceModelMatrices[Counter] = vmath::translate(WorldPos) * vmath::rotate(RayCaster::fRotAngleZ, 0.0f, 0.0f, 1.0f) * vmath::rotate(fRotAngleY, 0.0f, 1.0f, 0.0f) * vmath::rotate(fRotAngleX, 1.0f, 0.0f, 0.0f) * vmath::scale(fUniformScale);
				

				Counter += 1;
			}
		}
	}
	if (fp)
	{
		fclose(fp);
		fp = nullptr;
	}
	return Counter;
}

ModelPBR_SpeedTreeInstanced::~ModelPBR_SpeedTreeInstanced()
{
	vertices.clear();
	texCoords.clear();
	normals.clear();
	tangents.clear();

	modelInterleavedData.clear();
	modelIndices.clear();

	if (MyInstanceModelData.InstanceModelMatrices)
	{
		delete [] MyInstanceModelData.InstanceModelMatrices;
	}
}



