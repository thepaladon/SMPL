#version 310 es
precision mediump float;

layout(location = 0) out vec4 color;

uniform vec4 u_Color;
out vec4 FragColor;

in vec2 v_TexCoord;
uniform sampler2D u_Texture;

void main()
{
	vec3 texColor = texture(u_Texture, v_TexCoord).xyz;
	FragColor = vec4(texColor, 1.0);
}