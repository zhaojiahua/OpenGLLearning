#version 330 core

out vec3 fragColor;
in vec2 vertexUV;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D noiseTex;

uniform mat4 p_matrix;
uniform mat4 v_matrix;
uniform vec3 samples[64];
uniform vec2 noiseScale;

uniform float radius=1.0;

void main()
{
	vec3 fragPos=texture(gPosition,vertexUV).xyz;
	vec3 normal=texture(gNormal,vertexUV).rgb;
	vec3 randomVec=texture(noiseTex,vertexUV*noiseScale).xyz;
	vec3 tangent=normalize(randomVec-normal*dot(randomVec,normal));
	vec3 bitangent=cross(normal,tangent);
	mat3 TBN=mat3(tangent,bitangent,normal);

	float occlusion=0.0;
	for(int i=0;i<64;i++){
		vec3 sample=TBN*samples[i];//切线空间转换到观察空间
		sample=fragPos+sample*radius;
		vec4 offset=vec4(sample,1.0);
		offset=v_matrix*p_matrix*offset;//从观察空间变换到裁剪空间(并且转换到0.0-1.0的取值范围,方便采样深度值)
		offset.xyz/=offset.w;
		offset.xyz=offset.xyz*0.5+0.5;
		float sampleDepth=-texture(gPosition,offset.xy).a;//采样深度值
		float rangeCheck=smoothstep(0.0,1.0,radius/abs(fragPos.z-sampleDepth));//检测深度值是否在取值半径内
		occlusion+=(sampleDepth>=sample.z ? 1.0 : 0.0)*rangeCheck;
	}
	//vec3 tempfragColor=vec3(1.0-(occlusion/64.0));//遮蔽因子越大输出颜色越黑
	fragColor=vec3(1.0-(occlusion/64.0));
}