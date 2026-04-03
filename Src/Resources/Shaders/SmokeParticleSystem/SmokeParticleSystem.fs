#version 460 core

out vec4 FragColor;
uniform sampler2D tSampler2D;
uniform float fTime;

in float AlphaFactor;

void main(void)
{
	   vec2 coord = gl_PointCoord;
       float sin_factor = sin(fTime);
       float cos_factor = cos(fTime);

    coord = vec2(coord.x - 0.5 , coord.y - 0.5) * mat2(cos_factor, sin_factor, -sin_factor, cos_factor);
	coord += 0.5;

	FragColor =texture(tSampler2D,coord);
	FragColor.a*=0.037*pow((1.0-AlphaFactor),0.5);
	FragColor.b*=0.9;
	FragColor.r*=1.0;
	//FragColor =vec4(1);
}
