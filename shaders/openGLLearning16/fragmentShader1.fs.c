#version 330 core

//创建一个结构体来描述物体的材质属性
struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
//平行光
struct DirLight{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
//点光源
struct PointLight{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float kc;
	float kl;
	float kq;
};
//聚光灯
struct SpotLight{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutoff;
	float outercutoff;
};

in vec3 normal;
in vec2 vertexUV;
in vec3 fragPos;
out vec4 fragColor;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirlight;
#define POINTLIGHT_NUM 4
uniform PointLight pointlights[POINTLIGHT_NUM];
uniform SpotLight spotlight;

//处理平行光的函数
vec3 CalculateDirLight(DirLight light,vec3 normal,vec3 viewDir){
	vec3 lightDir=normalize(-light.direction);
	float diff=max(dot(lightDir,normal),0.0f);
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=pow(max(dot(viewDir,reflectDir),0.0f),material.shininess);
	vec3 ambient=light.ambient*vec3(texture(material.diffuse,vertexUV));
	vec3 diffuse=light.diffuse*diff*vec3(texture(material.diffuse,vertexUV));
	vec3 specular=light.specular*spec*vec3(texture(material.specular,vertexUV));
	return ambient+diffuse+specular;
}
//处理点光源的函数
vec3 CalculatePointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir){
	vec3 lightDir=normalize(light.position-fragPos);
	float diff=max(dot(lightDir,normal),0.0f);
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=pow(max(dot(viewDir,reflectDir),0.0f),material.shininess);

	//衰减因子
	float distance=length(light.position-fragPos);
	float attenuation=1.0/(light.kc+light.kl*distance+light.kq*distance*distance);

	vec3 ambient=light.ambient*vec3(texture(material.diffuse,vertexUV));
	vec3 diffuse=light.diffuse*diff*vec3(texture(material.diffuse,vertexUV));
	vec3 specular=light.specular*spec*vec3(texture(material.specular,vertexUV));
	return (ambient+diffuse+specular)*attenuation;
}
//计算聚光灯的函数
vec3 CalculateSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir){
	vec3 lightDir=normalize(light.position-fragPos);
	float theta=dot(lightDir,normalize(-light.direction));
	float epsilon=light.cutoff-light.outercutoff;
	float spotIntensity=clamp((theta-light.outercutoff)/epsilon,0.0f,1.0f);

	//计算diffuse
	float diff=max(dot(normal,lightDir),0.0f);
	vec3 diffuseColor=diff*light.diffuse*vec3(texture(material.diffuse,vertexUV));

	//计算环境光(将环境光的材质颜色设置跟漫发射材质相同)
	vec3 ambienColor=light.ambient*vec3(texture(material.diffuse,vertexUV));

	//计算高光
	vec3 reflectDir=normalize(reflect(lightDir,normal));
	float spec=pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor=spec*light.specular*vec3(texture(material.specular,vertexUV));

	//最终颜色
	return ambienColor+(diffuseColor+specularColor)*spotIntensity;
}

void main()
{
	vec3 norN=normalize(normal);
	vec3 viewDir=normalize(viewPos-fragPos);

	//首先平行光的计算
	vec3 tempresult=CalculateDirLight(dirlight,norN,viewDir);
	//然后遍历点光源计算
	for(int i=0;i<POINTLIGHT_NUM;i++){
		tempresult+=CalculatePointLight(pointlights[i],norN,fragPos,viewPos);
	}
	//最后聚光灯计算
	tempresult+=CalculateSpotLight(spotlight,norN,fragPos,viewPos);

	fragColor=vec4(tempresult,1.0f);
}