#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aN;
layout(location=2) in vec2 aUV;

uniform mat4 m_matrix;

void main()
{
	gl_Position=m_matrix*vec4(aPos,1.0f);
}