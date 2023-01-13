#version 330 core

in vec2 vertexUV;
out vec4 fragColor;

uniform sampler2D diffTex;

void main()
{
	float tempdepth=texture(diffTex,vertexUV).r;
	//fragColor=vec4(1.0f,0.5f,0.5f,1.0f);
	fragColor=vec4(vec3(tempdepth),1.0f);
}