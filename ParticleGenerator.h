#pragma once
#include "Shader.h"
#include "Texture2D.h"
#include "GameObject.h"

#include <vector>

struct Particle
{
	glm::vec2 mPosition, mVelocity;
	glm::vec4 mColor;
	GLfloat mLife;
	glm::vec2 gravity_V = glm::vec2(0.0f);//受重力影响的向下下落速度

	Particle() : mPosition(0.0f), mVelocity(0.0f), mColor(1.0f), mLife(0.0f) {};
};

class ParticleGenerator
{
public:
	Shader mShader;
	Texture2D mTexture;
	GLuint mParticleNum;
	GLfloat gravity = 9.8f;
	
	ParticleGenerator() {};
	ParticleGenerator(Shader& inshader, Texture2D& inTex, GLuint particleNum);

	//每帧调用更新粒子
	void UpdataParticle(GLfloat dt, GLuint newP_num, GameObject obj, glm::vec2 offset);
	//渲染粒子
	void Draw();

private:
	GLuint mVAO;
	std::vector<Particle>particles;

	//生成粒子(给定粒子的数量)
	void GenParticles();
	//从起始位置开始,从左向右检索粒子列表,找到一个死亡的粒子然后返回这个粒子的序号
	GLuint FirstDiedParticle(GLuint lastDiedP);
	//给定粒子,更新这个粒子的属性(粒子的位置跟随obj,并且有一定的偏移)
	void RespawnParticle(Particle& particle, GameObject& obj, glm::vec2 offset);
	//初始化渲染数据
	void InitRenderData();
};

