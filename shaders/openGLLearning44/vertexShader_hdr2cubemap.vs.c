#version 330 core
layout(location=0) in vec3 aPos;

out vec3 localPos;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 p_matrix;

void main()
{
	localPos=aPos;
	gl_Position=p_matrix*v_matrix*m_matrix*vec4(aPos,1.0);
}