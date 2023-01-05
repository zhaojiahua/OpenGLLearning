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

	int test = 1;

	//构造shader类的时候读取shader代码并构建着色器
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	//激活程序
	void Use();

	//uniform工具函数
	void SetBool(const std::string& inname, bool invalue) const;
	bool GetBool(const std::string& inname) const;
	void SetInt(const std::string& inname, int invalue) const;
	int GetInt(const std::string& inname) const;
	void SetFloat(const std::string& inname, float invalue) const;
	float GetFloat(const std::string& inname) const;
	void SetVec3f(const std::string& inname, float invalue[3]) const;
	void SetVec3f(const std::string& inname, glm::vec3 invalue) const;
	void SetVec4f(const std::string& inname, float invalue[4]) const;
	void SetVec4f(const std::string& inname, glm::vec4 invalue) const;
	void SetMat4f(const std::string& inname, glm::mat4 invalue) const;

};