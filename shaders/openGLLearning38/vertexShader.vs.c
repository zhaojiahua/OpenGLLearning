#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aN;
layout(location=2) in vec2 aUV;
layout(location=3) in vec3 aT;//切线
layout(location=4) in vec3 aB;//副切线(实际上用不上,我们要自己计算)

out vec2 vertexUV;
out vec3 normal;
out vec3 fragPos;
out vec3 tangentLightPos;//顶点着色器的计算压力比较小,所以我们把切线空间的转换计算放在顶点着色器中进行
out vec3 tangentViewPos;
out vec3 tangentFragPos;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 p_matrix;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	gl_Position=p_matrix*v_matrix*m_matrix*vec4(aPos,1.0f);
	vertexUV=aUV;
	fragPos=vec3(m_matrix*vec4(aPos,1.0f));//去除空间位移后只保留旋转信息

	vec3 T=normalize(vec3(m_matrix*vec4(aT,0.0)));
	vec3 N=normalize(vec3(m_matrix*vec4(aN,0.0)));
	vec3 B=cross(T,N);
	mat3 TBN=transpose(mat3(T,B,N));//这里因为TBN是正交矩阵,所以它的转置矩阵等于它的逆矩阵,可以节省大量的计算量
	tangentLightPos=TBN*lightPos;
	tangentViewPos=TBN*viewPos;
	tangentFragPos=TBN*fragPos;
}