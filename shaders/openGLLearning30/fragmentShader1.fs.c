#version 330 core

in vec3 fcolor;	//直接从顶点着色器接受颜色向量
out vec4 fragColor;	//片段着色器输出颜色

void main()
{
	fragColor=vec4(fcolor,1.0);
}