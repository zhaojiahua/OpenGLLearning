#pragma once
#include "GameObject.h"

const glm::vec2 SIZE(50, 50);
const glm::vec2 VELOCITY(0.0f, 150.0f);
//道具类(一个具有额外状态的GameObject类)
class PowerUp : public GameObject
{
public:
	std::string mType;
	GLfloat mDuration;
	GLboolean mActivated;
	PowerUp(std::string intype,glm::vec3 incolor,GLfloat induration,glm::vec2 inposition,Texture2D intex)
		:GameObject(inposition, SIZE,intex,incolor,VELOCITY),mType(intype),mDuration(induration),mActivated()
	{};
};

