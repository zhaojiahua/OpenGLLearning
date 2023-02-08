#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aN;
layout(location=2) in vec2 aUV;

out vec2 vertexUV;

void main()
{
	//vertexUV=vec2(aUV.x,1-aUV.y);
	vertexUV=aUV;
}