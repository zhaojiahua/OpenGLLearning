#version 330 core
#define PI 3.1415926535

in vec3 localPos;
out vec4 fragColor;

uniform float roughness;//受roughness变化影响
uniform samplerCube hdrcubemap;

//生成低差异序列
float VanDerCorpus(int n,int base){
	float invBase=1.0/float(base);
	float denom=1.0;
	float tempResult=0.0;
	for(int i=0;i<32;i++){
		if(n>0){
			denom=mod(float(n),2.0);
			tempResult+=denom*invBase;
			invBase=invBase/2.0;
			n=int(float(n)/2.0);
		}
	}
	return tempResult;
}
vec2 HammersleyNoBitOps(int i,int N){
	return vec2(float(i)/float(N),VanDerCorpus(i,2));
}
//由生成的低差异序列生成三维重要采样样本向量(使其朝向特定粗糙度的镜面波瓣方向)
vec3 ImportanceSampleGGX(vec2 xi,vec3 N,float roughness){
	float a=roughness*roughness;
	float phi=2.0*PI*xi.x;
	float cosTheta=sqrt((1.0-xi.y)/(1.0+(a*a-1.0)*xi.y));//粗糙度越大θ的取值范围就越大(迪士尼研究发现平方roughness效果更好)
	float sinTheta=sqrt(1.0-cosTheta*cosTheta);
	//球面坐标转换到笛卡尔坐标
	vec3 H=vec3(cos(phi)*sinTheta,sin(phi)*sinTheta,cosTheta);
	//从切线空间坐标转换到世界空间坐标
	vec3 up=abs(N.z)<0.999 ? vec3(0.0,0.0,1.0) : vec3(1.0,0.0,0.0);
	vec3 tangent=normalize(cross(up,N));
	vec3 bitangent=cross(N,tangent);
	return normalize(tangent*H.x+bitangent*H.y+N*H.z);
}

void main()
{	
	vec3 N=normalize(localPos);
	vec3 R=N;
	vec3 V=R;//假设视角方向(也就是镜面反射方向)总是等于输出采样方向

	vec3 prefilterResult=vec3(0.0);
	//预过滤卷积
	const int SAMPLE_COUNT=1024;
	float totalWeight=0.0;
	for(int i=0;i<SAMPLE_COUNT;i++){
		vec2 xi=HammersleyNoBitOps(i,SAMPLE_COUNT);
		vec3 H=ImportanceSampleGGX(xi,N,roughness);//生成采样的向量
		vec3 L=normalize(2.0*dot(V,H)*H-V);//根据出射方向和法线算出光源方向
		float NdotL=max(dot(N,L),0.0);
		if(NdotL>0.0){
			prefilterResult+=texture(hdrcubemap,L).rgb*NdotL;
			totalWeight+=NdotL;
		}
	}
	prefilterResult=prefilterResult/totalWeight;
	fragColor=vec4(prefilterResult,1.0);
	//fragColor=vec4(1.0,1.0,0.0,1.0);
}