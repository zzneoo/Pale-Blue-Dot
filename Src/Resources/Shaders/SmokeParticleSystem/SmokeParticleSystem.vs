#version 460 core
		
layout(location=0)in vec4 vPosition;

uniform mat4 m4ModelViewProjX;
uniform unsigned int iFrameID;
uniform float fTime;

out float AlphaFactor;

void main(void)
{
	gl_Position = m4ModelViewProjX* vec4(vPosition.xyz,1.0);
	AlphaFactor= vPosition.y/2700.0;
}
