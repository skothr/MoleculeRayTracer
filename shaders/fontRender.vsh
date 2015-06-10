#version 330

layout(location = 0)in vec4 coord;
out vec2 texcoord;

void main()
{
	gl_Position = vec4(coord.x, -coord.y, 0, 1);
	texcoord = coord.zw;
}