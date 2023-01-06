#version 330 core

in vec3 textureDir;//纹理坐标的方向向量
out vec4 fragColor;

uniform samplerCube cubemap;//3D纹理采样器

void main()
{
	fragColor=texture(cubemap,textureDir);
}