#version 330 core

struct Material{
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
};

in vec2 vertexUV;

out vec4 fragColor;

uniform Material material;
uniform float exposure;

void main()
{	
	const float gamma=2.2;
	vec3 hdrColor=texture(material.texture_diffuse1,vertexUV).rgb;
	vec3 bloomColor=texture(material.texture_diffuse2,vertexUV).rgb;
	hdrColor+=bloomColor;
	vec3 tempResult=vec3(1.0)-exp(-hdrColor*exposure);
	tempResult=pow(tempResult,vec3(1.0/gamma));
	fragColor=vec4(tempResult,1.0);
}