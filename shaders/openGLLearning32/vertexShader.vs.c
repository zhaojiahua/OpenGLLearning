#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aN;
layout(location=2) in vec2 aUV;
layout(location=3) in mat4 instanceMatrix;

out vec2 vertexUV;
out vec3 normal;
out vec3 fragPos;
uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 p_matrix;
void main()
{
	//mat4 tempMM=m_matrix*instanceMatrix;
	gl_Position=p_matrix*v_matrix*instanceMatrix*vec4(aPos,1.0f);
	vertexUV=aUV;
	normal=mat3(transpose(inverse(instanceMatrix)))*aN;
	fragPos=vec3(instanceMatrix*vec4(aPos,1.0f));
}