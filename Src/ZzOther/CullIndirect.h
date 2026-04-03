#pragma once
#include"stdafx.h"

struct MyBuffer
{
	GLuint          parameters;
	GLuint          drawCandidates;
	GLuint          drawCommands;
	GLuint          modelMatrices;
	GLuint          transforms;
} ;

struct CandidateDraw
{
	vmath::vec3 sphereCenter;
	float sphereRadius;
	unsigned int firstVertex;
	unsigned int vertexCount;
	unsigned int : 32;
	unsigned int : 32;
};

struct DrawArraysIndirectCommand
{
	GLuint vertexCount;
	GLuint instanceCount;
	GLuint firstVertex;
	GLuint baseInstance;
};

struct PerInstanceData
{
	vmath::vec3 WorldPos;
	GLfloat fRotAngleX;
	GLfloat fRotAngleY;
	GLfloat fRotAngleZ;
	GLfloat fUniformScale;
};

class CullIndirect
{
public:
	CullIndirect();
	~CullIndirect();

	size_t GetCandidateCount(void)
	{
		return myPerInstanceData.size();
	}

	MyBuffer GetBuffers(void)
	{
		return buffers;
	}

	void AddPerInstanceMatrix(PerInstanceData data)
	{
		pModelMatrix[myPerInstanceData.size()]= vmath::translate(data.WorldPos) * vmath::rotate(data.fRotAngleZ, 0.0f, 0.0f, 1.0f) * vmath::rotate(data.fRotAngleY, 0.0f, 1.0f, 0.0f) * vmath::rotate(data.fRotAngleX, 1.0f, 0.0f, 0.0f) * vmath::scale(data.fUniformScale);
		myPerInstanceData.push_back(data);
	}

	void GetPerInstanceData(void);
	
private:
	CandidateDraw* pDraws;
	MyBuffer buffers;

	vmath::mat4* pModelMatrix;

	//per instance data
	std::vector< PerInstanceData > myPerInstanceData;
	const unsigned int CANDIDATE_COUNT = 32;
};

