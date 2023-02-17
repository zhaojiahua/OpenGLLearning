#version 330 core
layout(location=0) in vec4 pos_uv;//vec2 position,vec2 uv

out vec2 vertexUV;

//震动翻转等特效
uniform	bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;

void main()
{
	gl_Position=vec4(pos_uv.xy,0.0,1.0);
	vec2 texcoord=pos_uv.zw;
	if(chaos){
		float strength=0.1;//晃动强度
		float frequence=4.0;//晃动频率
		vec2 tempuv=vec2(texcoord.x+sin(time*frequence)*strength,texcoord.y+cos(time*frequence)*strength);
		vertexUV=tempuv;
	}
	else if(confuse){
		vertexUV=vec2(1.0-texcoord.x,1.0-texcoord.y);
	}
	else vertexUV=texcoord;
	if(shake){
		float strength=0.02;//震动强度
		float frequence=15.0;//震动频率
		gl_Position.x+=cos(time*(frequence+1))*strength;
		gl_Position.y+=cos(time*(frequence+6))*strength;
	}
}
