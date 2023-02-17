#version 330 core
//指定像素着色器写入到指定的颜色缓冲区(帧缓冲必须具有多个颜色缓冲的时候才可以这么使用)
out vec4 fragColor;

in vec2 vertexUV;

uniform vec3 spriteColor;
uniform sampler2D texture_diffuse;

void main()
{
	vec4 tempColor=texture(texture_diffuse,vertexUV);
	fragColor=vec4(spriteColor,1.0)*tempColor;
	//fragColor=vec4(1.0,0.0,0.0,1.0);
}