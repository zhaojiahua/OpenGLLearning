#version 330 core

layout(triangles) in;
layout(line_strip,max_vertices=6) out;

in VS_OUT{
	vec2 avertexUV;
	vec3 anormal;
	vec3 afragPos;
} gs_in[];
out vec2 vertexUV;
out vec3 normal;
out vec3 fragPos;

uniform float time;

vec3 GetFaceNormal(){
	vec3 side1=vec3(gl_in[0].gl_Position)-vec3(gl_in[1].gl_Position);
	vec3 side2=vec3(gl_in[0].gl_Position)-vec3(gl_in[2].gl_Position);
	return normalize(cross(side1,side2));
}
vec4 Explor(vec4 position,vec3 normal){
	float mag=0.2;
	vec3 direction=normal*((sin(time)+1.0)/2.0)*mag;
	return position+vec4(direction,0.0);
}
void GenerateNormalLine(int index){
	gl_Position=gl_in[index].gl_Position;
	EmitVertex();
	gl_Position=gl_in[index].gl_Position+vec4(gs_in[index].anormal,0)*0.5;
	EmitVertex();
	EndPrimitive();
}

void main()
{
	//gl_Position=gl_in[0].gl_Position;	//一个图元通常包含多个Point
	//EmitVertex();
	//EndPrimitive();
	GenerateNormalLine(0);
	GenerateNormalLine(1);
	GenerateNormalLine(2);
}