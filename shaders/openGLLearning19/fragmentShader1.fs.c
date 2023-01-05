#version 330 core

out vec4 fragColor;
uniform float near;
uniform float far;

float LinearDepth(float depth){
	float z=depth*2.0-1.0;
	return (2.0*near*far)/(far+near-z*(far-near));
}

void main()
{
	//gl_FragCoord 是GL着色器内建的输出向量,其Z值包含了片段的深度值
	float depth=LinearDepth(gl_FragCoord.z)/far;
	fragColor=vec4(vec3(depth),1.0f);
}