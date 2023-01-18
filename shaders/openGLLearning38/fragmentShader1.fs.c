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
	sampler2D texture_height1;
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

in vec2 vertexUV;
in vec3 normal;
in vec3 fragPos;
in vec3 tangentLightPos;
in vec3 tangentViewPos;
in vec3 tangentFragPos;

out vec4 fragColor;

uniform float height_scale;
uniform float pointlight_far_plane;
uniform vec4 tempdiffuse_;
uniform Material material;
uniform samplerCube shadowMap;//用于采样立方体深度贴图
#define DIRECTOINLIGHT_NUM 1
#define POINTLIGHT_NUM 1
#define SPOTLIGHT_NUM 1
uniform DirLight dirlights[DIRECTOINLIGHT_NUM];
uniform PointLight pointlights[POINTLIGHT_NUM];
uniform SpotLight spotlights[SPOTLIGHT_NUM];

vec2 ParallaxMapping(vec2 texUV,vec3 viewDir){
	const float minlayernum=10;
	const float maxlayernum=20;
	float layernum=mix(maxlayernum,minlayernum,abs(dot(vec3(0.0,0.0,1.0),viewDir)));
	float layerDepth=1.0/layernum;
	float currentLayerDepth=0.0;
	vec2 p=viewDir.xy/viewDir.z*height_scale;
	vec2 deltaTexuv=p/layernum;
	
	vec2 currentuv=texUV;
	float curretnDepthMapValue=texture(material.texture_normal1,currentuv).r;

	while(currentLayerDepth<curretnDepthMapValue){
		currentuv -= deltaTexuv;
		curretnDepthMapValue=texture(material.texture_normal1,currentuv).r;
		currentLayerDepth+=layerDepth;
	}

	vec2 prevuv=currentuv+deltaTexuv;
	float afterDepth=curretnDepthMapValue-currentLayerDepth;
	float beforeDepth=texture(material.texture_normal1,prevuv).r-currentLayerDepth+layerDepth;

	float weight=afterDepth/(afterDepth-beforeDepth);

	return prevuv*weight+currentuv*(1.0-weight);
}

//处理点光源的函数
vec4 CalculatePointLight(PointLight light,vec3 normal,vec3 viewDir,vec2 offsetUV){
	vec3 lightDir=normalize(tangentLightPos-tangentFragPos);
	float diff=max(dot(lightDir,normal),0.0f);
	vec3 halfwayDir=normalize(lightDir+viewDir);
	float spec=pow(max(dot(normal,halfwayDir),0.0f),material.shininess);

	//衰减因子
	float distance=length(tangentViewPos-tangentFragPos);
	float attenuation=1.0/(light.kc+light.kl*distance+light.kq*distance*distance);

	vec4 tempdiffuse=vec4(texture(material.texture_diffuse1,offsetUV)+texture(material.texture_diffuse2,offsetUV)+texture(material.texture_diffuse3,offsetUV));
	vec4 ambient=light.ambient*tempdiffuse;
	vec4 diffuse=light.diffuse*diff*tempdiffuse;
	vec4 tempspecular=vec4(texture(material.texture_specular1,offsetUV)+texture(material.texture_specular2,offsetUV)+texture(material.texture_specular3,offsetUV));
	vec4 specular=light.specular*spec*tempspecular;

	return ambient+(diffuse+specular)*attenuation;
}

void main()
{
	vec3 viewDir=normalize(tangentViewPos-tangentFragPos);
	vec2 afteruv=ParallaxMapping(vertexUV,viewDir);
	//if(afteruv.x>1.0 || afteruv.y>1.0 || afteruv.x<0.0 || afteruv.y<0.0) discard;
	vec3 norN=texture(material.texture_normal1,vertexUV).rgb;
	norN=normalize(norN*2.0-1.0);//将法线信息线性转换到-1到1范围内并归一化
	vec4 tempresult;
	for(int i=0;i<POINTLIGHT_NUM;i++){
		tempresult=CalculatePointLight(pointlights[i],norN,viewDir,afteruv);
	}
	fragColor=tempresult;
	
}