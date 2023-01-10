#version 330 core

layout(points) in;
layout(triangle_strip,max_vertices=5) out;

in VS_OUT{
	vec3 color;
} gs_in[];
out vec3 fColor;
void BuidHouse(vec4 pointPosition){
	//每个点发射5个点组成一个几何形状
	fColor=gs_in[0].color;
	gl_Position=pointPosition+vec4(-0.2,-0.2,0.0,0.0);
	EmitVertex();
	gl_Position=pointPosition+vec4(0.2,-0.2,0.0,0.0);
	EmitVertex();
	gl_Position=pointPosition+vec4(-0.2,0.2,0.0,0.0);
	EmitVertex();
	gl_Position=pointPosition+vec4(0.2,0.2,0.0,0.0);
	EmitVertex();
	gl_Position=pointPosition+vec4(0.0,0.4,0.0,0.0);
	fColor=vec3(1.0);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	//gl_Position=gl_in[0].gl_Position;	//一个图元通常包含多个Point
	//EmitVertex();
	//EndPrimitive();
	BuidHouse(gl_in[0].gl_Position);
}