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
	sampler2D texture_normal2;
	sampler2D texture_normal3;
	sampler2D texture_height1;
	sampler2D texture_height2;
	sampler2D texture_height3;
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
out vec4 fragColor;

uniform vec3 viewPos;
uniform vec4 tempdiffuse_;
uniform Material material;
#define DIRECTOINLIGHT_NUM 1
#define POINTLIGHT_NUM 4
#define SPOTLIGHT_NUM 1
uniform DirLight dirlights[DIRECTOINLIGHT_NUM];
uniform PointLight pointlights[POINTLIGHT_NUM];
uniform SpotLight spotlights[SPOTLIGHT_NUM];

//处理平行光的函数
vec4 CalculateDirLight(DirLight light,vec3 normal,vec3 viewDir){
	vec3 lightDir=normalize(-light.direction);
	vec3 halfwayDir=normalize(lightDir+viewDir);//半程向量
	float diff=max(dot(lightDir,normal),0.0f);
	float spec=pow(max(dot(normal,halfwayDir),0.0f),material.shininess);//计算半程向量和法向量之间的夹角来确定高光反射的大小
	vec4 tempdiffuse=tempdiffuse_+vec4(texture(material.texture_diffuse1,vertexUV)+texture(material.texture_diffuse2,vertexUV)+texture(material.texture_diffuse3,vertexUV));
	vec4 ambient=light.ambient*tempdiffuse;
	vec4 diffuse=light.diffuse*diff*tempdiffuse;
	vec4 tempspecular=vec4(texture(material.texture_specular1,vertexUV)+texture(material.texture_specular2,vertexUV)+texture(material.texture_specular3,vertexUV));
	vec4 specular=light.specular*spec*tempspecular;
	return ambient+diffuse+specular;
}
//处理点光源的函数
vec4 CalculatePointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir){
	vec3 lightDir=normalize(light.position-fragPos);
	vec3 halfwayDir=normalize(lightDir+viewDir);//半程向量
	float diff=max(dot(lightDir,normal),0.0f);
	float spec=pow(max(dot(normal,halfwayDir),0.0f),material.shininess);

	//衰减因子
	float distance=length(light.position-fragPos);
	float attenuation=1.0/(light.kc+light.kl*distance+light.kq*distance*distance);

	vec4 tempdiffuse=vec4(texture(material.texture_diffuse1,vertexUV)+texture(material.texture_diffuse2,vertexUV)+texture(material.texture_diffuse3,vertexUV));
	vec4 ambient=light.ambient*tempdiffuse;
	vec4 diffuse=light.diffuse*diff*tempdiffuse;
	vec4 tempspecular=vec4(texture(material.texture_specular1,vertexUV)+texture(material.texture_specular2,vertexUV)+texture(material.texture_specular3,vertexUV));
	vec4 specular=light.specular*spec*tempspecular;
	return (ambient+diffuse+specular)*attenuation;
}
//计算聚光灯的函数
vec4 CalculateSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir){
	vec3 lightDir=normalize(light.position-fragPos);
	vec3 halfwayDir=normalize(lightDir+viewDir);//半程向量
	float theta=dot(lightDir,normalize(-light.direction));
	float epsilon=light.cutoff-light.outercutoff;
	float spotIntensity=clamp((theta-light.outercutoff)/epsilon,0.0f,1.0f);

	//计算diffuse
	float diff=max(dot(normal,lightDir),0.0f);
	vec4 tempdiffuse=tempdiffuse_+vec4(texture(material.texture_diffuse1,vertexUV)+texture(material.texture_diffuse2,vertexUV)+texture(material.texture_diffuse3,vertexUV));
	vec4 diffuseColor=diff*light.diffuse*tempdiffuse;

	//计算环境光(将环境光的材质颜色设置跟漫发射材质相同)
	vec4 ambienColor=light.ambient*tempdiffuse;

	//计算高光
	float spec=pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	vec4 tempspecular=vec4(texture(material.texture_specular1,vertexUV)+texture(material.texture_specular2,vertexUV)+texture(material.texture_specular3,vertexUV));
	vec4 specularColor=spec*light.specular*tempspecular;

	//最终颜色
	return ambienColor+(diffuseColor+specularColor)*spotIntensity;
}

void main()
{
	vec3 tempNormal=vec3(texture(material.texture_normal1,vertexUV)+texture(material.texture_normal2,vertexUV)+texture(material.texture_normal3,vertexUV));
	vec3 norN=normalize(normal+tempNormal);
	vec3 viewDir=normalize(viewPos-fragPos);
	vec4 tempresult;
	//首先平行光的计算
	for(int i=0;i<DIRECTOINLIGHT_NUM;i++){
		tempresult=CalculateDirLight(dirlights[i],norN,viewDir);
	}
	//然后点光源计算
	//for(int i=0;i<POINTLIGHT_NUM;i++){
	//	tempresult=CalculatePointLight(pointlights[i],norN,fragPos,viewDir);
	//}
	//最后聚光灯计算
	for(int i=0;i<SPOTLIGHT_NUM;i++){
		tempresult+=CalculateSpotLight(spotlights[i],norN,fragPos,viewDir);
	}

	fragColor=tempresult;
}