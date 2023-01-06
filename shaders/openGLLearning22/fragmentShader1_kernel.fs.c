#version 330 core

in vec2 vertexUV;
out vec4 fragColor;

uniform sampler2D screenTex;

const float offset=1.0f/300.0f;

void main()
{
	vec2 offsets[9]={
		vec2(-offset,offset),vec2(0.0f,offset),vec2(offset,offset),
		vec2(-offset,0.0f),  vec2(0.0f,0.0f),  vec2(offset,0.0f),
		vec2(-offset,-offset),  vec2(0.0f,-offset),  vec2(offset,-offset)
	};
	//锐化效果核
	float kernel_sharp[9]={
		-1,-1,-1,
		-1, 9,-1,
		-1,-1,-1,
	};
	//模糊效果核
	float kernel_blur[9]={
		1.0/16,2.0/16,1.0/16,
		2.0/16,4.0/16,2.0/16,
		1.0/16,2.0/16,1.0/16
	};
	vec3 sampleColor[9];
	for(int i=0;i<9;i++){
		sampleColor[i]=vec3(texture(screenTex,vertexUV.st+offsets[i]));
	}
	vec3 tempColor=vec3(0.0f);
	for(int i=0;i<9;i++){
		tempColor+=sampleColor[i]*kernel_sharp[i];
	}
	fragColor=vec4(tempColor,1.0f);
}