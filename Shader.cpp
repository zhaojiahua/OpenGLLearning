#include "Shader.h"

using namespace std;

Shader::Shader(){
	//生成着色器程序
	ID = glCreateProgram();
}

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	//生成着色器程序
	ID = glCreateProgram();

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

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath)
{
	//生成着色器程序
	ID = glCreateProgram();

	string vertexShaderSourceCode;
	string fragmentShaderSourceCode;
	string geometryShaderSourceCode;
	ifstream vertexCodeFile;
	ifstream fragmentCodeFile;
	ifstream geometryCodeFile;

	vertexCodeFile.exceptions(ifstream::failbit | ifstream::badbit);
	fragmentCodeFile.exceptions(ifstream::failbit | ifstream::badbit);
	geometryCodeFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		//打开文件
		vertexCodeFile.open(vertexShaderPath);
		fragmentCodeFile.open(fragmentShaderPath);
		geometryCodeFile.open(geometryShaderPath);
		stringstream vertexShaderStream, fragmentShaderStream, geometryShaderStream;
		//读取文件的缓冲内容到数据流中
		vertexShaderStream << vertexCodeFile.rdbuf();
		fragmentShaderStream << fragmentCodeFile.rdbuf();
		geometryShaderStream << geometryCodeFile.rdbuf();
		//关闭文件
		vertexCodeFile.close();
		fragmentCodeFile.close();
		geometryCodeFile.close();
		//转换数据流到string
		vertexShaderSourceCode = vertexShaderStream.str();
		fragmentShaderSourceCode = fragmentShaderStream.str();
		geometryShaderSourceCode = geometryShaderStream.str();
	}
	catch (ifstream::failure err)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}

	//string 转换成const char*
	const char* vertexShaderCode = vertexShaderSourceCode.c_str();
	const char* fragmentShaderCode = fragmentShaderSourceCode.c_str();
	const char* geometryShaderCode = geometryShaderSourceCode.c_str();

	//编译三个着色器
	unsigned int vertexShader, fragmentShader, geometryShader;
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
	//几何着色器
	geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryShader, 1, &geometryShaderCode, NULL);
	glCompileShader(geometryShader);
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
		cout << "ERROR:SHADER::FRAGMENT::COMPOILE_FAILED\n" << infoLog << endl;
	}

	//链接三个着色器
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glAttachShader(ID, geometryShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	//链接完成后删除着色器
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geometryShader);
}

GLint Shader::GenVertexShader(const char* vertexShaderPath)
{
	string vertexShaderSourceCode;
	ifstream vertexCodeFile;

	vertexCodeFile.exceptions(ifstream::failbit | ifstream::badbit);	//iostream读写数据出错的时候抛出异常

	try
	{
		//打开文件
		vertexCodeFile.open(vertexShaderPath);
		//读取文件的缓冲内容到数据流中
		stringstream vertexShaderStream;
		vertexShaderStream << vertexCodeFile.rdbuf();
		//关闭文件
		vertexCodeFile.close();

		//转换数据流到string
		vertexShaderSourceCode = vertexShaderStream.str();
	}
	catch (ifstream::failure err)
	{
		cout << "ERROR::VERTEXSHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
		return GL_FALSE;
	}

	//string 转换成const char*
	const char* vertexShaderCode = vertexShaderSourceCode.c_str();

	//编译着色器
	unsigned int vertexShader;
	GLint success;
	char infoLog[512];
	//顶点着色器
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR:VERTEXSHADER::COMPOILE_FAILED\n" << infoLog << endl;
	}

	glAttachShader(ID, vertexShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		cout << "ERROR::VERTEXSHADER::LINKING_FAILED\n" << infoLog << endl;
		return GL_FALSE;
	}
	//链接完成后删除着色器
	glDeleteShader(vertexShader);
	return success;
}

GLint Shader::GenGeometryShader(const char* geometryShaderPath)
{
	string geometryShaderSourceCode;
	ifstream geometryCodeFile;

	geometryCodeFile.exceptions(ifstream::failbit | ifstream::badbit);	//iostream读写数据出错的时候抛出异常

	try
	{
		//打开文件
		geometryCodeFile.open(geometryShaderPath);
		//读取文件的缓冲内容到数据流中
		stringstream geometryShaderStream;
		geometryShaderStream << geometryCodeFile.rdbuf();
		//关闭文件
		geometryCodeFile.close();

		//转换数据流到string
		geometryShaderSourceCode = geometryShaderStream.str();
	}
	catch (ifstream::failure err)
	{
		cout << "ERROR::GEOMETRYSHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
		return GL_FALSE;
	}

	//string 转换成const char*
	const char* geometryShaderCode = geometryShaderSourceCode.c_str();

	//编译着色器
	unsigned int geometryShader;
	GLint success;
	char infoLog[512];
	//几何着色器
	geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryShader, 1, &geometryShaderCode, NULL);
	glCompileShader(geometryShader);
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
		cout << "ERROR:GEOMETRYSHADER::COMPOILE_FAILED\n" << infoLog << endl;
	}

	glAttachShader(ID, geometryShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		cout << "ERROR::GEOMETRYSHADER::LINKING_FAILED\n" << infoLog << endl;
		return GL_FALSE;
	}
	//链接完成后删除着色器
	glDeleteShader(geometryShader);
	return success;
}

GLint Shader::GenFragmentShader(const char* fragmentShaderPath)
{
	string fragmentShaderSourceCode;
	ifstream fragmentCodeFile;

	fragmentCodeFile.exceptions(ifstream::failbit | ifstream::badbit);	//iostream读写数据出错的时候抛出异常

	try
	{
		//打开文件
		fragmentCodeFile.open(fragmentShaderPath);
		//读取文件的缓冲内容到数据流中
		stringstream fragmentShaderStream;
		fragmentShaderStream << fragmentCodeFile.rdbuf();
		//关闭文件
		fragmentCodeFile.close();

		//转换数据流到string
		fragmentShaderSourceCode = fragmentShaderStream.str();
	}
	catch (ifstream::failure err)
	{
		cout << "ERROR::FRAGMENTSHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
		return GL_FALSE;
	}

	//string 转换成const char*
	const char* fragmentShaderCode = fragmentShaderSourceCode.c_str();

	//编译着色器
	unsigned int fragmentShader;
	GLint success;
	char infoLog[512];
	//顶点着色器
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR:FRAGMENTSHADER::COMPOILE_FAILED\n" << infoLog << endl;
	}

	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		cout << "ERROR::FRAGMENTSHADER::LINKING_FAILED\n" << infoLog << endl;
		return GL_FALSE;
	}
	//链接完成后删除着色器
	glDeleteShader(fragmentShader);
	return success;
}

void Shader::Compile(const GLchar* vertexShaderCode, const GLchar* fragmentShaderCode, const GLchar* geometryShaderCode)
{
	GLuint vertexShader, fragmentShader, geometryShader;
	//顶点着色器
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	CheckCompileErrors(vertexShader, "VERTEX");
	//片元着色器
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	CheckCompileErrors(fragmentShader, "FRAGMENT");
	//几何着色器
	if (geometryShaderCode != nullptr) {
		geometryShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(geometryShader, 1, &geometryShaderCode, NULL);
		glCompileShader(geometryShader);
		CheckCompileErrors(geometryShader, "GEOMETRY");
	}
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	if (geometryShaderCode != nullptr) {
		glAttachShader(ID, geometryShader);
	}
	glLinkProgram(ID);
	CheckCompileErrors(ID,"PROGRAM");
	//链接完成后删除shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometryShaderCode != nullptr) {
		glDeleteShader(geometryShader);
	}
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

void Shader::SetVec2f(const std::string& inname, glm::vec2 invalue) const
{
	glUniform2f(glGetUniformLocation(ID, inname.c_str()), invalue[0], invalue[1]);
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
	glUniformMatrix4fv(glGetUniformLocation(ID, inname.c_str()), 1, GL_FALSE, glm::value_ptr(invalue));
}

void Shader::SetMat4f(const std::string& inname, int bTransp, glm::mat4 invalue) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, inname.c_str()), 1, bTransp, glm::value_ptr(invalue));
}

void Shader::CheckCompileErrors(GLuint object, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "Shader: Compile-time error: Type" << type << "\n" << infoLog << "\n-------------------------------" << std::endl;
		}
		else{
			glGetShaderiv(object, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(object, 1024, NULL, infoLog);
				std::cout << "Shader: Link-time error: Type" << type << "\n" << infoLog << "\n-------------------------------" << std::endl;
			}
		}
	}
}



