#version 330 core

in vec2 vertexUV;
out vec4 fragColor;

uniform sampler2D screenTex;

void main()
{
	fragColor=texture(screenTex,vertexUV);
	float aver=(fragColor.r+fragColor.g+fragColor.b)/3.0;
	fragColor=vec4(aver,aver,aver,1.0f);
}