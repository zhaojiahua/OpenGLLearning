#pragma once
#include "GameObject.h"
class BallObject :    public GameObject
{
public:
	GLfloat mRadius;
	GLboolean mStuck;
	GLboolean mSticky;
	GLboolean mPassThrough;

	BallObject();
	BallObject(glm::vec2 inposition, GLfloat inradius, glm::vec2 invelocity, Texture2D inTexure);

	glm::vec2 Move(GLfloat dt, GLuint winWidth);
	void Reset(glm::vec2 inposition, glm::vec2 invelocity);
};

