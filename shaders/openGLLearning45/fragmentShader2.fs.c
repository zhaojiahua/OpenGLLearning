#version 330 core
//指定像素着色器写入到指定的颜色缓冲区(帧缓冲必须具有多个颜色缓冲的时候才可以这么使用)
out vec4 fragColor;

in vec3 normal;
in vec2 vertexUV;
in vec3 fragPos;
in mat3 TBN;

uniform vec3 viewPos;

uniform sampler2D texture_albedo;
uniform sampler2D texture_normal;
uniform sampler2D texture_metallic;
uniform sampler2D texture_roughness;
uniform sampler2D texture_ao;
uniform samplerCube texture_irradiance;//环境辐照度贴图
uniform samplerCube texture_perfilterMap;//预过滤环境光照贴图
uniform sampler2D texture_brdfLUTMap;//BRDF卷积贴图

#define PI 3.1415926535
#define POINTLIGHTNUM 4
uniform vec3 lightPositions[POINTLIGHTNUM];

vec3 LoadNormalFromMap(){//注意将法线从切线空间变换到视图空间
	vec3 tangentN=texture(texture_normal,vertexUV).xyz*2.0-1.0;

	vec3 Q1=dFdx(fragPos);
	vec3 Q2=dFdy(fragPos);
	vec2 st1=dFdx(vertexUV);
	vec2 st2=dFdy(vertexUV);

	vec3 N=normalize(normal);
	vec3 T=normalize(Q1*st2.t-Q2*st1.t);
	vec3 B=-normalize(cross(N,T));
	mat3 TBN=mat3(T,B,N);

	return normalize(TBN*tangentN);
}

float CalculateAttenuation(vec3 fragPos,vec3 lightPos){
	float distance=length(fragPos-lightPos);
	return 1.0/(0.8+0.06*distance+0.02*distance*distance);
}

//F 菲涅尔系数项函数
vec3 FresnelSchlick(float cosTheta,vec3 F0){
	return F0+(1.0-F0)*pow(1.0-cosTheta,5.0);
}
vec3 FresnelSchlickRoughness(float cosTheta,vec3 F0,float roughness){
	return F0+(max(vec3(1.0-roughness),F0)-F0)*pow(clamp(1.0-cosTheta,0.0,1.0),5.0);
}
//D 法线分布系数项函数
float DistributionGGX(vec3 N,vec3 H,float roughness){
	float a=roughness*roughness;
	float a2=a*a;
	float NdotH=max(dot(N,H),0.0);
	float NdotH2=NdotH*NdotH;
	float nom=a2;
	float denom=(NdotH2*(a2-1.0)+1.0);
	denom=PI*denom*denom;
	return nom/denom;
}
//G 几何遮蔽系数项函数
float GeometrySchlickGGX(float NdotV,float roughness){
	float r=(roughness+1.0);
	float k=(r*r)/8.0;
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

void main()
{
	//从贴图采样颜色
	vec3 albedo=pow(texture(texture_albedo,vertexUV).rgb,vec3(2.2));//注意albedo贴图要从gamma空间校正
	float metallic=texture(texture_metallic,vertexUV).r;
	float roughness=texture(texture_roughness,vertexUV).r;
	float ao=texture(texture_ao,vertexUV).r;
	//自定义材质
	//vec3 albedo=vec3(1.0,0.0,0.0);
	//float metallic=1.0;
	//float roughness=0.5;
	//float ao=1.0;
	vec3 irradiance=texture(texture_irradiance,normal).rgb;//IBL漫反射


	vec3 N=LoadNormalFromMap();
	//vec3 N=normal;
	vec3 V=normalize(viewPos-fragPos);

	vec3 R=reflect(-V,N);
	const float MAX_REFLECTION_LOD=4.0;//环境预滤波贴图最多5个级别
	vec3 prefiltered=textureLod(texture_perfilterMap,R,roughness*MAX_REFLECTION_LOD).rgb;//从反射反向采样预滤波贴图(读取表面的间接镜面反射),根据不同的粗糙度采样不同的mipmap级别
	vec2 envBRDF=texture(texture_brdfLUTMap,vec2(max(dot(N,V),0.0),roughness)).rg;//根据粗糙度和视线-法线夹角采样BRDF积分贴图
	//vec2 envBRDF=vec2(0.5,0.2);
	vec3 F0=mix(vec3(0.04),albedo,metallic);//根据金属度融合金属和电介质的属性,金属度越大F0越接近albedo
	
	//光源计算
	vec3 Lo=vec3(0.0);
	for(int i=0;i<POINTLIGHTNUM;i++){
		vec3 L=normalize(lightPositions[i]-fragPos);
		vec3 H=normalize(V+L);
		vec3 lightColor=vec3(23.47,21.31,20.79);//先把点光源的辐射强度建模为一个常量辐射通量,这个点光源只朝一个方向发出辐射通量
		float cosTheta=max(dot(N,L),0.0);//我们将会根据入射角度和法线之间的夹角对辐射率缩放
		float attenuation=CalculateAttenuation(fragPos,lightPositions[i]);//计算点光源的衰减系数
		vec3 radiance=lightColor*attenuation*cosTheta;//求出辐射率
////
		//计算BRDF
		//F项
		vec3 F=FresnelSchlick(max(dot(H,V),0.0),F0);
		//D项
		float D=DistributionGGX(N,H,roughness);
		//G项
		float G=GeometrySmith(N,V,L,roughness);
////
		//算出镜面反射项
		float denominator=max(4.0*max(dot(N,V),0.0)*max(dot(N,L),0.0),0.001);//配平因子(避免出现除0的情况)
		vec3 specular=(D*G*F)/denominator;
////
		//计算折射比例
		vec3 kd=vec3(1.0)-F;//菲涅尔算出来的就是反射比例
		kd*=(1.0-metallic);//金属度决定折射光线逃逸出来形成漫反射的强度,纯金属一般情况是折射光线全部立即被吸收,所以金属是没有漫反射的,所看到的全是反射光线
		
		//最后计算出射光强
		Lo+=(kd*albedo/PI+specular)*radiance;
	}

	//这里我们用N代替了H,由于H是受粗糙度影响的,所以这里需要补偿一下粗糙度的影响
	vec3 Fs=FresnelSchlickRoughness(max(dot(N,V),0.0),F0,roughness);
	vec3 outks=Fs;
	vec3 IBL_specular=prefiltered*(Fs*envBRDF.x+envBRDF.y);//IBL镜面反射部分
	vec3 outkd=1.0-outks;
	outkd*=(1.0-metallic);//根据金属度决定漫反射系数
	vec3 IBL_diffuse=outkd*albedo*irradiance;//IBL漫反射部分
	vec3 ambient=(IBL_diffuse+IBL_specular)*ao;//最终的环境照结果
	vec3 tempColor=ambient+Lo;//环境光照加光源
	//gamma校正
	tempColor=tempColor/(tempColor+vec3(1.0));
	tempColor=pow(tempColor,vec3(1.0/2.2));
	fragColor=vec4(tempColor,1.0);
	//fragColor=vec4(1.0,0.0,0.0,1.0);
}