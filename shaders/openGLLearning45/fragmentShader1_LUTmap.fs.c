#version 330 core
#define PI 3.14159265359

in vec2 vertexUV;
out vec2 fragColor;

uniform float roughness;//受roughness变化影响
uniform samplerCube hdrcubemap;

float RadicalInverse_VdC(int bits) 
{
     bits = (bits << 16) | (bits >> 16);
     bits = ((bits & 0x55555555) << 1) | ((bits & 0xAAAAAAAA) >> 1);
     bits = ((bits & 0x33333333) << 2) | ((bits & 0xCCCCCCCC) >> 2);
     bits = ((bits & 0x0F0F0F0F) << 4) | ((bits & 0xF0F0F0F0) >> 4);
     bits = ((bits & 0x00FF00FF) << 8) | ((bits & 0xFF00FF00) >> 8);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
vec2 Hammersley(int i, int N)
{
	return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

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

float GeometrySchlickGGX(float NdotV,float roughness){
	float a=roughness;
	float k=(a*a)/2.0;
	float nom=NdotV;
	float denom=NdotV*(1.0-k)+k;
	return nom/denom;
}
float GeometrySmith(vec3 N, vec3 V,vec3 L,float roughness){
	float NdotV=max(dot(N,V),0.0);
	float NdotL=max(dot(N,L),0.0);
	float ggx2=GeometrySchlickGGX(NdotV,roughness);
	float ggx1=GeometrySchlickGGX(NdotL,roughness);
	return ggx1*ggx2;
}
//BRDF卷积着色器在2D平面上执行,直接使用其2D纹理坐标(NdotV roughness)作为卷积输入即可
vec2 IntegrateBRDF(float NdotV,float roughness){
	vec3 V;
	V.x=sqrt(1.0-NdotV*NdotV);
	V.y=0.0;
	V.z=NdotV;

	float A=0.0;
	float B=0.0;

	vec3 N=vec3(0.0,0.0,1.0);

	const int SAMPLE_COUNT=1024;
	for(int i=0;i<SAMPLE_COUNT;i++){
		//vec2 xi=HammersleyNoBitOps(i,SAMPLE_COUNT);
		vec2 xi=Hammersley(i,SAMPLE_COUNT);
		vec3 H=ImportanceSampleGGX(xi,N,roughness);//生成采样的向量
		vec3 L=normalize(2.0*dot(V,H)*H-V);//根据出射方向和法线算出光源方向

		float NdotL=max(L.z,0.0);
		float NdotH=max(H.z,0.0);
		float VdotH=max(dot(V,H),0.0);

		if(NdotL>0.0){
			float G=GeometrySmith(N,V,L,roughness);
			float G_vis=(G*VdotH)/(NdotH*NdotV);
			float Fc=pow(1.0-VdotH,5.0);

			A+=(1.0-Fc)*G_vis;
			B+=Fc*G_vis;
		}
	}
	A/=float(SAMPLE_COUNT);
	B/=float(SAMPLE_COUNT);
	return vec2(A,B);
}

void main()
{	
	vec2 brdfResult=IntegrateBRDF(vertexUV.x,vertexUV.y);
	fragColor=brdfResult;
	//fragColor=vec4(1.0,1.0,0.0,1.0);
}