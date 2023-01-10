#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aN;
layout(location=2) in vec2 aUV;

out VS_OUT{
	vec2 avertexUV;
	vec3 anormal;
	vec3 afragPos;
} vs_out;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 p_matrix;

void main()
{
	gl_Position=p_matrix*v_matrix*m_matrix*vec4(aPos,1.0f);
	vs_out.avertexUV=aUV;
	vs_out.anormal=mat3(transpose(inverse(m_matrix)))*aN;
	vs_out.afragPos=vec3(m_matrix*vec4(aPos,1.0f));
}