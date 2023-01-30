#version 330 core
//指定像素着色器写入到指定的颜色缓冲区(帧缓冲必须具有多个颜色缓冲的时候才可以这么使用)
layout(location=0) out vec4 gPosition;
layout(location=1) out vec4 gNormal;
layout(location=2) out vec4 gAlbedoSpec;

in vec3 normal;
in vec2 vertexUV;
in vec3 fragPos;
in mat3 TBN;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;

void main()
{
	gPosition=vec4(fragPos,1.0);//渲染位置信息
	gNormal=vec4(normalize(normal),1.0);//渲染法线信息
	gAlbedoSpec.rgb=texture(texture_diffuse,vertexUV).rgb;//漫反射颜色信息
	gAlbedoSpec.a=texture(texture_specular,vertexUV).r;//镜面强度信息
}