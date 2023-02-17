#include "BallObject.h"

BallObject::BallObject()
{}
BallObject::BallObject(glm::vec2 inposition, GLfloat inradius, glm::vec2 invelocity, Texture2D inTexure) :
	GameObject(inposition, glm::vec2(inradius * 2), inTexure, glm::vec3(1.0f), invelocity), mRadius(inradius), mStuck(GL_TRUE), mSticky(GL_FALSE), mPassThrough(GL_FALSE)
{}

glm::vec2 BallObject::Move(GLfloat dt, GLuint winWidth)
{
	if (!mStuck) {//如果没有被固定在挡板上
		position += velocity * dt;
		//检查边界
		//横坐标不能超出两边边界
		if (position.x <= 0.0f) {
			velocity.x *= -1;
			position.x = 0.0f;
		}
		else if (position.x + size.x >= winWidth) {
			velocity.x *= -1;
			position.x = winWidth - size.x;
		}
		//纵坐标不能超出上边界(触碰下边界游戏结束)
		if (position.y <= 0.0f) {
			velocity.y *= -1;
			position.y = 0.0f;
		}
	}
	return position;
}

void BallObject::Reset(glm::vec2 inposition, glm::vec2 invelocity)
{
	position = inposition;
	velocity = invelocity;
	color = glm::vec3(1.0f);
	mStuck = GL_TRUE;
}
