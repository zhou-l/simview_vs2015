#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out mediump vec4 texc;

void main()
{
	gl_Position.xyz = position.xyz;
	gl_Position.w = 1.0;
	texc = vec4(texCoord, 0, 1);
}
