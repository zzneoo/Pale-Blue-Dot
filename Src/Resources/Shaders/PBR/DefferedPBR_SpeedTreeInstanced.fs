#version 460 core

layout (location=0)out vec4 Out_WorldSpacePosition;
layout (location=1)out vec4 Out_WorldSpaceNormal;
layout (location=2)out vec4 Out_AlbedoAO;
layout (location=3)out vec4 Out_SpecRoughAlphaDither;
layout (location=4)out vec4 Out_Velocity;

uniform sampler2D Sampler2D_Albedo;
uniform sampler2D Sampler2D_Normal;

in vec3 WorldPos;
in vec2 outTexCoord;

smooth in vec4 CurrPos;
smooth in vec4 PrevPos;

in vec3 inTangent;
in vec3 inNormal;
in mat4 inPerInstanceModelX;

void main(void)
{
		
	vec4 Albedo = texture(Sampler2D_Albedo,outTexCoord);
	if(Albedo.a<0.1)
	discard;
	
	//Velocity
	vec2 CurrNDC=CurrPos.xy/CurrPos.w;
	vec2 CurrUV=CurrNDC*0.5+0.5;
	vec2 PrevNDC=PrevPos.xy/PrevPos.w;
	vec2 PrevUV=PrevNDC*0.5+0.5;	
	vec2 Velocity = (CurrUV-PrevUV);
	
	vec3 WorldSpaceSurfaceTangent = normalize((inPerInstanceModelX*vec4(inTangent,0.0)).xyz);
	vec3 WorldSpaceSurfaceNormal = normalize((inPerInstanceModelX*vec4(inNormal,0.0)).xyz);
	vec3 WorldSpaceSurfaceBiTangent = cross(WorldSpaceSurfaceNormal,WorldSpaceSurfaceTangent);
	
	mat3 TBN = mat3(WorldSpaceSurfaceTangent,WorldSpaceSurfaceBiTangent,WorldSpaceSurfaceNormal);
	
	vec4 NormalSpec = texture(Sampler2D_Normal,outTexCoord);
	vec3 Normal = NormalSpec.xyz;
	Normal =normalize(TBN*(Normal*2.0-1.0));
	//PBR material-------------------------------------------------------------------------------------------------

	Out_WorldSpacePosition = vec4(WorldPos.xyz,1.0);
	Out_WorldSpaceNormal = vec4(Normal,1.0);
	Out_AlbedoAO = vec4(Albedo.rgb*vec3(0.6,0.74,0.6),1.0);
	Out_SpecRoughAlphaDither = vec4(0.0,(1.0-NormalSpec.a)*0.0,1.0,1.0);
	Out_Velocity = vec4(Velocity,0.0,1.0);
	
} 

