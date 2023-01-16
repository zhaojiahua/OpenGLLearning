#version 330 core

in vec4 fragPos;//计算片元到光源位置的线性距离来承当深度值(gl_Position的z值经过透视矩阵变换后是非线性的,所以我们用自己计算的线性值)

uniform vec3 lightPos;//点光源的空间位置
uniform float far_plane;//点光源视角透视远平面

void main()
{
	float lightDistance=length(fragPos.xyz-lightPos);
	gl_FragDepth=lightDistance/far_plane;
	gl_FragColor=vec4(gl_FragDepth);
}