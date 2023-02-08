#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aN;
layout(location=2) in vec2 aUV;
layout(location=3) in vec3 aT;//切线
layout(location=4) in vec3 aB;//副切线(实际上用不上,我们要自己计算)

out vec2 vertexUV;
out vec3 normal;
out vec3 fragPos;
out mat3 TBN;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 p_matrix;

void main()
{
	//vec3 Q1=dFdx(aPos);
	//vec3 Q2=dFdy(aPos);
	//vec2 st1=dFdx(aUV);
	//vec2 st2=dFdy(aUV);

	vec3 T=normalize(vec3(m_matrix*vec4(aT,0.0)));
	vec3 N=normalize(vec3(m_matrix*vec4(aN,0.0)));
	//vec3 T=normalize(Q1*st2.t-Q2*st1.t);
	vec3 B=cross(T,N);
	TBN=mat3(T,B,N);
	gl_Position=p_matrix*v_matrix*m_matrix*vec4(aPos,1.0f);
	vertexUV=aUV;
	normal=mat3(transpose(inverse(m_matrix)))*aN;//法线校正
	fragPos=(m_matrix*vec4(aPos,1.0)).xyz;
	//fragPos=vec3(m_matrix*vec4(aPos,1.0f));//去除空间位移后只保留旋转信息
}