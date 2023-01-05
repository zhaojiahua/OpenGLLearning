#pragma once
#include "ZLight.h"
class ZSpotLight :    public ZLight
{
public:
	ZSpotLight(const glm::vec4& inColor, const float& incutoff,const float& inoutercutoff);
	

	float mCutoff;
	float mOuterCutoff;

	//配置聚光灯
	void SetLight(Shader* inshader, const unsigned int& index, const float& ambientFactor, const float& diffuseFactor, const glm::vec4& specularColor)override;
};

