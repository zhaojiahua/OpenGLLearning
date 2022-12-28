#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;
layout(location=2) in vec2 aUV;
out vec4 vertexColor;
out vec2 vertexUV;
uniform mat4 transf;
void main()
{
	gl_Position=transf*vec4(aPos,1.0f);
	vertexColor=vec4(aColor,1.0f);
	vertexUV=aUV;
}