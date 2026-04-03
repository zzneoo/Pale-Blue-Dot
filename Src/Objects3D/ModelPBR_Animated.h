#pragma once
#include "MyObject3D.h"
using namespace std;
using namespace vmath;

class ModelPBR_Animated :public MyObject3D
{
public:
	ModelPBR_Animated(const char* filePath);
	~ModelPBR_Animated();

	void Render(void)override;
private:
	//functions
	void LoadVao(void)override;//pure virtual
	void LoadObject3DTextures(void)override;//pure virtual

	//variables
	//data
	std::vector<vmath::vec3> vertices, normals, tangents;
	std::vector<vmath::vec2> texCoords;

	std::vector<GLuint> modelIndices;

	GLuint indicesCount;

};


