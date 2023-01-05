#include "ZSpotLight.h"

ZSpotLight::ZSpotLight(const glm::vec4& inColor, const float& incutoff, const float& inoutercutoff)
{
	mLightColor = inColor;

	mCutoff = incutoff;
	mOuterCutoff = inoutercutoff;

	mLightType = SPOTLIGHT;
}

void ZSpotLight::SetLight(Shader* inshader, const unsigned int& index, const float& ambientFactor, const float& diffuseFactor, const glm::vec4& specularColor)
{
	inshader->SetVec4f(std::string("spotlights[").append(std::to_string(index)).append("].ambient"), mLightColor * ambientFactor);
	inshader->SetVec4f(std::string("spotlights[").append(std::to_string(index)).append("].diffuse"), mLightColor * diffuseFactor);
	inshader->SetVec4f(std::string("spotlights[").append(std::to_string(index)).append("].specular"), specularColor);
	inshader->SetFloat(std::string("spotlights[").append(std::to_string(index)).append("].cutoff"), mCutoff);
	inshader->SetFloat(std::string("spotlights[").append(std::to_string(index)).append("].outercutoff"), mOuterCutoff);
}
