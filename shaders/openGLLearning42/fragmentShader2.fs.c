#version 330 core

out vec4 fragColor;

in vec2 vertexUV;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D ssaoTex;

struct PointLight{
	vec3 position;
	vec4 color;
	float linear;
	float quadratic;
};
const int POINTLIGHTNUM=32;
uniform PointLight pointlights[POINTLIGHTNUM];
uniform vec3 viewPos;

void main()
{
	//从gBuffer里面获取数据
	vec3 fragPos=texture(gPosition,vertexUV).rgb;
	vec3 normal=texture(gNormal,vertexUV).rgb;
	vec4 albedo=texture(gAlbedoSpec,vertexUV);
	float ambientAO=texture(ssaoTex,vertexUV).r;
	float specular=texture(gAlbedoSpec,vertexUV).a;

	//计算光照
	vec4 ambient=albedo*0.2*ambientAO;//添加遮蔽因子贴图
	vec3 viewDir=normalize(viewPos-fragPos);
	for(int i=0;i<POINTLIGHTNUM;i++){
		vec3 lightDir=normalize(pointlights[i].position-fragPos);
		vec4 diffuse=max(dot(normal,lightDir),0.0)*albedo*pointlights[i].color;
		vec3 halfDir=normalize(lightDir+viewDir);
		float spec=pow(max(dot(normal,halfDir),0.0),32);//高光因子
		vec4 specularColor=pointlights[i].color*spec*specular;
		float distance=length(pointlights[i].position-fragPos);
		float attenuation=1.0/(1.0+pointlights[i].linear*distance+pointlights[i].quadratic*distance*distance);//点光源的衰减因子
		ambient+=((diffuse+specularColor)*attenuation);
	}
	fragColor=vec4(ambient.rgb,1.0);
	//fragColor=vec4(1.0,1.0,0.0,1.0);
}