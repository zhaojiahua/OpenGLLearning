#version 330 core
#define PI 3.1415926535

in vec3 localPos;
out vec4 fragColor;

uniform samplerCube hdrcubemap;

void main()
{	
	vec3 normalVec=normalize(localPos);
	vec3 irradiance=vec3(0.0);
	//黎曼和求半球离散积分
	vec3 up=vec3(0.0,1.0,0.0);
	vec3 right=cross(up,normalVec);
	up=cross(normalVec,right);
	float delta=0.025;
	float sampleNum=0.0;
	for(float phi=0.0;phi<2.0*PI;phi+=delta){
		for(float theta=0.0;theta<0.5*PI;theta+=delta){
			vec3 tangentSample=vec3(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));//球面坐标变换到笛卡尔坐标
			vec3 sampleVec=tangentSample.x*right+tangentSample.y*up+tangentSample.z*normalVec;//切线空间转换到世界空间
			irradiance+=texture(hdrcubemap,sampleVec).rgb*cos(theta)*sin(theta);
			sampleNum++;
		}
	}
	irradiance=PI*irradiance/float(sampleNum);
	fragColor=vec4(irradiance,1.0);
	//fragColor=vec4(1.0,1.0,0.0,1.0);
}