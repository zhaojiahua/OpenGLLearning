#version 330 core

struct Material{
	sampler2D texture_diffuse1;
};

in vec2 vertexUV;

out vec4 fragColor;

uniform Material material;

void main()
{
	fragColor=texture(material.texture_diffuse1,vertexUV);
}