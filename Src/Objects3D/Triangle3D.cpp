#include"stdafx.h"
#include"Triangle3D.h"

Triangle3D::Triangle3D()
{
	LoadVao();
	LoadObject3DTextures();
}
Triangle3D::~Triangle3D()
{
}

void Triangle3D::LoadVao(void)
{
	const GLfloat triangleVertices[] =
	{ 0.0f, 2.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

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

void Triangle3D::Render(void)
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glMultiDrawArraysIndirectCountARB(GL_TRIANGLES, 0, 0, 1024, 0);
	glBindVertexArray(0);

}
