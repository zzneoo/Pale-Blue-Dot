#version 460 core

layout (location=0)out vec4 Out_WorldSpacePosition;
layout (location=1)out vec4 Out_WorldSpaceNormal;
layout (location=2)out vec4 Out_AlbedoAO;

uniform sampler2D tSampler_heightmap;
//uniform sampler2D tSampler_waterGradient;
uniform sampler2D tSampler_terrainNormal;

//grass

//uniform sampler2D tSampler_terrainAlbedoGrass;
//uniform sampler2D tSampler_terrainAO_Grass;
//uniform sampler2D tSampler_terrainNormalGrass;
//uniform sampler2D tSampler_terrainRoughnessGrass;

uniform sampler2D tSampler_terrainWearSlopeSnowAO;

uniform vec3 v3SunPos;
uniform vec3 v3EyePos;
uniform vec3 v3LightColor;

uniform mat4 m4ModelX;
uniform mat4 m4ViewX;

uniform float fTerrainFactor;

in vec2 tesTexCoords;
in vec4 worldPos;
in float distance;
in float terrainHeight;

in vec3 toSunVector;
in vec3 toEyeVector;

void main(void)
{
	vec3 Snow = vec3(0.97);
	vec3 Stone = vec3(0.1);
	vec3 Sand = pow(vec3(0.31640,0.257812,0.1875),vec3(2.2));
	//vec3 hill = 
	
	//maps
	vec4 WearSlopeSnowAO=texture(tSampler_terrainWearSlopeSnowAO,tesTexCoords);
	vec3 finalAlbedo=mix(Sand,Stone,smoothstep(0.4,0.5,terrainHeight/100));
		finalAlbedo= mix(finalAlbedo,finalAlbedo*WearSlopeSnowAO.r,0.6);
		finalAlbedo= mix(finalAlbedo,Snow,WearSlopeSnowAO.b);
	float finalAO = WearSlopeSnowAO.a;
	//TBN-----------------------------------------------------------------------------------------------------------
	vec4 normal=texture(tSampler_terrainNormal,vec2(tesTexCoords.x,tesTexCoords.y));

	vec3 terrainNormal = normal.rbg*2.0-1.0;
	//vec3 terrainTangent=vec3(terrainNormal.y,-terrainNormal.x,0.0);//experimental(works for terrain only)
	//vec3 terrainBiTangent=vec3(0.0,-terrainNormal.z,terrainNormal.y);//experimental(works for terrain only)

	//mat3 TangentToWorldSpaceX=mat3(terrainTangent,terrainBiTangent,terrainNormal);
	//PBR material-------------------------------------------------------------------------------------------------
	float finalRoughness = 0.7569;

	vec3 finalMetalness = vec3(0.0);
	
	//PBR vectors----------------------------------------------------------------------------------------------------
	//vec3 N = terrainNormal;//normalize(TangentToWorldSpaceX*finalNormal);// PBR normal instead vec3(0.0,0.0,1.0)


	Out_WorldSpacePosition = vec4(worldPos.xyz,1.0);
	Out_WorldSpaceNormal = vec4(terrainNormal,finalRoughness);
	Out_AlbedoAO = vec4(finalAlbedo,finalAO);

} 

