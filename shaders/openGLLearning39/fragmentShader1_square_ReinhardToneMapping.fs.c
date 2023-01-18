#version 330 core

struct Material{
	sampler2D texture_diffuse1;
};

in vec2 vertexUV;

out vec4 fragColor;

uniform Material material;

void main()
{
	vec3 hdrcolor=texture(material.texture_diffuse1,vertexUV).rgb;
	vec3 mapped=hdrcolor/(hdrcolor+vec3(1.0));
	mapped=pow(mapped,vec3(1.0/2.2));
	fragColor=vec4(mapped,1.0);
}