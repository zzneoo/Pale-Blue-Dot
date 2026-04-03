#pragma once
#include "ShaderLoader.h"
class DeformedSphereShaderLoader :	public ZzShaderLoader
{
public:
	DeformedSphereShaderLoader();
	~DeformedSphereShaderLoader();

	//matrices
	void LoadDeformedSphereFactorUniform(GLfloat SphereFactor);

	void LoadModelXUniform(vmath::mat4 matrix);
	void LoadViewXUniform(vmath::mat4 matrix);
	void LoadProjXUniform(vmath::mat4 matrix);
	void LoadModelViewProjXUniform(vmath::mat4 matrix);

	//maps
	void LoadSphereMap_TextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);

	//shadow
	//void LoadShadowMapTextureUniform(GLenum TEXTURE, GLuint tNo, GLuint textureID);
	//void Loadm4LightSpaceXUniform(vmath::mat4 matrix);


private:
	//Functions
	bool Initialize(void) override;
	void GetAllUniformLocations(void) override;

	//Class Members
	const GLchar * filepath_vs = "Resources/Shaders/DeformedSphere/DeformedSphere.vs";
	const GLchar * filepath_tcs = "Resources/Shaders/DeformedSphere/DeformedSphere.tcs";
	const GLchar * filepath_tes = "Resources/Shaders/DeformedSphere/DeformedSphere.tes";
	const GLchar * filepath_fs = "Resources/Shaders/DeformedSphere/DeformedSphere.fs";

	//uniforms
	GLuint m4ModelX_Uniform;
	GLuint m4ViewX_Uniform;
	GLuint m4ProjX_Uniform;
	GLuint m4ModelViewProjX_Uniform;

	GLuint fDeformedSphereFactorUniform;
	
	//maps
	GLuint Sampler2D_SphereMap_Uniform;

	//shadow
//	GLuint m4LightSpaceX_Uniform;
//	GLuint Sampler2D_ShadowMapUniform;
};

