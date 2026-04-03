#include "CullIndirect.h"

CullIndirect::CullIndirect()
{
	//Candidate Draw
	GetPerInstanceData();

	glGenBuffers(1, &buffers.parameters);
	glBindBuffer(GL_PARAMETER_BUFFER, buffers.parameters);
	glBufferStorage(GL_PARAMETER_BUFFER, 256, nullptr, 0);

	glGenBuffers(1, &buffers.drawCandidates);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers.drawCandidates);

	//const GLuint size = GLuint(myPerInstanceData.size());
	const GLuint size = 1024;
	pDraws = new CandidateDraw[size];

	for (GLuint i = 0; i < size; i++)
	{
		pDraws[i].sphereCenter = vmath::vec3(0.0f);
		pDraws[i].sphereRadius = 1.0f;
		pDraws[i].firstVertex = 0;
		pDraws[i].vertexCount =  20886;
	}

	glBufferStorage(GL_SHADER_STORAGE_BUFFER, size * sizeof(CandidateDraw), pDraws, 0);
	delete[] pDraws;

	//Buffers
	glGenBuffers(1, &buffers.drawCommands);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers.drawCommands);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, size * sizeof(DrawArraysIndirectCommand), nullptr, GL_MAP_READ_BIT);

	glGenBuffers(1, &buffers.modelMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, buffers.modelMatrices);
	glBufferStorage(GL_UNIFORM_BUFFER, size * sizeof(vmath::mat4), nullptr, GL_MAP_WRITE_BIT);

	// Bind model matrix UBO and fill with data
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, buffers.modelMatrices);
	 pModelMatrix = (vmath::mat4*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, size * sizeof(vmath::mat4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (GLuint i = 0; i < myPerInstanceData.size(); i++)
	{
		//float f = float(i) / 127.0f +  0.025f;
		//float g = float(i) / 127.0f;
		//const vmath::mat4 model_matrix = vmath::translate(10000.0f*vmath::vec3(sinf(f * 3.0f), cosf(f * 5.0f), cosf(f * 9.0f) - 2.0f)) *
		//	vmath::rotate( 140.0f, vmath::normalize(vmath::vec3(sinf(g * 35.0f), cosf(g * 75.0f), cosf(g * 39.0f))));
		pModelMatrix[i] = vmath::translate(myPerInstanceData[i].WorldPos)*vmath::rotate(myPerInstanceData[i].fRotAngleZ,0.0f,0.0f,1.0f) * vmath::rotate(myPerInstanceData[i].fRotAngleY, 0.0f, 1.0f, 0.0f) * vmath::rotate(myPerInstanceData[i].fRotAngleX, 1.0f, 0.0f, 0.0f)*vmath::scale(myPerInstanceData[i].fUniformScale);
	}
//	myPerInstanceData.clear();

	glUnmapBuffer(GL_UNIFORM_BUFFER);

}

CullIndirect::~CullIndirect()
{
	myPerInstanceData.clear();
}

void CullIndirect::GetPerInstanceData(void)
{
	GLchar ch[256];
	FILE* fp = NULL;

	if (fopen_s(&fp, "treeData.zz", "r") != 0)
	{
		MessageBox(NULL, TEXT("PerInstanceData Can Not Be Loaded\nExitting ..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		//	return false;
	}


	while (1)
	{
		int res = 0;
		if (fp)
		{
			res = fscanf_s(fp, "%s", ch, (GLuint)_countof(ch));
			if (res == EOF)
				break;


			if (strcmp(ch, "Z") == 0)
			{
				PerInstanceData temp;

				vmath::vec3 WorldPos;
				GLfloat fRotAngleX;
				GLfloat fRotAngleY;
				GLfloat fRotAngleZ;
				GLfloat fUniformScale;

				fscanf_s(fp, "%f %f %f %f %f %f %f\n", &WorldPos[0], &WorldPos[1], &WorldPos[2], &fRotAngleX, &fRotAngleY, &fRotAngleZ, &fUniformScale);

				temp.WorldPos = WorldPos;
				temp.fRotAngleX = fRotAngleX;
				temp.fRotAngleY = fRotAngleY;
				temp.fRotAngleZ = fRotAngleZ;
				temp.fUniformScale = fUniformScale;

				myPerInstanceData.push_back(temp);
			}
		}
	}
	if (fp)
	{
		fclose(fp);
		fp = nullptr;
	}
}