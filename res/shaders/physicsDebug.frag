#version 310 es

precision mediump float;

out vec4 FragColor;
in vec3 out_Color;

void main()
{
	FragColor = vec4(out_Color, 1.0); // show a plane colour
}
