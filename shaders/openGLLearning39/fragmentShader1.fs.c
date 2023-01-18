#version 330 core

//创建一个结构体来描述物体的材质属性
struct Material{
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
	sampler2D texture_specular3;
	sampler2D texture_normal1;
	sampler2D texture_height1;
	float shininess;
};
//平行光
struct DirLight{
	vec3 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};
//点光源
struct PointLight{
	vec3 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float kc;
	float kl;
	float kq;
};
//聚光灯
struct SpotLight{
	vec3 position;
	vec3 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float cutoff;
	float outercutoff;
};

in vec3 normal;
in vec2 vertexUV;
in vec3 fragPos;
in mat3 TBN;

out vec4 fragColor;

uniform float pointlight_far_plane;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec4 tempdiffuse_;
uniform Material material;
uniform samplerCube shadowMap;//用于采样立方体深度贴图
#define DIRECTOINLIGHT_NUM 1
#define POINTLIGHT_NUM 2
#define SPOTLIGHT_NUM 1
uniform DirLight dirlights[DIRECTOINLIGHT_NUM];
uniform PointLight pointlights[POINTLIGHT_NUM];
uniform SpotLight spotlights[SPOTLIGHT_NUM];

float CalculateShadowFactor(vec3 normal){//在阴影内的返回0.0,阴影外返回1.0
	vec3 fragToLight=fragPos-lightPos;//从片元指向点光源位置的向量,用于采样立方体深度贴图
	float nearestDepth=texture(shadowMap,fragToLight).r;
	nearestDepth*=pointlight_far_plane;//深度值被压缩到0-1之间了,这里需要给还原回来
	float currentDepth=length(fragToLight);
	float bias=max(0.05*(1.0-dot(normal,fragToLight)),0.005);
	float tempresult=currentDepth-bias>nearestDepth ? 0.0:1.0;
	return tempresult;
}

//处理点光源的函数
vec4 CalculatePointLight(PointLight light,vec3 normal,vec3 viewDir){
	vec3 lightDir=normalize(light.position-fragPos);
	float diff=max(dot(lightDir,normal),0.0f);
	vec3 halfwayDir=normalize(lightDir+viewDir);
	float spec=pow(max(dot(normal,halfwayDir),0.0f),material.shininess);

	//衰减因子
	float distance=length(light.position-fragPos);
	float attenuation=1.0/(light.kc+light.kl*distance+light.kq*distance*distance);

	vec4 tempdiffuse=vec4(texture(material.texture_diffuse1,vertexUV));
	vec4 ambient=light.ambient*tempdiffuse;
	vec4 diffuse=light.diffuse*diff*tempdiffuse;
	vec4 tempspecular=vec4(texture(material.texture_specular1,vertexUV));
	vec4 specular=light.specular*spec*tempspecular;
	
	//阴影因子
	//float shadow=CalculateShadowFactor(normal);

	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;

	return ambient+(diffuse+specular);
}

void main()
{
	//vec3 norN=normalize(normal);
	vec3 norN=TBN*texture(material.texture_normal1,vertexUV).rgb;//从法线贴图读取法线信息并从切线空间变换到世界空间
	norN=normalize(norN*2.0-1.0);//将法线信息线性转换到-1到1范围内并归一化
	vec3 viewDir=normalize(viewPos-fragPos);
	vec4 tempresult;
	for(int i=0;i<POINTLIGHT_NUM;i++){
		tempresult+=CalculatePointLight(pointlights[i],norN,viewDir);
	}
	fragColor=tempresult;
	//fragColor=vec4(1.0,1.0,0.0,1.0);
}