#version 330 core

struct Material{
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
};

in vec2 vertexUV;

out vec4 fragColor;

uniform Material material;

void main()
{	
	vec3 hdrColor=texture(material.texture_diffuse1,vertexUV).rgb;
	fragColor=vec4(hdrColor,1.0);
}