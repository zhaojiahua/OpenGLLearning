#version 330 core

in vec3 normal;
in vec2 vertexUV;
in vec3 fragPos;
out vec4 fragColor;
uniform sampler2D myTexture1;
uniform sampler2D myTexture2;
uniform float mixValue=0.2f;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
void main()
{
	//计算diffuse
	vec3 normN=normalize(normal);
	vec3 lightDir=normalize(lightPos-fragPos);
	float diff=max(dot(normN,lightDir),0.0f);
	vec3 diffuseColor=lightColor*diff;

	//计算环境光
	float ambientStrength=0.1f;
	vec3 ambienColor=ambientStrength*lightColor;

	//计算高光
	float specularStrenth=0.5f;
	vec3 viewDir=normalize(viewPos-fragPos);
	vec3 reflectDir=normalize(reflect(-normalize(lightPos),normN));
	float spec_=pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specularColor=specularStrenth*spec_*lightColor;

	//最终颜色
	vec3 resultColor=ambienColor+diffuseColor+specularColor;

	//物体的固有颜色属性
	vec2 tempUV=vec2(1-vertexUV.x,vertexUV.y);
	vec4 objColor=mix(texture(myTexture1,vertexUV),texture(myTexture2,tempUV),mixValue);

	fragColor=objColor*vec4(resultColor,1.0f);
	
}