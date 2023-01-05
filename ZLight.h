#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <string>

#define DIRECTIONLIGHT 0
#define POINTLIGHT 1
#define SPOTLIGHT 2

class ZLight
{
public:
	glm::vec4 mLightColor = { 1.0f,1.0f,1.0f ,1.0f};
	glm::vec3 mLightPosition = { 0.0f,0.0f,0.0f };

	unsigned int mLightType = DIRECTIONLIGHT;

	virtual void SetLight(Shader* inshader,const unsigned int& index, const float& ambientFactor, const float& diffuseFactor, const glm::vec4& specularColor) = 0;

};

