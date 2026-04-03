#version 460 core
		
layout(location=0)in vec4 vPosition;
layout(location=1)in vec2 vTexCoord;

uniform mat4 m4ModelViewProjX;
uniform unsigned int iFrameID;
uniform float fTime;

out vec2 vsTexCoord;

void main(void)
{
	gl_Position =m4ModelViewProjX* vPosition;

	vsTexCoord=vTexCoord;
}
