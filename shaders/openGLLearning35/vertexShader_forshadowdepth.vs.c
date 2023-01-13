#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aN;
layout(location=2) in vec2 aUV;

uniform mat4 lightspace_matrix;
void main()
{
	gl_Position=lightspace_matrix*vec4(aPos,1.0f);
}