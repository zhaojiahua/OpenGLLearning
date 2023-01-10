#version 330 core
layout(location=0) in vec2 aPos;
layout(location=1) in vec3 aColor;

out	vec3 fcolor;

uniform vec2 offsets[100];	// 从CPU接收100个浮点数组用于几何着色器的位置偏移输出

void main()
{
	gl_Position=vec4(aPos+offsets[gl_InstanceID],0.0,1.0);	//gl_InstanceID是OpenGL的内建变量,实例序号,从0开始
	fcolor=aColor;
}