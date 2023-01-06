#version 330 core

in vec3 vertexUV;//盒子上的位置坐标作为纹理坐标
out vec4 fragColor;

uniform samplerCube cubemap;//3D纹理采样器

void main()
{
	fragColor=texture(cubemap,vertexUV);
}