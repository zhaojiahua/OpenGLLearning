#version 330 core

out vec4 fragColor;
void main()
{
	//fragColor=vec4(sin(gl_FragCoord.x),cos(gl_FragCoord.y),0.6f,1.0f);
	if(gl_FrontFacing)
		fragColor=vec4(sin(gl_FragCoord.x),0.0f,0.6f,1.0f);
	else
		fragColor=vec4(0.0f,cos(gl_FragCoord.y),0.6f,1.0f);
}