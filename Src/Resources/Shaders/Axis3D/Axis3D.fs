#version 460 core

layout (location=0)out vec4 Out_WorldSpacePosition;
layout (location=1)out vec4 Out_WorldSpaceNormal;
layout (location=2)out vec4 Out_AlbedoAO;
layout (location=3)out vec4 Out_SpecRoughAlphaDither;

uniform vec3 v3Color;
uniform float fMaxEyeDistance;

in vec3 worldPos;
in float flogz;
in vec2 PosZW;

//layout (depth_less) out float gl_FragDepth;

void main(void)
{	
	
	Out_WorldSpacePosition = vec4(worldPos,1.0);
	Out_WorldSpaceNormal = vec4(0.0,1.0,0.0,1.0);
	Out_AlbedoAO = vec4(v3Color,1.0);
	
	float alpha=0.0;
	if(length(worldPos)>0.0)
	{
		alpha=1.0;
	}
	
	Out_SpecRoughAlphaDither = vec4(1.0,1.0,alpha,0.0);
	
	float Fcoef = 1.0 / log2(fMaxEyeDistance + 1.0);
	gl_FragDepth = log2(flogz) * fMaxEyeDistance;
	//gl_FragDepth = log2(PosZW.y/10.0)/log2(20000000.0/10.0) ;
	//gl_FragDepth = PosZW.x/PosZW.y;
}
