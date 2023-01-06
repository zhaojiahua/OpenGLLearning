#version 330 core

in vec2 vertexUV;
out vec4 fragColor;

uniform sampler2D screenTex;

void main()
{
	fragColor=texture(screenTex,vertexUV);
}