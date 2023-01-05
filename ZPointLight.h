#pragma once
#include "ZLight.h"
class ZPointLight :    public ZLight
{
public:
	ZPointLight(const glm::vec4& inColor, const glm::vec3& inPosition, const float& inkc, const float& inkl, const float& inkq);

	float mKc;
	float mKl;
	float mKq;

	//配置点光源
	void SetLight(Shader* inshader, const unsigned int& index, const float& ambientFactor, const float& diffuseFactor, const glm::vec4& specularColor)override;
};

