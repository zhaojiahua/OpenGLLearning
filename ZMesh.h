#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

#include "Shader.h"

using namespace std;
//自定义一个顶点数据包含位置发现和uv信息
struct ZVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};
//自定义一个texture结构
struct ZTexture
{
	unsigned int id;
	string type;
	aiString path;//储存纹理的路径用于与其他纹理进行比较
};
class ZMesh
{
public:
	vector<ZVertex> mVertices;//顶点数据
	vector<unsigned int>mIndices;//顶点索引数据
	vector<ZTexture>mTextures;//纹理图片数据
	glm::mat4 mMatrix = glm::mat4(1.0f);//模型固有的矩阵数据
	glm::vec3 mPosition = { 0.0f ,0.0f,0.0f };//mesh的空间位置

	//构造函数
	ZMesh();
	ZMesh(vector<ZVertex> vertices, vector<unsigned int>indices, vector<ZTexture>textures);
	//绘制函数
	void Draw(Shader* inshader, glm::mat4 inMMatrix, unsigned int DrawMode);
	//绘制带阴影
	void DrawWithShadow(Shader* inshader, glm::mat4 inMMatrix, unsigned int DrawMode,unsigned int shadowmap);
	//绘制带阴影(立方体阴影深度贴图)
	void DrawWithCubeShadow(Shader* inshader, glm::mat4 inMMatrix, unsigned int DrawMode, unsigned int shadowmap);
	//通过实例化绘制(绘制量多的情况)
	void DrawInstance(Shader* inshader, unsigned int amount, glm::mat4* inMMatrix, unsigned int DrawMode);
	//渲染阴影深度贴图
	void DrawDepth(Shader* inshader);
	//忽略贴图,只读取场景中的顶点位置信息绘制
	void SampleDraw(Shader* inshader);

	unsigned int GetVAO();

	//打印矩阵(用于调试)
	void PrintMM();

private:
	//渲染数据内存地址数据
	unsigned int mVAO, mVBO, mEBO;
	//绑定和配置mesh的渲染缓冲数据
	void SetupMesh();
};
class ZModel 
{
public:
	ZModel(const char* path);
	void Draw(Shader* inshader, glm::mat4 inMMatrix, glm::vec3 cameraPosition,unsigned int DrawMode);
	void DrawWithShadow(Shader* inshader, glm::mat4 inMMatrix, glm::vec3 cameraPosition, unsigned int DrawMode, unsigned int shadowmap);
	void DrawWithCubeShadow(Shader* inshader, glm::mat4 inMMatrix, glm::vec3 cameraPosition, unsigned int DrawMode, unsigned int shadowmap);
	void DrawInstance(Shader* inshader, unsigned int amount, glm::mat4* inMMatrix, glm::vec3 cameraPosition, unsigned int DrawMode);
	void DrawDepth(Shader* inshader);
	void SampleDraw(Shader* inshader);

	vector<ZTexture> textures_loaded;
	//模型数据
	vector<ZMesh> meshes;
	string directory;

	//加载立方体贴图(方盒子贴图)(需要提供方盒子的六个面的贴图路径,注意顺序一定是"右左上下前后")
	unsigned int LoadCubeMap(vector<string> textures_faces);

private:
	//加载模型数据
	void LoadModel(string path);
	//处理模型节点
	void ProcessNode(aiNode* node,const aiScene* scene);
	//处理mesh
	ZMesh ProcessMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 nodeM, glm::vec3 mposition);
	//从图片文件加载纹理
	unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);
	//加载纹理图片数据
	vector<ZTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

