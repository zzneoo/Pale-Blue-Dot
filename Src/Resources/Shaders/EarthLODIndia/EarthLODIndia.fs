#version 460 core

layout (location=0)out vec4 Out_WorldSpacePosition;
layout (location=1)out vec4 Out_WorldSpaceNormal;
layout (location=2)out vec4 Out_AlbedoAO;
layout (location=3)out vec4 Out_SpecRoughAlphaDither;
layout (location=4)out vec4 Out_Velocity;

uniform sampler2D tSampler_NormalAO;
uniform sampler2D tSampler_EarthLODIndiaAlbedo;
uniform sampler2D tSampler_EarthLODBlend;

uniform vec3 v3TintColor;

in mat3 TangentToWorld;

in vec2 tesTexCoords;
in vec4 worldPos;
smooth in vec4 CurrPos;
smooth in vec4 PrevPos;

in float flogz;
in float depth;
uniform float FcoefForLog;

//layout (depth_less) out float gl_FragDepth;

void main(void)
{
	
	vec2 UV = vec2(tesTexCoords.x,tesTexCoords.y);
	//Velocity
	vec2 CurrNDC=CurrPos.xy/CurrPos.w;
	vec2 CurrUV=CurrNDC*0.5+0.5;
	vec2 PrevNDC=PrevPos.xy/PrevPos.w;
	vec2 PrevUV=PrevNDC*0.5+0.5;	
	vec2 Velocity = (CurrUV-PrevUV);
	
	//TBN-----------------------------------------------------------------------------------------------------------
	vec4 NormalAO=texture(tSampler_NormalAO,vec2(UV));
	vec3 Normal = normalize(NormalAO.rgb*2.0-1.0);
		Normal.b*=2.0;
	Normal = TangentToWorld*Normal;
	
	//PBR material-------------------------------------------------------------------------------------------------
	float finalRoughness = 0.470;
	vec3 finalMetalness = vec3(0.0);		
	vec4 EarthLOD1Albedo = texture(tSampler_EarthLODIndiaAlbedo,UV);
	float alpha = texture(tSampler_EarthLODBlend,UV).r;
		
	Out_WorldSpacePosition = vec4(worldPos.xyz,1.0);
	Out_WorldSpaceNormal = vec4(Normal,1.0);
	Out_AlbedoAO = vec4(mix(EarthLOD1Albedo.rgb,EarthLOD1Albedo.rgb*v3TintColor,EarthLOD1Albedo.a),1.0);
	Out_SpecRoughAlphaDither = vec4(EarthLOD1Albedo.a,finalRoughness,alpha,1.0);
	Out_Velocity = vec4(Velocity,0.0,1.0);
	
	//Logarithmic depth
	gl_FragDepth = log2(flogz) * FcoefForLog;
	//gl_FragDepth = log2(CurrPos.w/10.0)/log2(20000000.0/10.0);
	//gl_FragDepth = depth;
} 

