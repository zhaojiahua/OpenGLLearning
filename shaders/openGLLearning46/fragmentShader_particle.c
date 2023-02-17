#version 330 core

out vec4 fragColor;

in vec2 vertexUV;

uniform vec4 spriteColor;
uniform sampler2D texture_diffuse;

void main()
{
	vec4 tempColor=texture(texture_diffuse,vertexUV);
	fragColor=spriteColor*tempColor;
	//fragColor=vec4(1.0,0.0,0.0,1.0);
}