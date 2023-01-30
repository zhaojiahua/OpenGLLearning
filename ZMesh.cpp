#include "stb_image.h"
#include "ZMesh.h"

#include <map>

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
		inshader->SetInt(("material.texture_diffuse1"), 0);
		inshader->SetInt(("material.texture_diffuse2"), 0);
		inshader->SetInt(("material.texture_diffuse3"), 0);
		inshader->SetInt(("material.texture_specular1"), 0);
		inshader->SetInt(("material.texture_specular2"), 0);
		inshader->SetInt(("material.texture_specular3"), 0);
		inshader->SetInt(("material.texture_normal1"), 0);
		inshader->SetInt(("material.texture_height1"), 0);
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
		}
	}
	else {//如果没有任何贴图数据的话就要启用默认漫反射颜色不至于场景完全黑暗
		//cout << "no texture" << endl;
		inshader->SetVec4f("tempdiffuse_", glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	}
	inshader->SetMat4f("m_matrix", inMMatrix * mMatrix);
	//绘制网格
	glBindVertexArray(mVAO);
	glDrawElements(DrawMode, mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void ZMesh::DeferredDraw(Shader* inshader, glm::mat4 inMMatrix, unsigned int DrawMode)
{
	if (mTextures.size() > 0) {
		inshader->SetInt("texture_diffuse", 0);
		inshader->SetInt("texture_specular", 0);
		inshader->SetInt("texture_normal", 0);
		//绑定相应的纹理单元
		for (unsigned int i = 0; i < mTextures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);//激活相应的纹理单元
			glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
			string name = mTextures[i].type;
			inshader->SetInt(name.c_str(), i);
		}
	}
	inshader->SetMat4f("m_matrix", inMMatrix * mMatrix);
	//绘制网格
	glBindVertexArray(mVAO);
	glDrawElements(DrawMode, mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void ZMesh::DrawWithShadow(Shader* inshader, glm::mat4 inMMatrix, unsigned int DrawMode,unsigned int shadowmap)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	unsigned int i = 0;
	if (mTextures.size() > 0) {
		//绑定相应的纹理单元
		for ( i = 0; i < mTextures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);//激活相应的纹理单元
			glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
			string num;
			string name = mTextures[i].type;
			if (name == "texture_diffuse")num = to_string(diffuseNr++);
			else if (name == "texture_specular")num = to_string(specularNr++);
			else if (name == "texture_normal")num = to_string(normalNr++);
			else if (name == "texture_height")num = to_string(heightNr++);
			inshader->SetInt(("material." + name + num).c_str(), i);
		}
	}
	else {//如果没有任何贴图数据的话就要启用默认漫反射颜色不至于场景完全黑暗
		//cout << "no texture" << endl;
		inshader->SetVec4f("tempdiffuse_", glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	}
	glActiveTexture(GL_TEXTURE0 + i + 1);
	glBindTexture(GL_TEXTURE_2D, shadowmap);
	inshader->SetInt("shadowMap", i + 1);
	inshader->SetMat4f("m_matrix", inMMatrix * mMatrix);
	//绘制网格
	glBindVertexArray(mVAO);
	glDrawElements(DrawMode, mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void ZMesh::DrawWithCubeShadow(Shader* inshader, glm::mat4 inMMatrix, unsigned int DrawMode, unsigned int shadowmap)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	unsigned int i = 0;
	if (mTextures.size() > 0) {
		inshader->SetInt(("material.texture_diffuse1"), 0);
		inshader->SetInt(("material.texture_diffuse2"), 0);
		inshader->SetInt(("material.texture_diffuse3"), 0);
		inshader->SetInt(("material.texture_specular1"), 0);
		inshader->SetInt(("material.texture_specular2"), 0);
		inshader->SetInt(("material.texture_specular3"), 0);
		inshader->SetInt(("material.texture_normal1"), 0);
		inshader->SetInt(("material.texture_height1"), 0);
		//绑定相应的纹理单元
		for (i = 0; i < mTextures.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);//激活相应的纹理单元
			glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
			string num;
			string name = mTextures[i].type;
			if (name == "texture_diffuse")num = to_string(diffuseNr++);
			else if (name == "texture_specular")num = to_string(specularNr++);
			else if (name == "texture_normal")num = to_string(normalNr++);
			else if (name == "texture_height")num = to_string(heightNr++);
			inshader->SetInt(("material." + name + num).c_str(), i);
		}
	}
	else {//如果没有任何贴图数据的话就要启用默认漫反射颜色不至于场景完全黑暗
		//cout << "no texture" << endl;
		inshader->SetVec4f("tempdiffuse_", glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	}
	glActiveTexture(GL_TEXTURE0 + i + 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowmap);
	inshader->SetInt("shadowMap", i + 1);
	inshader->SetMat4f("m_matrix", inMMatrix * mMatrix);
	//绘制网格
	glBindVertexArray(mVAO);
	glDrawElements(DrawMode, mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void ZMesh::DrawInstance(Shader* inshader, unsigned int amount, glm::mat4* inMMatrix, unsigned int DrawMode)
{
	//生成一个顶点缓冲对象(为顶点续属性)
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), inMMatrix, GL_STATIC_DRAW);
	glBindVertexArray(mVAO);
	//顶点属性(由于顶点属性的最大数据量是vec4,所以我们只能将mat4拆分成4个属性分别存放)
	GLsizei vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2*vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3*vec4Size));
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glBindVertexArray(0);

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
			inshader->SetMat4f("m_matrix", mMatrix);
		}
	}
	else {//如果没有任何贴图数据的话就要启用默认漫反射颜色不至于场景完全黑暗
		inshader->SetVec4f("tempdiffuse_", glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	}

	//绘制网格
	glBindVertexArray(mVAO);
	glDrawElementsInstanced(DrawMode, mIndices.size(), GL_UNSIGNED_INT, 0, amount);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void ZMesh::DrawDepth(Shader* inshader)
{
	inshader->SetMat4f("m_matrix", mMatrix);
	//绘制网格
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void ZMesh::SampleDraw(Shader* inshader)
{
	inshader->SetMat4f("m_matrix", glm::mat4(1.0f));
	//inshader->SetInt("diffTex", 0);
	//绘制网格
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

unsigned int ZMesh::TextureFromFile(const string& filename, bool gamma)
{
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		cout << "Texture failed to load at path: " << filename << endl;
		stbi_image_free(data);
	}
	return textureID;
}

void ZMesh::AddHeightMap(string heightmapPath)
{
	unsigned int texID = TextureFromFile(heightmapPath);
	ZTexture heiTex;
	heiTex.id = texID;
	heiTex.type = "texture_height";
	heiTex.path = heightmapPath;
	mTextures.push_back(heiTex);
}

void ZMesh::AddHeightMap(unsigned int texID)
{
	for (auto tex : mTextures) {
		if (tex.id == texID)return;
	}
	ZTexture diffTex;
	diffTex.id = texID;
	diffTex.type = "texture_diffuse";
	diffTex.path = "NO";
	mTextures.push_back(diffTex);
}

unsigned int ZMesh::GetVAO()
{
	return mVAO;
}

void ZMesh::PrintMM()
{
	cout << mMatrix[0][0] << " " << mMatrix[0][1] << " " << mMatrix[0][2] << " " << mMatrix[0][3] << endl;
	cout << mMatrix[1][0] << " " << mMatrix[1][1] << " " << mMatrix[1][2] << " " << mMatrix[1][3] << endl;
	cout << mMatrix[2][0] << " " << mMatrix[2][1] << " " << mMatrix[2][2] << " " << mMatrix[2][3] << endl;
	cout << mMatrix[3][0] << " " << mMatrix[3][1] << " " << mMatrix[3][2] << " " << mMatrix[3][3] << endl;
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
	vector<ZMesh>sorted;
	if (meshes.size() > 0) {
		sorted.push_back(meshes[0]);
		for (unsigned int i = 1; i < meshes.size(); i++) {//根据distance的键值从低到高存储每一个mesh(根据distance距离值由大到小排列)
			float distance = glm::length(cameraPosition - meshes[i].mPosition);
			//(根据distance距离值由大到小排列)
			vector<ZMesh>::iterator temp_it = sorted.begin();
			while (true) {
				float old_distance = glm::length(cameraPosition - temp_it->mPosition);
				if (distance >= old_distance) {
					sorted.insert(temp_it, meshes[i]);
					break;//跳出循环
				}
				if (temp_it == sorted.end()) {//说明走到了最后
					sorted.push_back(meshes[i]);
					break;//跳出循环
				}
				temp_it++;
			}
			}
		}
		//顺序绘制每一个mesh
		//cout << sorted.size() << endl;
		for (vector<ZMesh>::iterator rit = sorted.begin(); rit != sorted.end(); rit++) {
			rit->Draw(inshader, inMMatrix, DrawMode);
		}
}

void ZModel::DeferredDraw(Shader* inshader, glm::mat4 inMMatrix, glm::vec3 cameraPosition, unsigned int DrawMode)
{
	//给物体按距离排序,渲染的时候由远及近以此绘制
	vector<ZMesh>sorted;
	if (meshes.size() > 0) {
		sorted.push_back(meshes[0]);
		for (unsigned int i = 1; i < meshes.size(); i++) {//根据distance的键值从低到高存储每一个mesh(根据distance距离值由大到小排列)
			float distance = glm::length(cameraPosition - meshes[i].mPosition);
			//(根据distance距离值由大到小排列)
			vector<ZMesh>::iterator temp_it = sorted.begin();
			while (true) {
				float old_distance = glm::length(cameraPosition - temp_it->mPosition);
				if (distance >= old_distance) {
					sorted.insert(temp_it, meshes[i]);
					break;//跳出循环
				}
				if (temp_it == sorted.end()) {//说明走到了最后
					sorted.push_back(meshes[i]);
					break;//跳出循环
				}
				temp_it++;
			}
		}
	}
	//顺序绘制每一个mesh
	//cout << sorted.size() << endl;
	for (vector<ZMesh>::iterator rit = sorted.begin(); rit != sorted.end(); rit++) {
		rit->DeferredDraw(inshader, inMMatrix, DrawMode);
	}
}

void ZModel::DrawWithShadow(Shader* inshader, glm::mat4 inMMatrix, glm::vec3 cameraPosition, unsigned int DrawMode, unsigned int shadowmap)
{
	//给物体按距离排序,渲染的时候由远及近以此绘制
	vector<ZMesh>sorted;
	if (meshes.size() > 0) {
		sorted.push_back(meshes[0]);
		for (unsigned int i = 1; i < meshes.size(); i++) {//根据distance的键值从低到高存储每一个mesh(根据distance距离值由大到小排列)
			float distance = glm::length(cameraPosition - meshes[i].mPosition);
			//(根据distance距离值由大到小排列)
			vector<ZMesh>::iterator temp_it = sorted.begin();
			while (true) {
				float old_distance = glm::length(cameraPosition - temp_it->mPosition);
				if (distance >= old_distance) {
					sorted.insert(temp_it, meshes[i]);
					break;//跳出循环
				}
				if (temp_it == sorted.end()) {//说明走到了最后
					sorted.push_back(meshes[i]);
					break;//跳出循环
				}
				temp_it++;
			}
		}
	}
	//顺序绘制每一个mesh
	//cout << sorted.size() << endl;
	for (vector<ZMesh>::iterator rit = sorted.begin(); rit != sorted.end(); rit++) {
		rit->DrawWithShadow(inshader, inMMatrix, DrawMode, shadowmap);
	}
}

void ZModel::DrawWithCubeShadow(Shader* inshader, glm::mat4 inMMatrix, glm::vec3 cameraPosition, unsigned int DrawMode, unsigned int shadowmap)
{
	//给物体按距离排序,渲染的时候由远及近以此绘制
	vector<ZMesh>sorted;
	if (meshes.size() > 0) {
		sorted.push_back(meshes[0]);
		for (unsigned int i = 1; i < meshes.size(); i++) {//根据distance的键值从低到高存储每一个mesh(根据distance距离值由大到小排列)
			float distance = glm::length(cameraPosition - meshes[i].mPosition);
			//(根据distance距离值由大到小排列)
			vector<ZMesh>::iterator temp_it = sorted.begin();
			while (true) {
				float old_distance = glm::length(cameraPosition - temp_it->mPosition);
				if (distance >= old_distance) {
					sorted.insert(temp_it, meshes[i]);
					break;//跳出循环
				}
				if (temp_it == sorted.end()) {//说明走到了最后
					sorted.push_back(meshes[i]);
					break;//跳出循环
				}
				temp_it++;
			}
		}
	}
	//顺序绘制每一个mesh
	//cout << sorted.size() << endl;
	for (vector<ZMesh>::iterator rit = sorted.begin(); rit != sorted.end(); rit++) {
		rit->DrawWithCubeShadow(inshader, inMMatrix, DrawMode, shadowmap);
	}
}

void ZModel::DrawInstance(Shader* inshader, unsigned int amount, glm::mat4* inMMatrix, glm::vec3 cameraPosition, unsigned int DrawMode)
{
	//给物体按距离排序,渲染的时候由远及近以此绘制
	vector<ZMesh>sorted;
	if (meshes.size() > 0) {
		sorted.push_back(meshes[0]);
		for (unsigned int i = 1; i < meshes.size(); i++) {//根据distance的键值从低到高存储每一个mesh(根据distance距离值由大到小排列)
			float distance = glm::length(cameraPosition - meshes[i].mPosition);
			//(根据distance距离值由大到小排列)
			vector<ZMesh>::iterator temp_it = sorted.begin();
			while (true) {
				float old_distance = glm::length(cameraPosition - temp_it->mPosition);
				if (distance >= old_distance) {
					sorted.insert(temp_it, meshes[i]);
					break;//跳出循环
				}
				if (temp_it == sorted.end()) {//说明走到了最后
					sorted.push_back(meshes[i]);
					break;//跳出循环
				}
				temp_it++;
			}
		}
		//顺序绘制每一个mesh
		//cout << sorted.size() << endl;
		for (vector<ZMesh>::iterator rit = sorted.begin(); rit != sorted.end(); rit++) {
			rit->DrawInstance(inshader, amount, inMMatrix, DrawMode);
		}
	}
}

void ZModel::DrawDepth(Shader* inshader)
{
	//cout << sorted.size() << endl;
	for (vector<ZMesh>::iterator rit = meshes.begin(); rit != meshes.end(); rit++) {
		rit->DrawDepth(inshader);
	}
}


void ZModel::SampleDraw(Shader* inshader)
{
	for (vector<ZMesh>::iterator rit = meshes.begin(); rit != meshes.end(); rit++) {
		rit->SampleDraw(inshader);
	}
}

unsigned int ZModel::LoadCubeMap(vector<string> textures_faces)
{
	unsigned int texture_box;
	glGenTextures(1, &texture_box);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_box);
	//遍历纹理目标
	int texC_width, texC_height, texC_nrChannels;
	for (unsigned int i = 0; i < textures_faces.size(); i++) {
		unsigned char* tempdata = stbi_load(textures_faces[i].c_str(), &texC_width, &texC_height, &texC_nrChannels, 0);
		if (tempdata) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, texC_width, texC_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempdata);
		}
		else cout << "Cubemap texture failed to load at path:" << textures_faces[i] << endl;
		stbi_image_free(tempdata);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return texture_box;
}

void ZModel::LoadModel(string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);//以三角形读取,翻转UV的V向,并计算切线和副切线
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
	nodeM=glm::transpose(nodeM);
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
		vertex.tangent.x = mesh->mTangents[i].x;//切线
		vertex.tangent.y = mesh->mTangents[i].y;
		vertex.tangent.z = mesh->mTangents[i].z;
		vertex.bitangent.x = mesh->mBitangents[i].x;//副切线
		vertex.bitangent.y = mesh->mBitangents[i].y;
		vertex.bitangent.z = mesh->mBitangents[i].z;
		if (mesh->HasNormals()) {
			vertex.normal.x = mesh->mNormals[i].x;//法线
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
		//if (diffuseTexs.size())cout << "diffuseTexs: " << diffuseTexs.size() << endl;
		meshTextures.insert(meshTextures.end(), diffuseTexs.begin(), diffuseTexs.end());
		vector<ZTexture> specularTexs = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		//if (diffuseTexs.size())cout << "specularTexs: " << specularTexs.size() << endl;
		meshTextures.insert(meshTextures.end(), specularTexs.begin(), specularTexs.end());
		vector<ZTexture> normalTexs = LoadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
		//if (diffuseTexs.size())cout << "normalTexs: " << normalTexs.size() << endl;
		meshTextures.insert(meshTextures.end(), normalTexs.begin(), normalTexs.end());
		vector<ZTexture> ambientTexs = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_ambient");
		//if (diffuseTexs.size())cout << "ambientTexs: " << ambientTexs.size() << endl;
		meshTextures.insert(meshTextures.end(), ambientTexs.begin(), ambientTexs.end());
		vector<ZTexture> emissiveTexs = LoadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emissive");
		//if (diffuseTexs.size())cout << "emissiveTexs: " << emissiveTexs.size() << endl;
		meshTextures.insert(meshTextures.end(), emissiveTexs.begin(), emissiveTexs.end());
		vector<ZTexture> heightTexs = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
		//if (diffuseTexs.size())cout << "heightTexs: " << heightTexs.size() << endl;
		meshTextures.insert(meshTextures.end(), heightTexs.begin(), heightTexs.end());
	}
	ZMesh tempMesh= ZMesh(meshVertices, meshIndices, meshTextures);
	tempMesh.mMatrix = nodeM;
	tempMesh.mPosition = mposition;
	return tempMesh;
}

unsigned int ZModel::TextureFromFile(const char* path, const string& directory, bool gamma)
{
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
