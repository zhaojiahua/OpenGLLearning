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

#define PI 3.1415926
#define POINTLIGHTNUM 4
uniform vec3 lightPositions[POINTLIGHTNUM];

float CalculateAttenuation(vec3 fragPos,vec3 lightPos){
	float distance=length(fragPos-lightPos);
	return 1.0/(0.8+0.03*distance+0.01*distance*distance);
}

//F 菲涅尔系数项函数
vec3 FresnelSchlick(float cosTheta,vec3 F0){
	return F0+(1.0-F0)*pow(1.0-cosTheta,5.0);
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
	vec3 normal=TBN*texture(texture_normal,vertexUV).rgb;//注意将法线从切线空间变换到视图空间
	float metallic=texture(texture_metallic,vertexUV).r;
	float roughness=texture(texture_roughness,vertexUV).r;
	float ao=texture(texture_ao,vertexUV).r;

	vec3 N=normalize(normal);
	vec3 V=normalize(viewPos-fragPos);
	vec3 Lo=vec3(0.0);
	for(int i=0;i<POINTLIGHTNUM;i++){
		vec3 L=normalize(lightPositions[i]-fragPos);
		vec3 H=normalize(V+L);
		vec3 lightColor=vec3(23.47,21.31,20.79);//先把点光源的辐射强度建模为一个常量辐射通量,这个点光源只朝一个方向发出辐射通量
		float cosTheta=max(dot(N,L),0.0);//我们将会根据入射角度和法线之间的夹角对辐射率缩放
		float attenuation=CalculateAttenuation(fragPos,lightPositions[i]);//计算点光源的衰减系数
		vec3 radiance=lightColor*attenuation*cosTheta;//求出辐射率

		//计算BRDF
		//F项
		vec3 F0=mix(vec3(0.04),albedo,metallic);//根据金属度融合金属和电介质的属性,金属度越大F0越接近albedo
		vec3 F=FresnelSchlick(max(dot(H,V),0.0),F0);
		//D项
		float D=DistributionGGX(N,H,roughness);
		//G项
		float G=GeometrySmith(N,V,L,roughness);

		//算出镜面反射项
		float denominator=max(4.0*max(dot(N,V),0.0)*max(dot(N,L),0.0),0.001);//配平因子(避免出现除0的情况)
		vec3 specular=(D*G*F)/denominator;

		//计算折射比例
		vec3 kd=vec3(1.0)-F;//菲涅尔算出来的就是反射比例
		kd*=(1.0-metallic);//金属度决定折射光线逃逸出来形成漫反射的强度,纯金属一般情况是折射光线全部立即被吸收,所以金属是没有漫反射的,所看到的全是反射光线
		
		//最后计算出射光强
		Lo+=(kd*albedo/PI+specular)*radiance;
	}
	vec3 ambient=0.05*albedo*ao;//给个环境光
	vec3 tempColor=ambient+Lo;
	//gamma校正
	tempColor=tempColor/(tempColor+vec3(1.0));
	tempColor=pow(tempColor,vec3(1.0/2.2));
	fragColor=vec4(tempColor,1.0);
	//fragColor=vec4(1.0,0.0,0.0,1.0);
}