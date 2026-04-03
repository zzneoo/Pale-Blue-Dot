#pragma once
#include "MyObject3D.h"
using namespace std;
using namespace vmath;

class ModelPBR_SpeedTree :public MyObject3D
{
public:
	ModelPBR_SpeedTree(const char* filePath);
	~ModelPBR_SpeedTree();

	void Render(void)override;
	vmath::vec3 getLightColor(GLdouble sunTheta);
	vmath::vec3 getLightColor2(GLdouble sunTheta);
private:
	//functions
	void LoadVao(void)override;//pure virtual
	void LoadObject3DTextures(void)override;//pure virtual

	bool MyObjLoader(const char* filePath);
	void CreateInterleavedData(void);
	void MyTangentLoader(std::vector<vec3> vertices, std::vector<vec2> uvs);

	//lightColor
	GLdouble clamp(GLdouble factor, GLdouble minimum, GLdouble maximum)
	{
#pragma push_macro("min")
#undef min

#pragma push_macro("max")
#undef max
		return vmath::min(vmath::max(factor, minimum), maximum);
#pragma pop_macro("min")
#pragma pop_macro("max")
	}

	GLdouble smoothstep(GLdouble edge0, GLdouble edge1, GLdouble factor)
	{
		GLdouble t = clamp((factor - edge0) / (edge1 - edge0), 0.0, 1.0);
		return t * t * (3.0 - 2.0 * t);
	}


	//variables
	//data
	std::vector<vmath::vec3> vertices, normals, tangents;
	std::vector<vmath::vec2> texCoords;

	std::vector<GLfloat> modelInterleavedData;
	std::vector<GLuint> modelIndices;

	GLuint indicesCount;

};

