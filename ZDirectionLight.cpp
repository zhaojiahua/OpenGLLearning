#include "ZDirectionLight.h"

ZDirectionLight::ZDirectionLight(const glm::vec4& inColor, const glm::vec3& inDirection)
{
	mLightColor = inColor;
	mLightDirection = inDirection;
}

void ZDirectionLight::SetLight(Shader* inshader, const unsigned int& index, const float& ambientFactor, const float& diffuseFactor, const glm::vec4& specularColor)
{
	inshader->SetVec4f(std::string("dirlights[").append(std::to_string(index)).append("].ambient"), mLightColor * ambientFactor);
	inshader->SetVec4f(std::string("dirlights[").append(std::to_string(index)).append("].diffuse"), mLightColor * diffuseFactor);
	inshader->SetVec4f(std::string("dirlights[").append(std::to_string(index)).append("].specular"), specularColor);
	inshader->SetVec3f(std::string("dirlights[").append(std::to_string(index)).append("].direction"), mLightDirection);
}
