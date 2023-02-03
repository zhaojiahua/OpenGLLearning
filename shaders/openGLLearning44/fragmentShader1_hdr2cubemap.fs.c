#version 330 core

in vec3 localPos;
out vec4 fragColor;

uniform sampler2D hdrmap;

const vec2 inv=vec2(0.159155,0.31831);//vec2(1.0/(2*PI),1.0/PI);
//由3维空间向量转换到平面2维纹理坐标
vec2 SampleSphereMap(vec3 v3){
	vec2 uv=vec2(atan(v3.z,v3.x),asin(v3.y));//计算θ和φ作为uv值
	uv*=inv;
	uv+=0.5;
	return uv;
}
void main()
{	
	vec2 uv=SampleSphereMap(normalize(localPos));
	vec3 hdrColor=texture(hdrmap,uv).rgb;
	fragColor=vec4(hdrColor,1.0);
	//fragColor=vec4(1.0,1.0,0.0,1.0);
}