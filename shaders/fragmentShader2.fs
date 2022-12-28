#version 330 core
out vec4 fragColor;
uniform vec4 outColor;
void main()
{
	fragColor=outColor;
}