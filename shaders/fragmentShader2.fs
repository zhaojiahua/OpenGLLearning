#version 330 core
in vec2 vertexUV;
out vec4 fragColor;
uniform vec4 outColor;
void main()
{
	fragColor=outColor;
}