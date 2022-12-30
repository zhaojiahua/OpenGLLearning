#version 330 core

//创建一个结构体来描述物体的材质属性
struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
//创建一个结构体来描述光照属性
struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 normal;
in vec2 vertexUV;
in vec3 fragPos;
out vec4 fragColor;
uniform sampler2D myTexture1;
uniform sampler2D myTexture2;
uniform float mixValue=0.2f;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main()
{
	//计算diffuse
	vec3 normN=normalize(normal);
	vec3 lightDir=normalize(light.position-fragPos);
	float diff=max(dot(normN,lightDir),0.0f);
	vec3 diffuseColor=diff*material.diffuse*light.diffuse;

	//计算环境光
	vec3 ambienColor=material.ambient*light.ambient;

	//计算高光
	vec3 viewDir=normalize(viewPos-fragPos);
	vec3 reflectDir=normalize(reflect(-light.position,normN));
	float spec=pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor=spec*material.specular*light.specular;

	//最终颜色
	vec3 resultColor=ambienColor+diffuseColor+specularColor;
	fragColor=vec4(resultColor,1.0f);
	
}