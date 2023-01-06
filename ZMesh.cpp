#include "stb_image.h"
#include "ZMesh.h"

#include <map>

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma=false) {
	string filename = string(path);
	filename = directory + '/' + filename;
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)format = GL_RED;
		else if (nrComponents == 3)format = GL_RGB;
		else if (nrComponents == 4)format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		cout << "Texture failed to load at path: " << path << endl;
		stbi_image_free(data);
	}
	return textureID;
}

ZMesh::ZMesh(){
	
}

ZMesh::ZMesh(vector<ZVertex> vertices, vector<unsigned int> indices, vector<ZTexture> textures)
{
	this->mVertices = vertices;
	this->mIndices = indices;
	this->mTextures = textures;
	SetupMesh();
}

void ZMesh::Draw(Shader* inshader,glm::mat4 inMMatrix,unsigned int DrawMode)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	
	if (mTextures.size() > 0) {
		//绑定相应的纹理单元
		for (unsigned int i = 0; i < mTextures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);//激活相应的纹理单元
			glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
			string num;
			string name = mTextures[i].type;
			if (name == "texture_diffuse")num = to_string(diffuseNr++);
			else if (name == "texture_specular")num = to_string(specularNr++);
			else if (name == "texture_normal")num = to_string(normalNr++);
			else if (name == "texture_height")num = to_string(heightNr++);
			inshader->SetInt(("material." + name + num).c_str(), i);
			inshader->SetMat4f("m_matrix", inMMatrix * MMatrix);
		}
	}
	else {//如果没有任何贴图数据的话就要启用默认漫反射颜色不至于场景完全黑暗
		inshader->SetVec4f("tempdiffuse_", glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	}
	
	//绘制网格
	glBindVertexArray(mVAO);
	glDrawElements(DrawMode, mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void ZMesh::PrintMM()
{
	cout << MMatrix[0][0] << " " << MMatrix[0][1] << " " << MMatrix[0][2] << " " << MMatrix[0][3] << endl;
	cout << MMatrix[1][0] << " " << MMatrix[1][1] << " " << MMatrix[1][2] << " " << MMatrix[1][3] << endl;
	cout << MMatrix[2][0] << " " << MMatrix[2][1] << " " << MMatrix[2][2] << " " << MMatrix[2][3] << endl;
	cout << MMatrix[3][0] << " " << MMatrix[3][1] << " " << MMatrix[3][2] << " " << MMatrix[3][3] << endl;
}

void ZMesh::SetupMesh()
{
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);
	glGenVertexArrays(1, &mVAO);
	
	//绑定缓冲内存数据
	glBindVertexArray(mVAO);
	//填充顶点数据
	glBindBuffer(GL_ARRAY_BUFFER,mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(ZVertex), &mVertices[0], GL_STATIC_DRAW);
	//填充顶点索引数据
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);
	//设置属性指针
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ZVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ZVertex), (void*)offsetof(ZVertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ZVertex), (void*)offsetof(ZVertex, uv));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ZVertex), (void*)offsetof(ZVertex, tangent));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(ZVertex), (void*)offsetof(ZVertex, bitangent));
	//解绑
	glBindVertexArray(0);
}

ZModel::ZModel(const char* path)
{
	LoadModel(path);
}

void ZModel::Draw(Shader* inshader, glm::mat4 inMMatrix,glm::vec3 cameraPosition,unsigned int DrawMode)
{
	//给物体按距离排序,渲染的时候由远及近以此绘制
	map<float, ZMesh>sorted;
	//cout << meshes.size() << endl;
	for (unsigned int i = 0; i < meshes.size(); i++) {
		float distance = glm::length(cameraPosition - meshes[i].mPosition);
		sorted[distance] = meshes[i];//根据distance的键值从低到高存储每一个mesh
	}
	//逆序绘制每一个mesh
	for (map<float, ZMesh>::reverse_iterator rit = sorted.rbegin(); rit != sorted.rend(); rit++) {
		rit->second.Draw(inshader, inMMatrix, DrawMode);
	}
	//for (auto mesh : meshes) {
	//	mesh.Draw(inshader, inMMatrix);
	//}
}

void ZModel::LoadModel(string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));//截取模型文件路径的目录
	ProcessNode(scene->mRootNode, scene);
}

void ZModel::ProcessNode(aiNode* node, const aiScene* scene)
{
	aiMatrix4x4 nodetransform = node->mTransformation;
	aiVector3D s, r, t;
	nodetransform.Decompose(s, r, t);
	glm::vec3 mPosition(t.x, t.y, t.z);
	glm::mat4 nodeM = {
		nodetransform.a1 , nodetransform.a2   ,nodetransform.a3 ,nodetransform.a4 ,
		nodetransform.b1 , nodetransform.b2  , nodetransform.b3, nodetransform.b4,
		nodetransform.c1 , nodetransform.c2   ,nodetransform.c3 ,nodetransform.c4 ,
		nodetransform.d1 , nodetransform.d2  , nodetransform.d3, nodetransform.d4
	};
	//处理节点所有的网格
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene, nodeM, mPosition));
	}
	//递归处理子节点
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

ZMesh ZModel::ProcessMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 nodeM,glm::vec3 mposition)
{
	vector<ZVertex>meshVertices;
	vector<unsigned int>meshIndices;
	vector<ZTexture>meshTextures;
	//处理顶点数据
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		ZVertex vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;
		if (mesh->HasNormals()) {
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}
		if (mesh->mTextureCoords[0]) {
			vertex.uv.x = mesh->mTextureCoords[0][i].x;
			vertex.uv.y = mesh->mTextureCoords[0][i].y;
		}
		meshVertices.push_back(vertex);
	}
	//处理索引数据
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			meshIndices.push_back(face.mIndices[j]);
		}
	}
	//处理材质
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<ZTexture> diffuseTexs = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		meshTextures.insert(meshTextures.end(), diffuseTexs.begin(), diffuseTexs.end());
		vector<ZTexture> specularTexs = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		meshTextures.insert(meshTextures.end(), specularTexs.begin(), specularTexs.end());
		vector<ZTexture> normalTexs = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		meshTextures.insert(meshTextures.end(), normalTexs.begin(), normalTexs.end());
		vector<ZTexture> heightTexs = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		meshTextures.insert(meshTextures.end(), heightTexs.begin(), heightTexs.end());
	}
	ZMesh tempMesh= ZMesh(meshVertices, meshIndices, meshTextures);
	tempMesh.MMatrix = nodeM;
	tempMesh.mPosition = mposition;
	return tempMesh;
}

vector<ZTexture> ZModel::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<ZTexture> tempresult;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(str.C_Str(), textures_loaded[j].path.C_Str()) == 0) {
				tempresult.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			ZTexture tempTex;
			tempTex.id = TextureFromFile(str.C_Str(), directory);
			tempTex.type = typeName;
			tempTex.path = str.C_Str();
			tempresult.push_back(tempTex);
			textures_loaded.push_back(tempTex);
		}
	}
	return tempresult;
}
