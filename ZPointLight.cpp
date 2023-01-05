#include "ZPointLight.h"

ZPointLight::ZPointLight(const glm::vec4& inColor, const glm::vec3& inPosition, const float& inkc, const float& inkl, const float& inkq)
{
	mLightColor = inColor;
	mLightPosition = inPosition;
	mKc = inkc;
	mKl = inkl;
	mKq = inkq;
	mLightType = POINTLIGHT;
}

void ZPointLight::SetLight(Shader* inshader, const unsigned int& index, const float& ambientFactor, const float& diffuseFactor, const glm::vec4& specularColor)
{
	inshader->SetVec3f(std::string("pointlights[").append(std::to_string(index)).append("].position"), mLightPosition);
	inshader->SetVec4f(std::string("pointlights[").append(std::to_string(index)).append("].ambient"), mLightColor * ambientFactor);
	inshader->SetVec4f(std::string("pointlights[").append(std::to_string(index)).append("].diffuse"), mLightColor * diffuseFactor);
	inshader->SetVec4f(std::string("pointlights[").append(std::to_string(index)).append("].specular"), specularColor);
	inshader->SetFloat(std::string("pointlights[").append(std::to_string(index)).append("].kc"), mKc);
	inshader->SetFloat(std::string("pointlights[").append(std::to_string(index)).append("].kl"), mKl);
	inshader->SetFloat(std::string("pointlights[").append(std::to_string(index)).append("].kq"), mKq);
}
