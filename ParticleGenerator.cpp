#include "ParticleGenerator.h"

void ParticleGenerator::GenParticles()
{
	
}

GLuint ParticleGenerator::FirstDiedParticle(GLuint lastDiedP)
{
	for (GLuint i = lastDiedP; i < particles.size(); i++) {
		if (particles[i].mLife <= 0.0f) return i;
	}
	for (GLuint i = 0; i < lastDiedP; i++) {
		if (particles[i].mLife <= 0.0f) return i;
	}
	return 0;//如果没有找到死亡的粒子就返回第一个粒子
}

void ParticleGenerator::RespawnParticle(Particle& particle, GameObject& obj, glm::vec2 offset)
{
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5f + (rand() % 100 / 100.0f);
	particle.mPosition = obj.position + random + offset;
	particle.mColor = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.mLife = 1.0f;//////////////////////默认生成新粒子的生命为1.0
	particle.mVelocity = obj.velocity * 0.2f;
}

void ParticleGenerator::InitRenderData()
{
	for (GLuint i = 0; i < mParticleNum; i++) {
		particles.push_back(Particle());
	}
	GLuint mVBO;
	//1X1的小四边形
	GLfloat vertices[] = {
		//position	//uv
		0.0f,1.0f,		0.0f,1.0f,
		1.0f,0.0f,		1.0f,0.0f,
		0.0f,0.0f,		0.0f,0.0f,

		0.0f,1.0f,		0.0f,1.0f,
		1.0f,1.0f,		1.0f,1.0f,
		1.0f,0.0f,		1.0f,0.0f
	};
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(mVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	//解绑
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ParticleGenerator::ParticleGenerator(Shader& inshader, Texture2D& inTex, GLuint particleNum) :mShader(inshader), mTexture(inTex), mParticleNum(particleNum)
{
	InitRenderData();
}

void ParticleGenerator::UpdataParticle(GLfloat dt, GLuint newP_num, GameObject obj, glm::vec2 offset)
{
	//第一个循环主要负责查找死亡粒子并更新死亡粒子
	for (GLuint i = 0; i < newP_num; i++) {
		GLuint lastDiedParticle = 0;
		GLuint firstdiedParticle = FirstDiedParticle(lastDiedParticle);
		lastDiedParticle = firstdiedParticle;//如果查找到了一个死亡的粒子,把索引位置定位到这个粒子的位置,这样做为了提升性能
		RespawnParticle(particles[firstdiedParticle], obj, offset);//查找到一个死亡的粒子后就更新这个粒子的属性
	}
	//第二个循环主要负责更新或者的粒子属性
	//随着时间的变化更新粒子的生命值和alpha值,使其随着时间的变化逐渐消失
	for (GLuint i = 0; i < particles.size(); i++) {
		Particle& tempP = particles[i];
		tempP.mLife -= dt;	//逐帧减少生命值
		if (tempP.mLife > 0.0) {
			tempP.gravity_V.y -= gravity * dt;
			tempP.mPosition -= (tempP.mVelocity + tempP.gravity_V) * dt;//粒子向与小球相反的方向发射(并且受重力影响)
			tempP.mColor.a -= dt * 0.2f;//逐帧减少不透明度
		}
	}
}

void ParticleGenerator::Draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	mShader.Use();
	//std::cout << particles.size() << std::endl;
	for (Particle& p : particles) {
		if (p.mLife > 0.0f) {
			//std::cout << p.mColor.a << std::endl;
			mShader.SetVec2f("offset", p.mPosition);
			mShader.SetVec4f("spriteColor", p.mColor);
			mTexture.Bind();
			glBindVertexArray(mVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
