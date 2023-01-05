#version 330 core

in vec3 normal;
in vec2 vertexUV;
in vec3 fragPos;
out vec4 fragColor;

void main()
{
	fragColor=vec4(0.22f,0.02f,0.30f,1.0f);
}