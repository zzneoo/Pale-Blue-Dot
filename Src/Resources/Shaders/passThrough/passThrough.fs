#version 460 core

out vec4 FragColor;
uniform sampler2D tSampler2D;

in vec2 vsTexCoord;

void main(void)
{
	FragColor =texture(tSampler2D,vsTexCoord)*0.1;
//	FragColor =vec4(1);
}
