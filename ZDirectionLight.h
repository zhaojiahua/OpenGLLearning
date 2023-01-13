#pragma once
#include "ZLight.h"
class ZDirectionLight :    public ZLight
{
public:
	ZDirectionLight(const glm::vec4& inColor, const glm::vec3& inDirection);

	glm::vec3 mLightDirection = glm::vec3(-1.0f, 0.0f, 0.0f);//默认平行光照向x轴负方向

	//配置平行光
	virtual void SetLight(Shader* inshader, const unsigned int& index, const float& ambientFactor, const float& diffuseFactor, const glm::vec4& specularColor)override;
};

