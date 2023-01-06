#version 330 core

in vec2 vertexUV;
out vec4 fragColor;

uniform sampler2D screenTex;

void main()
{
	fragColor=vec4(vec3(1.0f-texture(screenTex,vertexUV)),1.0f);
}