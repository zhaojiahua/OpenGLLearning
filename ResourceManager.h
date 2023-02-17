#pragma once
#include <map>
#include <string>

#include <glad/glad.h>

#include "Texture2D.h"
#include "Shader.h"

using namespace std;
//(资源加载器)单例模式,专门负责加载游戏相关的资源
class ResourceManager
{
	//构造函数
	ResourceManager() {};

public:
	//资源存储
	//所有的shader
	static map<string, Shader> Shaders;
	//所有的纹理
	static map<string, Texture2D> Texture2Ds;
	//创建Shader并存储到Shaders中,如果已有就覆盖掉
	static Shader LoadShader(const GLchar* vshaderFile, const GLchar* fshaderFile, const GLchar* gshaderFile, string shadername);
	//通过名字从Shaders中获取Shader实例
	static Shader GetShader(string shadername);
	//创建Texture并存储到Texture2Ds中,如果已有就覆盖掉
	static Texture2D LoadTexture2D(const GLchar* imageFile, GLboolean typefloat, string texturename);
	//通过名字从Texture2Ds中获取Texture2D实例
	static Texture2D GetTexture2D(string texturename);
	//释放所有的资源(清理已有的Shader和Texture2D)
	static void Clear();

private:
	static Shader LoadShaderFromFile(const GLchar* vshaderFile, const GLchar* fshaderFile, const GLchar* gshaderFile = nullptr);
	static Texture2D LoadTextureFromFile(const GLchar* imageFile, GLboolean floattype = GL_FALSE);
};

