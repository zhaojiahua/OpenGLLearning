#version 330 core

layout(triangles) in;
layout(triangle_strip,max_vertices=3) out;

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

void main()
{
	//gl_Position=gl_in[0].gl_Position;	//一个图元通常包含多个Point
	//EmitVertex();
	//EndPrimitive();
	vec3 faceNormal=GetFaceNormal();
	gl_Position=Explor(gl_in[0].gl_Position,faceNormal);
	vertexUV=gs_in[0].avertexUV;
	normal=gs_in[0].anormal;
	fragPos=gs_in[0].afragPos;
	EmitVertex();
	gl_Position=Explor(gl_in[1].gl_Position,faceNormal);
	vertexUV=gs_in[1].avertexUV;
	normal=gs_in[1].anormal;
	fragPos=gs_in[1].afragPos;
	EmitVertex();
	gl_Position=Explor(gl_in[2].gl_Position,faceNormal);
	vertexUV=gs_in[2].avertexUV;
	normal=gs_in[2].anormal;
	fragPos=gs_in[2].afragPos;
	EmitVertex();
	EndPrimitive();
}