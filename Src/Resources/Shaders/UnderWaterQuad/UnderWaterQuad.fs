#version 460 core

layout (location=0)out vec4 FragColor;
layout (location=1)out vec4 FragColorMask;

uniform vec3 v3Color;
uniform vec3 v3SunPos;
uniform vec3 v3EyePos;

in vec2 TexCoord;
in float vsHeight;

void main(void)
{
	FragColorMask = vec4(1.0,0.0,0.0,1.0);

	vec3 L = normalize(v3SunPos);
	float SunTheta=max(dot(L,vec3(0.0,1.0,0.0)),0.0);

	FragColor.rgb =vec3(0.436, 0.691, 0.735)*v3Color*max(dot(vec3(0.0,1.0,0.0),L),0.0)*0.5 ;//
	
	//Height FOG
//	FragColor =vec4(mix(mix(FragColor.rgb,vec3(0.8)*smoothstep(0.1,0.2,SunTheta),min(v3EyePos.y*0.000475,1.0)),FragColor.rgb,smoothstep(0.12,0.61,vsHeight*0.0022)),1.0);
}
