#pragma once
#include "MyObject3D.h"
using namespace std;
using namespace vmath;

class ModelPBR_SpeedTreeInstanced :public MyObject3D
{
public:
	ModelPBR_SpeedTreeInstanced(const char* filePath);
	~ModelPBR_SpeedTreeInstanced();

	void Render(void)override;

	InstanceModelData* GetMyInstanceModelData(void)
	{
		return &MyInstanceModelData;
	}

private:
	//functions
	void LoadVao(void)override;//pure virtual
	void LoadObject3DTextures(void)override;//pure virtual

	bool MyObjLoader(const char* filePath);
	void CreateInterleavedData(void);
	void MyTangentLoader(std::vector<vec3> vertices, std::vector<vec2> uvs);
	GLuint GetPerInstanceData(void);


	//variables
	//data
	std::vector<vmath::vec3> vertices, normals, tangents;
	std::vector<vmath::vec2> texCoords;

	std::vector<GLfloat> modelInterleavedData;
	std::vector<GLuint> modelIndices;

	InstanceModelData MyInstanceModelData;

	GLuint indicesCount;

};

