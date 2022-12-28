#version 330 core
in vec4 vertexColor;
in vec3 vertexPos;
in vec2 vertexUV;
out vec4 fragColor;
uniform sampler2D myTexture1;
uniform sampler2D myTexture2;
uniform float mixValue=0.2f;
void main()
{
	vec2 tempUV=vec2(1-vertexUV.x,vertexUV.y);
	fragColor=mix(texture(myTexture1,vertexUV),texture(myTexture2,tempUV),mixValue)*vertexColor;
}