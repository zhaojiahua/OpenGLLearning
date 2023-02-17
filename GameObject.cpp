#include "GameObject.h"

GameObject::GameObject() : position(0, 0), size(1, 1), velocity(0.0f), color(1.0f), rotation(0.0f), spriteTex(), isSolid(GL_FALSE), bDestroyed(GL_FALSE)
{
}

GameObject::GameObject(glm::vec2 inpos, glm::vec2 insize, Texture2D inspriteTex, glm::vec3 incolor, glm::vec2 invelocity)
	: position(inpos), size(insize), velocity(invelocity), color(incolor), rotation(0.0f), spriteTex(inspriteTex), isSolid(GL_FALSE), bDestroyed(GL_FALSE)
{
}

void GameObject::Draw(SpriteRender& spriterender)
{
	spriterender.DrawSprite(spriteTex, position, size, rotation, color);
}
