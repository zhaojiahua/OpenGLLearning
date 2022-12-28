#pragma once
#include <glad/glad.h>

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
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	//激活程序
	void Use();

	//uniform工具函数
	void SetBool(const std::string& inname, bool invalue) const;
	void SetInt(const std::string& inname, int invalue) const;
	void SetFloat(const std::string& inname, float invalue) const;
	void SetVec4f(const std::string& inname, float invalue[4]) const;
};