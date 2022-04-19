#version 310 es

precision mediump float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 MVP;
out vec3 out_Color;

void main()
{
	gl_Position =  MVP * vec4(aPos, 1.0);
	out_Color = aColor;
}
