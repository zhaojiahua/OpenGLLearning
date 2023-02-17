#pragma once

#include "SpriteRender.h"
#include "Texture2D.h"

//游戏中每个物体都是这个类的派生类
class GameObject
{
public:
	glm::vec2 position, size, velocity;
	glm::vec3 color;
	GLfloat rotation;
	GLboolean isSolid;
	GLboolean bDestroyed;
	Texture2D spriteTex;
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D spriteTex, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f));
	virtual void Draw(SpriteRender& spriterender);
};

