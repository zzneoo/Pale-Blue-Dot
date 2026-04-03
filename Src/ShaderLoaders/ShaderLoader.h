#pragma once
#include "stdafx.h"
#include"MyLog.h"

class ShaderLoader
{
public:
	ShaderLoader();
	virtual~ShaderLoader();
	
	void Start(void)
	{
		glUseProgram(ShaderProgramObject);
	}
	void Stop(void)
	{
		glUseProgram(0);
	}
	GLuint GetShaderProgram(void)
	{
		return ShaderProgramObject;
	}

private:
	//methods
	void CleanUp_ShaderLoader(void);
	virtual bool Initialize(void) = 0;
	virtual void GetAllUniformLocations(void) = 0;
	GLchar* load_ShaderFromFile(GLchar const* path, bool* status);

	//variables
	GLuint ShaderProgramObject;
protected:
	//methods
	bool CreateAndCompileShader(GLuint* shaderObject, GLenum SHADER, const GLchar* shaderFilePath);
	void CreateShaderProgram(void);
	bool LinkShaderProgram(void);
	
	void MyAttachShader(GLuint *ShaderObject);
	void MyDetachShader(GLuint *ShaderObject);

	GLuint GetUniformLocation(const GLchar* uniform);

	//Variables
	const char* ShaderLoaderLog_filename = "../Log/MyShaderLoaderlog.txt";
	char* TempShaderFileName;//to debug shader cleanup 
	MyLog ShaderLoaderLog;

	GLuint VertexShaderObject;
	GLuint TessControlShaderObject;
	GLuint TessEvalShaderObject;
	GLuint GeometryShaderObject;
	GLuint FragmentShaderObject;
	GLuint ExtraShaderObject;

	GLuint ComputeShaderObject;
};
class ZzShaderLoader :public ShaderLoader
{
public:
	ZzShaderLoader() {};
	virtual~ZzShaderLoader() {};

private:
	void AttachShaders(GLuint shaderCount);

	void DetachShaders(GLuint shaderCount);

	bool CreateAndLinkShaderProgram(GLuint shaderCount);

protected:

	bool InitializeShaderLoader(const GLchar* filepath_cs);
	bool InitializeShaderLoader(const GLchar* filepath_vs, const GLchar* filepath_fs);
	bool InitializeShaderLoader(const GLchar* filepath_vs, const GLchar* filepath_gs, const GLchar* filepath_fs);
	bool InitializeShaderLoader(const GLchar* filepath_vs, const GLchar* filepath_tcs, const GLchar* filepath_tes, const GLchar* filepath_fs);
	bool InitializeShaderLoader(const GLchar* filepath_vs, const GLchar* filepath_tcs, const GLchar* filepath_tes, const GLchar* filepath_gs, const GLchar* filepath_fs);

	void SetExtraShader(GLuint ExtraShaderID)
	{
		ExtraShaderObject = ExtraShaderID;
	}

	void UniformLoadMatrix(GLuint location, vmath::mat4 matrix)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
	}
	void UniformLoadDoubleMatrix(GLuint location, vmath::dmat4 matrix)
	{
		glUniformMatrix4dv(location, 1, GL_FALSE, matrix);
	}
	void UniformLoadMatrixCounted(GLuint location, vmath::mat4* matrix,GLsizei count)
	{
		glUniformMatrix4fv(location, count, GL_FALSE, (GLfloat*)matrix);
	}

	void UniformLoadVec3(GLuint location, vmath::vec3 vector)
	{
		glUniform3fv(location, 1, vector);
	}
	void UniformLoadVec4(GLuint location, vmath::vec4 vector)
	{
		glUniform4fv(location, 1, vector);
	}

	void UniformLoadVec2(GLuint location, vmath::vec2 vector)
	{
		glUniform2fv(location, 1, vector);
	}

	void UniformLoad1f(GLuint location, GLfloat num)
	{
		glUniform1f(location, num);
	}

	void UniformLoad1d(GLuint location, GLdouble num)
	{
		glUniform1d(location, num);
	}

	void UniformLoad1i(GLuint location, GLint num)
	{
		glUniform1i(location, num);
	}

	void UniformLoad1ui(GLuint location, GLuint num)
	{
		glUniform1ui(location, num);
	}

	void UniformLoadTexture1D(GLuint location, GLenum TEXTURE, GLuint tNo, GLuint textureID)
	{
		glActiveTexture(TEXTURE);
		glBindTexture(GL_TEXTURE_1D, textureID);
		glUniform1i(location, tNo);
	}

	void UniformLoadTexture2D(GLuint location, GLenum TEXTURE, GLuint tNo, GLuint textureID)
	{
		glActiveTexture(TEXTURE);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUniform1i(location, tNo);
	}
	void UniformLoadTextureCubeMap(GLuint location, GLenum TEXTURE, GLuint tNo, GLuint textureID)
	{
		glActiveTexture(TEXTURE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		glUniform1i(location, tNo);
	}

	void UniformLoadTexture3D(GLuint location, GLenum TEXTURE, GLuint tNo, GLuint textureID)
	{
		glActiveTexture(TEXTURE);
		glBindTexture(GL_TEXTURE_3D, textureID);
		glUniform1i(location, tNo);
	}

};

