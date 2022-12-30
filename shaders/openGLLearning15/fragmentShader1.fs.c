#version 330 core

//创建一个结构体来描述物体的材质属性
struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
//创建一个结构体来描述光照属性
struct Light{
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
uniform Light light;

void main()
{
	float distance=length(light.position-fragPos);
	vec3 normN=normalize(normal);
	vec3 lightDir=normalize(light.position-fragPos);
	float theta=dot(lightDir,normalize(-light.direction));
	float epsilon=light.cutoff-light.outercutoff;
	float spotIntensity=clamp((theta-light.outercutoff)/epsilon,0.0f,1.0f);

	//计算diffuse
	float diff=max(dot(normN,lightDir),0.0f);
	vec3 diffuseColor=diff*light.diffuse*vec3(texture(material.diffuse,vertexUV));

	//计算环境光(将环境光的材质颜色设置跟漫发射材质相同)
	vec3 ambienColor=light.ambient*vec3(texture(material.diffuse,vertexUV));

	//计算高光
	vec3 viewDir=normalize(viewPos-fragPos);
	vec3 reflectDir=normalize(reflect(lightDir,normN));
	float spec=pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor=spec*light.specular*vec3(texture(material.specular,vertexUV));

	//最终颜色
	vec3 resultColor=ambienColor+(diffuseColor+specularColor)*spotIntensity;
	fragColor=vec4(resultColor,1.0f);
}