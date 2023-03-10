#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class Shader
{
public:
	//着色器程序ID
	unsigned int ID;

	//构造shader类的时候读取shader代码并构建着色器
	Shader();
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath);

	//生成顶点着色器
	GLint GenVertexShader(const char* vertexShaderPath);
	//生成几何着色器
	GLint GenGeometryShader(const char* geometryShaderPath);
	//生成片元着色器
	GLint GenFragmentShader(const char* fragmentShaderPath);
	//根据shader代码生成并编译着色器
	void Compile(const GLchar* vertexShaderCode, const GLchar* fragmentShaderCode, const GLchar* geometryShaderCode);

	//激活程序
	void Use();

	//uniform工具函数
	void SetBool(const std::string& inname, bool invalue) const;
	bool GetBool(const std::string& inname) const;
	void SetInt(const std::string& inname, int invalue) const;
	int GetInt(const std::string& inname) const;
	void SetFloat(const std::string& inname, float invalue) const;
	float GetFloat(const std::string& inname) const;
	void SetVec2f(const std::string& inname, glm::vec2 invalue) const;
	void SetVec3f(const std::string& inname, float invalue[3]) const;
	void SetVec3f(const std::string& inname, glm::vec3 invalue) const;
	void SetVec4f(const std::string& inname, float invalue[4]) const;
	void SetVec4f(const std::string& inname, glm::vec4 invalue) const;
	void SetMat4f(const std::string& inname, glm::mat4 invalue) const;
	void SetMat4f(const std::string& inname, int bTransp, glm::mat4 invalue) const;

private:
	//检查shader的编译链接等环节是否出错
	void CheckCompileErrors(GLuint object, std::string type);
};