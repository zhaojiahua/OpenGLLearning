#version 330 core
layout(location=0) in vec3 aPos;

out vec3 vertexUV;

uniform mat4 p_matrix;
uniform mat4 v_matrix;

void main()
{
	vertexUV=aPos;
	vec4 temppos=p_matrix*v_matrix*vec4(aPos,1.0f);
	gl_Position=temppos.xyww;//我们让z轴的值(在片元着色阶段就是深度值)等于w,这样经过透视除法之后深度值就等于1.0,这样就可以开启深度测试渲染天空盒了
}