#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aUV;

out vec2 vertexUV;

void main()
{
	gl_Position=vec4(aPos.x,aPos.y,0.0f,1.0f);
	vertexUV=aUV;
}