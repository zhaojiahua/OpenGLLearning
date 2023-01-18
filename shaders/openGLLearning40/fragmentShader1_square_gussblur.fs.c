#version 330 core

struct Material{
	sampler2D texture_diffuse1;
};

in vec2 vertexUV;

out vec4 fragColor;

uniform Material material;
uniform bool horizontal;
uniform float weight[5]=float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);//一个简单的高斯曲线数据

void main()
{	
	//vec2 unit_tex=1.0/textureSize(material.texture_diffuse1,0);//计算贴图的大小和单位像素
	//vec3 tempResult=texture(material.texture_diffuse1,vertexUV).rgb*weight[0];//当前像素的颜色值
	//if(horizontal){
	//	for(int i=0;i<5;i++){
	//		tempResult+=texture(material.texture_diffuse1,vertexUV+vec2(unit_tex.x*i,0.0)).rgb*weight[i];
	//		tempResult+=texture(material.texture_diffuse1,vertexUV-vec2(unit_tex.x*i,0.0)).rgb*weight[i];
	//	}
	//}
	//else{
	//	for(int i=0;i<5;i++){
	//		tempResult+=texture(material.texture_diffuse1,vertexUV+vec2(0.0,unit_tex.y*i)).rgb*weight[i];
	//		tempResult+=texture(material.texture_diffuse1,vertexUV-vec2(0.0,unit_tex.y*i)).rgb*weight[i];
	//	}
	//}
	//fragColor=vec4(tempResult,1.0);
	fragColor=vec4(1.0,1.0,0.0,1.0);
}