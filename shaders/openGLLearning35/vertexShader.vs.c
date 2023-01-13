#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aN;
layout(location=2) in vec2 aUV;

out vec2 vertexUV;
out vec3 normal;
out vec3 fragPos;
out vec4 lightSpacePos;//计算出光空间的点位置给到片元着色器

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 p_matrix;
uniform mat4 lightspace_matrix;//光空间变换矩阵

void main()
{
	gl_Position=p_matrix*v_matrix*m_matrix*vec4(aPos,1.0f);
	vertexUV=aUV;
	normal=mat3(transpose(inverse(m_matrix)))*aN;//发现校正
	fragPos=vec3(m_matrix*vec4(aPos,1.0f));//去除空间位移后只保留旋转信息
	lightSpacePos=lightspace_matrix*vec4(fragPos,1.0);
}