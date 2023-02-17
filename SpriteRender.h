#pragma once
#include "Shader.h"
#include "Texture2D.h"

//专门渲染精灵的一个类
class SpriteRender
{
public:
	SpriteRender(Shader& shader);
	~SpriteRender();
	void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(1.0f), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	Shader mShader;
	GLuint mVAO;

	void InitRenderData();
};

