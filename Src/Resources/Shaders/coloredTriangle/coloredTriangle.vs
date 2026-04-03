#version 460
		
layout(location=0)in vec4 vPosition;
layout(location=1)in vec4 vColor;
out vec4 out_Color;
uniform mat4 uMVPx;
void main(void)
{
	gl_Position = uMVPx * vPosition;
	out_Color=vColor;
}
