#version 460 core

uniform sampler2D Sampler2D_Albedo;

in vec2 outTexCoord;



void main(void)
{

	float alpha = texture(Sampler2D_Albedo,outTexCoord).a;
	if(alpha<0.1)
	discard;

}
