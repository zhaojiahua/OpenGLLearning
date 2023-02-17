#version 330 core
layout(location=0) in vec4 pos_uv;//vec2 position,vec2 uv

out vec2 vertexUV;

uniform mat4 p_matrix;
uniform mat4 m_matrix;
uniform vec2 offset;

void main()
{
	float scale=30.0;
	gl_Position=p_matrix*m_matrix*vec4(pos_uv.xy*scale+offset,0.0,1.0);
	vertexUV=pos_uv.zw;
}