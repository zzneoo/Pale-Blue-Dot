#pragma once
#include "MyObject3D.h"
using namespace std;
using namespace vmath;

class ModelPBR :public MyObject3D
{
public:
	ModelPBR(const char* filePath);
	~ModelPBR();

	void Render(void)override;
private:
	//functions
	void LoadVao(void)override;//pure virtual
	void LoadObject3DTextures(void)override;//pure virtual

	bool MyObjLoader(const char* filePath);
	void CreateInterleavedData(void);
	void MyTangentLoader(std::vector<vec3> vertices, std::vector<vec2> uvs);

	//variables
	//data
	std::vector<vmath::vec3> vertices, normals, tangents;
	std::vector<vmath::vec2> texCoords;

	std::vector<GLfloat> modelInterleavedData;
	std::vector<GLuint> modelIndices;

	GLuint indicesCount;

};

