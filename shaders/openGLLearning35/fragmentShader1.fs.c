#version 330 core

//创建一个结构体来描述物体的材质属性
struct Material{
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
	sampler2D texture_specular3;
	sampler2D texture_normal1;
	sampler2D texture_normal2;
	sampler2D texture_normal3;
	sampler2D texture_height1;
	sampler2D texture_height2;
	sampler2D texture_height3;
	float shininess;
};
//平行光
struct DirLight{
	vec3 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};
//点光源
struct PointLight{
	vec3 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float kc;
	float kl;
	float kq;
};
//聚光灯
struct SpotLight{
	vec3 position;
	vec3 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float cutoff;
	float outercutoff;
};

in vec3 normal;
in vec2 vertexUV;
in vec3 fragPos;
in vec4 lightSpacePos;//用于计算阴影

out vec4 fragColor;

uniform vec3 viewPos;
uniform vec4 tempdiffuse_;
uniform Material material;
uniform sampler2D shadowMap;//用于采样深度贴图
#define DIRECTOINLIGHT_NUM 1
#define POINTLIGHT_NUM 4
#define SPOTLIGHT_NUM 1
uniform DirLight dirlights[DIRECTOINLIGHT_NUM];
uniform PointLight pointlights[POINTLIGHT_NUM];
uniform SpotLight spotlights[SPOTLIGHT_NUM];

float CalculateShadowFactor(vec3 normal,vec3 lightDir){//在阴影内的返回0.0,阴影外返回1.0
	vec3 pre_pos=lightSpacePos.xyz/lightSpacePos.w;//gl_Position会被OpenGL自动进行透视除法,然而这里我们要自己做透视除法
	pre_pos=pre_pos*0.5+0.5;//由于深度贴图的值都在0-1.0之间,所以这里我们做了一下适配
	float nearestDepth=texture(shadowMap,pre_pos.xy).r;//我们把坐标都变换到了0-1.0之间就相当于uv坐标,然后就可以直接从深度贴图中采样
	float currentDepth=pre_pos.z;//z值就是当前的深度值
	float bias=max(0.05*(1.0-dot(normal,lightDir)),0.005);
	float tempresult;
	vec2 texelSize=1.0/textureSize(shadowMap,0);//textureSize函数返回一个给定采样器纹理的0级mipmap的宽高,vec2类型
	for(int x=-1;x<=1;++x){
		for(int y=-1;y<=1;++y){
			float tempDepth=texture(shadowMap,pre_pos.xy+vec2(x,y)*texelSize).r;
			tempresult+=currentDepth-bias>tempDepth?0.0:1.0;
		}
	}
	tempresult/=9.0;
	if(pre_pos.z>1.0)tempresult=1.0;//超出视锥体的部分强制返回1.0
	return tempresult;
}

//处理平行光的函数
vec4 CalculateDirLight(DirLight light,vec3 normal,vec3 viewDir){
	vec3 lightDir=normalize(-light.direction);
	vec3 halfwayDir=normalize(lightDir+viewDir);//半程向量
	float diff=max(dot(lightDir,normal),0.0f);
	float spec=pow(max(dot(normal,halfwayDir),0.0f),material.shininess);//计算半程向量和法向量之间的夹角来确定高光反射的大小
	vec4 tempdiffuse=tempdiffuse_+vec4(texture(material.texture_diffuse1,vertexUV)+texture(material.texture_diffuse2,vertexUV)+texture(material.texture_diffuse3,vertexUV));
	vec4 ambient=light.ambient*tempdiffuse;
	vec4 diffuse=light.diffuse*diff*tempdiffuse;
	vec4 tempspecular=vec4(texture(material.texture_specular1,vertexUV)+texture(material.texture_specular2,vertexUV)+texture(material.texture_specular3,vertexUV));
	vec4 specular=light.specular*spec*min(tempspecular.r,1.0);
	//计算阴影
	float shadowFactor=CalculateShadowFactor(normal,lightDir);
	return ambient+shadowFactor*(diffuse+specular);//避免死黑而保留ambient
}

void main()
{
	vec3 norN=normalize(normal);
	vec3 viewDir=normalize(viewPos-fragPos);
	vec4 tempresult;
	for(int i=0;i<DIRECTOINLIGHT_NUM;i++){
		tempresult=CalculateDirLight(dirlights[i],norN,viewDir);
	}
	fragColor=tempresult;
}