#version 460 core
	
	
layout(location=0)in vec4 vPosition;
layout(location=1)in vec2 vTexCoord;
layout(location=2)in vec3 vNormal;
layout(location=3)in vec3 vTangent;
//layout(location=4)in mat4 m4PerInstancedX;

uniform mat4 m4ModelX;
uniform mat4 m4ViewProjX;

out vec2 outTexCoord;
out vec3 WorldPos;

smooth out vec4 CurrPos;
smooth out vec4 PrevPos;

out vec3 inTangent;
out vec3 inNormal;
//out mat4 inPerInstanceModelX;

void main(void)
{
	//vec3 WorldSpaceSurfaceTangent = normalize((m4PerInstancedX*vec4(vTangent,0.0)).xyz);
	//vec3 WorldSpaceSurfaceNormal = normalize((m4PerInstancedX*vec4(vNormal,0.0)).xyz);
	//vec3 WorldSpaceSurfaceBiTangent = cross(WorldSpaceSurfaceNormal,WorldSpaceSurfaceTangent);
	
	//TBN = mat3(WorldSpaceSurfaceTangent,WorldSpaceSurfaceBiTangent,WorldSpaceSurfaceNormal);
	
	inTangent = vTangent;
	inNormal = vNormal;
//	inPerInstanceModelX = m4PerInstancedX;
	
	outTexCoord = vec2(vTexCoord.x,vTexCoord.y);
	WorldPos =  (m4ModelX * vPosition).xyz;
	
	gl_Position = m4ViewProjX * vec4(WorldPos,1.0);
}
