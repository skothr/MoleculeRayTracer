#version 330

in vec2 texcoord;
out vec4 color;

uniform sampler2D tex;

void main()
{
    color = texture2D(tex, texcoord);// + vec4(0.2, 0.2, 0.2, -0.5);
}