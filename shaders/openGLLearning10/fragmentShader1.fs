#version 330 core
in vec3 normal;
in vec2 vertexUV;
in vec3 fragPos;
out vec4 fragColor;
uniform sampler2D myTexture1;
uniform sampler2D myTexture2;
uniform float mixValue=0.2f;
uniform vec4 lightColor;
uniform vec3 lightPos;
void main()
{
	vec3 normN=normalize(normal);
	vec3 lightDir=normalize(lightPos-fragPos);
	float diff=max(dot(normN,lightDir),0.0f);
	vec4 diffuseColor=lightColor*diff;
	float ambientStrength=0.1f;
	vec4 ambienColor=ambientStrength*lightColor;
	vec2 tempUV=vec2(1-vertexUV.x,vertexUV.y);
	fragColor=mix(texture(myTexture1,vertexUV),texture(myTexture2,tempUV),mixValue)*(ambienColor+diffuseColor);
}