#version 330 core
//指定像素着色器写入到指定的颜色缓冲区(帧缓冲必须具有多个颜色缓冲的时候才可以这么使用)
out vec4 fragColor;
in vec2 vertexUV;

uniform sampler2D texture_diffuse;

uniform vec2 offsets[9];
uniform int edge_kernel[9];//边缘检测卷积核
uniform float blur_kernel[9];//图像模糊卷积核

uniform bool chaos;
uniform bool confuse;
uniform bool shake;

void main()
{
	vec4 tempColor=vec4(0.0);
	vec3 simple[9];//卷积核采样值

	if(chaos || shake){
		for(int i=0;i<9;i++){
			simple[i]=vec3(texture(texture_diffuse,vertexUV+offsets[i]));
		}
	}

	//特效处理
	if(chaos){//晃动的时候加上边缘加测特效
		for(int i=0;i<9;i++){
			tempColor+=vec4(simple[i]*edge_kernel[i],1.0);
		}
		tempColor.a=1.0;
	}
	else if(confuse){//翻转的时候加上颜色取反特效
		tempColor+=vec4(1.0-texture(texture_diffuse,vertexUV).rgb,1.0);
	}
	else if(shake){//震动的时候加上模糊特效
		for(int i=0;i<9;i++){
			tempColor+=vec4(simple[i]*blur_kernel[i],1.0);
		}
		tempColor.a=1.0;
	}
	else tempColor=texture(texture_diffuse,vertexUV);
	fragColor=tempColor;
	//fragColor=vec4(1.0,0.0,0.0,1.0);
}