#pragma once
#include "ShaderLoader.h"
class CullingIndirectShaderLoader :public ZzShaderLoader
{
public:
	CullingIndirectShaderLoader();
	~CullingIndirectShaderLoader();

	void LoadProjXUniform(vmath::mat4 matrix);

private:
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	const GLchar* filepath_cs = "Resources/Shaders/cullIndirect/cullIndirect.compute";

	//uniforms
	GLuint ProjXUniform;

};

