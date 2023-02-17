#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image.h"

map<string, Shader> ResourceManager::Shaders;
map<string, Texture2D> ResourceManager::Texture2Ds;

Shader ResourceManager::LoadShader(const GLchar* vshaderFile, const GLchar* fshaderFile, const GLchar* gshaderFile, string shadername)
{
	Shaders[shadername] = LoadShaderFromFile(vshaderFile, fshaderFile, gshaderFile);
	return Shaders[shadername];
}

Shader ResourceManager::GetShader(string shadername)
{
	return Shaders[shadername];
}

Texture2D ResourceManager::LoadTexture2D(const GLchar* imageFile, GLboolean typefloat, string texturename)
{
	Texture2Ds[texturename] = LoadTextureFromFile(imageFile, typefloat);
	return Texture2Ds[texturename];
}

Texture2D ResourceManager::GetTexture2D(string texturename)
{
	return Texture2Ds[texturename];
}

void ResourceManager::Clear()
{
	for (auto shader : Shaders) {
		glDeleteProgram(shader.second.ID);
	}
	for (auto tex : Texture2Ds) {
		glDeleteTextures(1, &tex.second.ID);
	}
}

Shader ResourceManager::LoadShaderFromFile(const GLchar* vshaderFile, const GLchar* fshaderFile, const GLchar* gshaderFile)
{
	string vshaderCode, fshaderCode, gshaderCode;
	try
	{
		//根据路径打开文件
		ifstream vertexShaderFile(vshaderFile);
		ifstream fragmentShaderFile(fshaderFile);
		//将文件里的内容存储到内存
		stringstream vertexShaderStream, fragmentShaderStream;
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		//关闭文件
		vertexShaderFile.close();
		fragmentShaderFile.close();
		//得到string格式
		vshaderCode = vertexShaderStream.str();
		fshaderCode = fragmentShaderStream.str();
		//如果有几何着色器的就处理生成几何着色器
		if (gshaderFile != nullptr) {
			ifstream geometryShaderFile(gshaderFile);
			stringstream geometryShaderStream;
			geometryShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			gshaderCode = geometryShaderStream.str();
		}
	}
	catch (const exception e)
	{
		cout << "Shader::Read shader file failed!" << endl;
	}
	Shader shader;
	shader.Compile(vshaderCode.c_str(), fshaderCode.c_str(), gshaderFile != nullptr ? gshaderCode.c_str() : nullptr);
	return shader;
}

Texture2D ResourceManager::LoadTextureFromFile(const GLchar* imageFile, GLboolean floattype)
{
	Texture2D texture;
	GLint width, height, nrComponents;
	if (floattype) {
		float* data_f = stbi_loadf(imageFile, &width, &height, &nrComponents, 0);
		texture.SetFormat(nrComponents);
		texture.Generate_f(width, height, data_f);
		stbi_image_free(data_f);
	}
	else {
		unsigned char* data = stbi_load(imageFile, &width, &height, &nrComponents, 0);
		texture.SetFormat(nrComponents);
		texture.Generate(width, height, data);
		stbi_image_free(data);
	}
	
	texture.floatType = floattype;
	return texture;
}
