#version 310 es
precision mediump float;

layout(location = 0) out vec4 color;

uniform vec4 u_Color;
out vec4 FragColor;

in vec2 v_TexCoord;
uniform sampler2D u_Texture;

void main()
{
	FragColor = u_Color;
}