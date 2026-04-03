#include "stdafx.h"
#include "SkyDome3D.h"

#define PI 3.14159265359f

SkyDome3D::SkyDome3D()
{
	fSkyFactor = 0.0f;

	LoadVao();
	SetModelMatrix(vmath::vec3(0.0f, 0.0f, 0.0f), 0.0f, vmath::vec3(1.0f, 0.0f, 0.0f), vmath::vec3(fOuterRadius));
}


SkyDome3D::~SkyDome3D()
{
}


void SkyDome3D::LoadObject3DTextures(void)
{
	//pure virtual
}

void SkyDome3D::LoadVao(void)
{

	GLfloat radius = 1.0f;

	//SPHERE VERTICES
	const GLsizei sphereVerticesCount = MAX_SPHERE_X * ((MAX_SPHERE_Y / QuarterDivider)-adjustStack);//quarter sphere
	vmath::vec3 *sphereVertices=new vmath::vec3[sphereVerticesCount];

	GLuint countVertex = 0;

	GLsizei CountSlicesX = 0;
	GLsizei CountSlicesY = 0;

	for (GLfloat i = 0.0f; CountSlicesY< ((MAX_SPHERE_Y / QuarterDivider) - adjustStack); i += 1.0f / stacks)
	{
		//t = (i / PI);

		CountSlicesX = 0;
		for (GLfloat j = 0.0f; CountSlicesX< MAX_SPHERE_X; j +=1.0f / slices)
		{
			GLfloat x = cosf(j*PI)*sinf(i*PI);
			GLfloat y = cosf(i*PI);
			GLfloat z = sinf(j*PI)*sinf(i*PI);

			sphereVertices[countVertex][0] = radius * x;
			sphereVertices[countVertex][1] = radius * y;
			sphereVertices[countVertex][2] = radius * z;


		//	s = (j / (2.0f*PI));

			//this works too but I dont know maths behind it
		//	sphereUVs[countUVs++] = 0.5f +(atan2f(z,x)/(2.0f*PI));// 
		//	sphereUVs[countUVs++] = 0.5f-((asinf(y)/PI));// 

			//sphereUVs[countUVs++] = s;// dont need uvs for skydome
			//sphereUVs[countUVs++] = t;//

			countVertex += 1;
			CountSlicesX += 1;
		}
		CountSlicesY += 1;

	}

	//SPHERE INDICES

	GLuint *indices = new GLuint[indicesCount];


	int counter = 0;
	for (int i = 0, j = 0; i < indicesCount-1; i += 6, j += 1)
	{
		indices[i + 0] = (GLuint)MAX_SPHERE_X + (GLuint)j + (GLuint)1;
		indices[i + 1] = (GLuint)j;
		indices[i + 2] = (GLuint)j + (GLuint)1;

		indices[i + 3] = (GLuint)MAX_SPHERE_X + (GLuint)j + (GLuint)1;
		indices[i + 4] = (GLuint)MAX_SPHERE_X + (GLuint)j;
		indices[i + 5] = (GLuint)j;

		counter += 1;

		if (counter / (MAX_SPHERE_X - 1) == 1)
		{
			j += 1;
			counter = 0;
		}

	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	const GLsizei sphereVertexBufferSize = sphereVerticesCount * sizeof(vmath::vec3);
	glBufferData(GL_ARRAY_BUFFER, sphereVertexBufferSize, sphereVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(ZZNEO_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(ZZNEO_ATTRIB_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vbo_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element);

	const GLsizei indicesBufferSize = indicesCount * sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesBufferSize, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	if (sphereVertices)
	{
		delete[] sphereVertices;
		sphereVertices = NULL;
	}

	if (indices)
	{
		delete[] indices;
		indices = NULL;
	}

}

void SkyDome3D::Render(void)
{
	glFrontFace(GL_CW);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE);

	glDepthMask(GL_FALSE);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element);
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDepthMask(GL_TRUE);

	glFrontFace(GL_CCW);
}