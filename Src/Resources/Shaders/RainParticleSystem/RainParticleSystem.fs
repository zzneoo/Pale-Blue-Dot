#version 460 core

out vec4 FragColor;
uniform sampler2D tSampler2D;
uniform float fTime;


void main(void)
{
	vec2 coord = gl_PointCoord;
    float sin_factor = sin(0.5);
    float cos_factor = cos(0.5);

    coord = vec2(coord.x - 0.5 , coord.y - 0.5) * mat2(cos_factor, sin_factor, -sin_factor, cos_factor);
	coord += 0.5;

	FragColor =texture(tSampler2D,coord).rrrr;
//	FragColor.b*=0.8;
//	FragColor.g*=1.0;
//	FragColor.r*=0.9;
	FragColor.a*=0.4;
	//FragColor =vec4(1);
}
