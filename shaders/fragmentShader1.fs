#version 330 core
in vec4 vertexColor;
in vec3 vertexPos;
out vec4 fragColor;
void main()
{
	fragColor=vec4(vertexPos,1.0f);
}