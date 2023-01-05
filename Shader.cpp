#include "Shader.h"

using namespace std;

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	string vertexShaderSourceCode;
	string fragmentShaderSourceCode;
	ifstream vertexCodeFile;
	ifstream fragmentCodeFile;

	vertexCodeFile.exceptions(ifstream::failbit | ifstream::badbit);
	fragmentCodeFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		//打开文件
		vertexCodeFile.open(vertexShaderPath);
		fragmentCodeFile.open(fragmentShaderPath);
		stringstream vertexShaderStream, fragmentShaderStream;
		//读取文件的缓冲内容到数据流中
		vertexShaderStream << vertexCodeFile.rdbuf();
		fragmentShaderStream << fragmentCodeFile.rdbuf();
		//关闭文件
		vertexCodeFile.close();
		fragmentCodeFile.close();
		//转换数据流到string
		vertexShaderSourceCode = vertexShaderStream.str();
		fragmentShaderSourceCode = fragmentShaderStream.str();
	}
	catch (ifstream::failure err)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}

	//string 转换成const char*
	const char* vertexShaderCode = vertexShaderSourceCode.c_str();
	const char* fragmentShaderCode = fragmentShaderSourceCode.c_str();

	//编译着色器
	unsigned int vertexShader, fragmentShader;
	int success;
	char infoLog[512];
	//顶点着色器
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR:SHADER::VERTEX::COMPOILE_FAILED\n" << infoLog << endl;
	}
	//片元着色器
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR:SHADER::FRAGMENT::COMPOILE_FAILED\n" << infoLog << endl;
	}

	//着色器程序
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	//链接完成后删除着色器
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& inname, bool invalue) const
{
	glUniform1i(glGetUniformLocation(ID, inname.c_str()), (int)invalue);
}

bool Shader::GetBool(const std::string& inname) const
{
	int tempvalue;
	glGetUniformiv(ID, glGetUniformLocation(ID, inname.c_str()), &tempvalue);
	return tempvalue;
}

void Shader::SetInt(const std::string& inname, int invalue) const
{
	glUniform1i(glGetUniformLocation(ID, inname.c_str()), invalue);
}

int Shader::GetInt(const std::string& inname) const
{
	int tempvalue;
	glGetUniformiv(ID, glGetUniformLocation(ID, inname.c_str()), &tempvalue);
	return tempvalue;
}

void Shader::SetFloat(const std::string& inname, float invalue) const
{
	glUniform1f(glGetUniformLocation(ID, inname.c_str()), invalue);
}

float Shader::GetFloat(const std::string& inname) const
{
	float tempvalue;
	glGetUniformfv(ID, glGetUniformLocation(ID, inname.c_str()), &tempvalue);
	return tempvalue;
}

void Shader::SetVec3f(const std::string& inname, float invalue[3]) const
{
	glUniform3f(glGetUniformLocation(ID, inname.c_str()), invalue[0], invalue[1], invalue[2]);
}

void Shader::SetVec3f(const std::string& inname, glm::vec3 invalue) const
{
	glUniform3f(glGetUniformLocation(ID, inname.c_str()), invalue[0], invalue[1], invalue[2]);
}

void Shader::SetVec4f(const std::string& inname, float invalue[4]) const
{
	glUniform4f(glGetUniformLocation(ID, inname.c_str()), invalue[0], invalue[1], invalue[2], invalue[3]);
}

void Shader::SetVec4f(const std::string& inname, glm::vec4 invalue) const
{
	glUniform4f(glGetUniformLocation(ID, inname.c_str()), invalue[0], invalue[1], invalue[2], invalue[3]);
}

void Shader::SetMat4f(const std::string& inname, glm::mat4 invalue) const
{
	float tempM[16] = {
		invalue[0].x,invalue[0].y,invalue[0].z,invalue[0].w,
		invalue[1].x,invalue[1].y,invalue[1].z,invalue[1].w,
		invalue[2].x,invalue[2].y,invalue[2].z,invalue[2].w,
		invalue[3].x,invalue[3].y,invalue[3].z,invalue[3].w
	};
	glUniformMatrix4fv(glGetUniformLocation(ID, inname.c_str()), 1, GL_TRUE, tempM);
}


