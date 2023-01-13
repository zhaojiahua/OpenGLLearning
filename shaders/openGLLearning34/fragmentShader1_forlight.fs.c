#version 330 core

out vec4 fragColor;

in vec2 vertexUV;
in vec3 normal;
in vec3 fragPos;

uniform vec4 lightColor;
uniform vec3 lightDirection;

void main()
{
	vec3 lightDir=normalize(-lightDirection);
	float diff=max(dot(lightDir,normal),0.0f);
	vec4 ambient=0.4*lightColor;
	vec4 diffuse=diff*lightColor;
	fragColor=ambient+diffuse;
}