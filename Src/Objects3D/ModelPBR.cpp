#include "ModelPBR.h"
#include "MyOpenGL.h"

ModelPBR::ModelPBR(const char* filePath)
{
	indicesCount = 0;
	if (true == MyOpenGL::bObject3DInitStatus)
	{
		MyOpenGL::bObject3DInitStatus = MyObjLoader(filePath);
		if (true == MyOpenGL::bObject3DInitStatus)
		{
			CreateInterleavedData();
			LoadVao();
		}
	}
	LoadObject3DTextures();
}

bool ModelPBR::MyObjLoader(const char* filePath)
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


		int res=0;
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

void ModelPBR::MyTangentLoader(std::vector<vec3> vertices, std::vector<vec2> uvs)
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

void ModelPBR::CreateInterleavedData(void)
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

void ModelPBR::LoadObject3DTextures(void)
{
	textureID_Albedo = MyTGA_Loader::LoadTGA_texture("Resources/Models/ganesha/albedo.tga",true,GL_LINEAR,false);
	textureID_Normal = MyTGA_Loader::LoadTGA_texture("Resources/Models/ganesha/normal.tga",true,GL_LINEAR,false);
	textureID_AmbientOcclusion = MyTGA_Loader::LoadTGA_texture("Resources/Models/ganesha/AO.tga", true,GL_LINEAR,false);
	textureID_Roughness = MyTGA_Loader::LoadTGA_texture("Resources/Models/ganesha/roughness.tga",true,GL_LINEAR,false);

}
void ModelPBR ::LoadVao(void)
{
	GLsizei stride = (sizeof(vec3) * 3) + sizeof(vec2);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelInterleavedData.size() * sizeof(GLfloat), (const GLvoid*)&modelInterleavedData[0], GL_STATIC_DRAW);

	glVertexAttribPointer(ZZNEO_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glVertexAttribPointer(ZZNEO_ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, stride, (void*)12);
	glVertexAttribPointer(ZZNEO_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, (void*)20);
	glVertexAttribPointer(ZZNEO_ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE, stride, (void*)32);

	glEnableVertexAttribArray(ZZNEO_ATTRIB_POSITION);
	glEnableVertexAttribArray(ZZNEO_ATTRIB_TEXCOORD);
	glEnableVertexAttribArray(ZZNEO_ATTRIB_NORMAL);
	glEnableVertexAttribArray(ZZNEO_ATTRIB_TANGENT);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenBuffers(1, &vbo_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelIndices.size() * sizeof(GLuint), (const GLvoid*)&modelIndices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	indicesCount = (GLuint)modelIndices.size();
	modelIndices.clear();
	modelInterleavedData.clear();
}

void ModelPBR::Render(void)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element);
	glDrawElements(GL_TRIANGLES, indicesCount,GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ModelPBR::~ModelPBR()
{
	vertices.clear();
	texCoords.clear();
	normals.clear();
	tangents.clear();

	modelInterleavedData.clear();
	modelIndices.clear();
}
