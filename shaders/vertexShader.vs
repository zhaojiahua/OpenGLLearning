#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aUV;
out vec2 vertexUV;
uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 p_matrix;
void main()
{
	gl_Position=p_matrix*v_matrix*m_matrix*vec4(aPos,1.0f);
	vertexUV=aUV;
}