#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;
out vec4 vertexColor;
out vec3 vertexPos;
uniform float offsetH;
void main()
{
	gl_Position=vec4(aPos.x+offsetH,-aPos.y,aPos.z,1.0f);
	vertexPos=aPos;
	vertexColor=vec4(aColor,1.0f);
}