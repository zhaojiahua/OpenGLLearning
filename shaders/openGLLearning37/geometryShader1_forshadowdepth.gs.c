#version 330 core
//几何着色器负责负责将所有顶点变换到6个不同的光空间
layout(triangles)in;//输入的是三角形
layout(triangle_strip,max_vertices=18)out;//输出的也是三角,但是输出的6不同方向的三角形,所以最大点数量应该是3*6=18

out vec4 fragPos;

uniform mat4 pointShadowMatrices[6];

void main()
{
	for(int face=0;face<6;++face){
		gl_Layer=face;//几何着色器的内建变量gl_Layer,指定了发散出来的图形送到立方体的哪个面,只有附加到激活的帧缓冲的立方体贴图纹理才有效
		for(int i=0;i<3;++i){
			fragPos=gl_in[i].gl_Position;
			gl_Position=pointShadowMatrices[face]*fragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}