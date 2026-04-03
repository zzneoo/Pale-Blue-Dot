#include "ShaderLoader.h"

//------------------------ShaderLoader---------------------------//

ShaderLoader::ShaderLoader()
{
	ShaderProgramObject = 0;

	VertexShaderObject = 0;
	TessControlShaderObject = 0;
	TessEvalShaderObject = 0;
	GeometryShaderObject = 0;
	FragmentShaderObject = 0;
	ExtraShaderObject = 0;

	ComputeShaderObject = 0;

	TempShaderFileName = NULL;
}
ShaderLoader::~ShaderLoader()
{
	CleanUp_ShaderLoader();
}

GLchar* ShaderLoader::load_ShaderFromFile(GLchar const* path,bool* status)
{
	GLchar* buffer =NULL;
	size_t length;
	FILE *f;
	if (fopen_s(&f, path, "rb")!=0)
	{
		TCHAR str[512];

		wsprintf(str, "Error load_ShaderFile= %s", path);
		MessageBox(NULL,str, TEXT("Error"), MB_OK | MB_TOPMOST);
		*status = false;
	}
	else
	{
		*status = true;
	}

	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		//rewind(f);
		fseek(f, 0, SEEK_SET);
		buffer = (GLchar*)malloc((length + (size_t)1) * sizeof(GLchar));//+1 null
		if (buffer)
		{
			fread(buffer, sizeof(GLchar), length, f);
			buffer[length] = '\0';
		}
		fclose(f);

	}	
	
	return buffer;
}
bool ShaderLoader::CreateAndCompileShader(GLuint* shaderObject, GLenum SHADER, const GLchar* shaderFilePath)
{
	TempShaderFileName =(char*) shaderFilePath;

	*shaderObject = glCreateShader(SHADER);

	bool status = true;

	 GLchar *ShaderSourceCode = load_ShaderFromFile(shaderFilePath,&status);

	 if (status == false)
	 {
		 return false;
	 }

	glShaderSource(*shaderObject, 1, (const GLchar **)&ShaderSourceCode, NULL);

	if (ShaderSourceCode)
	{
		free(ShaderSourceCode);
		ShaderSourceCode = NULL;
	}

	glCompileShader(*shaderObject);

	GLint iInfoLogLength = 0;
	GLint iShaderCompiledStatus = 0;
	char *szInfoLog = NULL;
	glGetShaderiv(*shaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(*shaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(*shaderObject, iInfoLogLength, &written, szInfoLog);
				char str[8192];
				sprintf_s(str, "%s Compilation Log : %s\n", shaderFilePath, szInfoLog);
				//fprintf(m_pFile_ShaderLoader, "Vertex Shader Compilation Log : %s\n", szInfoLog);
				ShaderLoaderLog.PrintLog(str);
				ShaderLoaderLog.EndLog(ShaderLoaderLog_filename);
				free(szInfoLog);
				return false;
			}
		}
	}
	return true;
}
bool ShaderLoader::LinkShaderProgram(void)
{
	GLint iInfoLogLength = 0;
	GLint iShaderCompiledStatus = 0;
	char *szInfoLog = NULL;

	glLinkProgram(ShaderProgramObject);
	GLint iShaderProgramLinkStatus = 0;
	glGetProgramiv(ShaderProgramObject, GL_LINK_STATUS, &iShaderProgramLinkStatus);
	if (iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(ShaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(ShaderProgramObject, iInfoLogLength, &written, szInfoLog);
				//fprintf(m_pFile_ShaderLoader, "Shader Program Link Log : %s\n", szInfoLog);

				char str[8192];
				sprintf_s(str, "Shader Program Link Log : %s\n", szInfoLog);
				ShaderLoaderLog.PrintLog(str);
				ShaderLoaderLog.EndLog(ShaderLoaderLog_filename);
				free(szInfoLog);

				return false;
			}
		}
	}
	return true;
}
void ShaderLoader::CreateShaderProgram(void)
{
	ShaderProgramObject = glCreateProgram();
}
void ShaderLoader::MyAttachShader(GLuint *ShaderObject)
{
	glAttachShader(ShaderProgramObject, *ShaderObject);
}
void ShaderLoader::MyDetachShader(GLuint *ShaderObject)
{
	glDetachShader(ShaderProgramObject, *ShaderObject);
}
GLuint ShaderLoader::GetUniformLocation(const GLchar* uniform)
{
	return glGetUniformLocation(ShaderProgramObject, uniform);
}
void ShaderLoader::CleanUp_ShaderLoader(void)
{
	glUseProgram(ShaderProgramObject);

	if (FragmentShaderObject)
	{
		glDeleteShader(FragmentShaderObject);
		FragmentShaderObject = 0;
	}

	if (GeometryShaderObject)
	{
		glDeleteShader(GeometryShaderObject);
		GeometryShaderObject = 0;
	}

	if (TessEvalShaderObject)
	{
		glDeleteShader(TessEvalShaderObject);
		TessEvalShaderObject = 0;
	}

	if (TessControlShaderObject)
	{
		glDeleteShader(TessControlShaderObject);
		TessControlShaderObject = 0;
	}

	if (VertexShaderObject)
	{
		glDeleteShader(VertexShaderObject);
		VertexShaderObject = 0;
	}


	if (ComputeShaderObject)
	{
		glDeleteShader(ComputeShaderObject);
		ComputeShaderObject = 0;
	}

	glUseProgram(0);

	if (ShaderProgramObject)
	{
		glDeleteProgram(ShaderProgramObject);
		ShaderProgramObject = 0;
	}
	ShaderLoaderLog.EndLog(ShaderLoaderLog_filename);

}


//------------------------ZzShaderLoader---------------------------//
void ZzShaderLoader::AttachShaders(GLuint shaderCount)
{

	switch (shaderCount)
	{
	case 1:
		MyAttachShader(&ComputeShaderObject);
		break;

	case 2:
		MyAttachShader(&VertexShaderObject);
		MyAttachShader(&FragmentShaderObject);
		break;

	case 3:
		MyAttachShader(&VertexShaderObject);
		MyAttachShader(&GeometryShaderObject);
		MyAttachShader(&FragmentShaderObject);
		break;

	case 4:
		MyAttachShader(&VertexShaderObject);
		MyAttachShader(&TessControlShaderObject);
		MyAttachShader(&TessEvalShaderObject);
		MyAttachShader(&FragmentShaderObject);
		break;

	case 5:
		MyAttachShader(&VertexShaderObject);
		MyAttachShader(&TessControlShaderObject);
		MyAttachShader(&TessEvalShaderObject);
		MyAttachShader(&GeometryShaderObject);
		MyAttachShader(&FragmentShaderObject);
		break;

	default:
		break;
	}

	if (ExtraShaderObject)
	{
		MyAttachShader(&ExtraShaderObject);
	}
}
void  ZzShaderLoader::DetachShaders(GLuint shaderCount)
{
	switch (shaderCount)
	{
	case 1:
		MyDetachShader(&ComputeShaderObject);
		break;

	case 2:
		MyDetachShader(&FragmentShaderObject);
		MyDetachShader(&VertexShaderObject);		
		break;

	case 3:
		MyDetachShader(&FragmentShaderObject);
		MyDetachShader(&GeometryShaderObject);
		MyDetachShader(&VertexShaderObject);
		break;

	case 4:
		MyDetachShader(&FragmentShaderObject);
		MyDetachShader(&TessEvalShaderObject);
		MyDetachShader(&TessControlShaderObject);
		MyDetachShader(&VertexShaderObject);
		break;

	case 5:
		MyDetachShader(&FragmentShaderObject);
		MyDetachShader(&GeometryShaderObject);
		MyDetachShader(&TessEvalShaderObject);
		MyDetachShader(&TessControlShaderObject);
		MyDetachShader(&VertexShaderObject);
		break;

	default:
		break;
	}
	if (ExtraShaderObject)
	{
		MyDetachShader(&ExtraShaderObject);
	}
}
bool ZzShaderLoader::CreateAndLinkShaderProgram(GLuint shaderCount)
{

	CreateShaderProgram();

	AttachShaders(shaderCount);

	if (!LinkShaderProgram())
	{
		//Here Get Attached shaders and detach them
		return false;
	}

	DetachShaders(shaderCount);

	return true;
}

bool ZzShaderLoader::InitializeShaderLoader(const GLchar* filepath_cs)
{
	GLuint ShaderCount = 1;

	ShaderLoaderLog.StartLog(ShaderLoaderLog_filename);

	if (!CreateAndCompileShader(&ComputeShaderObject, GL_COMPUTE_SHADER, filepath_cs))
	{
		return false;
	}

	if (!CreateAndLinkShaderProgram(ShaderCount))
	{
		return false;
	}

	ShaderLoaderLog.EndLog(ShaderLoaderLog_filename);

	return true;
}
bool ZzShaderLoader::InitializeShaderLoader(const GLchar* filepath_vs, const GLchar* filepath_fs)
{
	GLuint ShaderCount = 2;

	ShaderLoaderLog.StartLog(ShaderLoaderLog_filename);

	if (!CreateAndCompileShader(&VertexShaderObject, GL_VERTEX_SHADER, filepath_vs))
	{
		return false;
	}
	if (!CreateAndCompileShader(&FragmentShaderObject, GL_FRAGMENT_SHADER, filepath_fs))
	{
		return false;
	}
	if (!CreateAndLinkShaderProgram(ShaderCount))
	{
		return false;
	}

	ShaderLoaderLog.EndLog(ShaderLoaderLog_filename);

	return true;
}
bool ZzShaderLoader::InitializeShaderLoader(const GLchar* filepath_vs, const GLchar* filepath_gs, const GLchar* filepath_fs)
{
	GLuint ShaderCount = 3;

	ShaderLoaderLog.StartLog(ShaderLoaderLog_filename);

	if (!CreateAndCompileShader(&VertexShaderObject, GL_VERTEX_SHADER, filepath_vs))
	{
		return false;
	}
	if (!CreateAndCompileShader(&GeometryShaderObject, GL_GEOMETRY_SHADER, filepath_gs))
	{
		return false;
	}
	if (!CreateAndCompileShader(&FragmentShaderObject, GL_FRAGMENT_SHADER, filepath_fs))
	{
		return false;
	}
	if (!CreateAndLinkShaderProgram(ShaderCount))
	{
		return false;
	}

	ShaderLoaderLog.EndLog(ShaderLoaderLog_filename);

	return true;
}
bool ZzShaderLoader::InitializeShaderLoader(const GLchar* filepath_vs, const GLchar* filepath_tcs, const GLchar* filepath_tes, const GLchar* filepath_fs)
{
	GLuint ShaderCount = 4;

	ShaderLoaderLog.StartLog(ShaderLoaderLog_filename);

	if (!CreateAndCompileShader(&VertexShaderObject, GL_VERTEX_SHADER, filepath_vs))
	{
		return false;
	}
	if (!CreateAndCompileShader(&TessControlShaderObject, GL_TESS_CONTROL_SHADER, filepath_tcs))
	{
		return false;
	}
	if (!CreateAndCompileShader(&TessEvalShaderObject, GL_TESS_EVALUATION_SHADER, filepath_tes))
	{
		return false;
	}
	if (!CreateAndCompileShader(&FragmentShaderObject, GL_FRAGMENT_SHADER, filepath_fs))
	{
		return false;
	}
	if (!CreateAndLinkShaderProgram(ShaderCount))
	{
		return false;
	}

	ShaderLoaderLog.EndLog(ShaderLoaderLog_filename);

	return true;
}
bool ZzShaderLoader::InitializeShaderLoader(const GLchar* filepath_vs, const GLchar* filepath_tcs, const GLchar* filepath_tes, const GLchar* filepath_gs, const GLchar* filepath_fs)
{
	GLuint ShaderCount = 5;

	ShaderLoaderLog.StartLog(ShaderLoaderLog_filename);

	if (!CreateAndCompileShader(&VertexShaderObject, GL_VERTEX_SHADER, filepath_vs))
	{
		return false;
	}
	if (!CreateAndCompileShader(&TessControlShaderObject, GL_TESS_CONTROL_SHADER, filepath_tcs))
	{
		return false;
	}
	if (!CreateAndCompileShader(&TessEvalShaderObject, GL_TESS_EVALUATION_SHADER, filepath_tes))
	{
		return false;
	}
	if (!CreateAndCompileShader(&GeometryShaderObject, GL_GEOMETRY_SHADER, filepath_gs))
	{
		return false;
	}
	if (!CreateAndCompileShader(&FragmentShaderObject, GL_FRAGMENT_SHADER, filepath_fs))
	{
		return false;
	}
	if (!CreateAndLinkShaderProgram(ShaderCount))
	{
		return false;
	}

	ShaderLoaderLog.EndLog(ShaderLoaderLog_filename);

	return true;
}

