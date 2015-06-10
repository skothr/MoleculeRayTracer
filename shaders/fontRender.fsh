#version 330

in vec2 texcoord;
out vec4 color;

uniform sampler2D tex;
uniform vec4 text_color;

void main()
{
	color = vec4(1, 1, 1, texture2D(tex, texcoord).r) * text_color;
}