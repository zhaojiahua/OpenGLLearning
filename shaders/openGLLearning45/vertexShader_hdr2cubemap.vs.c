#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aN;
layout(location=2) in vec2 aUV;

out vec3 localPos;
out vec2 vertexUV;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 p_matrix;

void main()
{
	localPos=aPos;
	gl_Position=p_matrix*v_matrix*m_matrix*vec4(aPos,1.0);
}