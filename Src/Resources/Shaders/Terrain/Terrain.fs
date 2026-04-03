#version 460 core

layout (location=0)out vec4 Out_WorldSpacePosition;
layout (location=1)out vec4 Out_WorldSpaceNormal;
layout (location=2)out vec4 Out_AlbedoAO;

#define TALUS_SCALE 100
#define GRASS_SCALE 100
#define SAND_SCALE 100
#define ROCK_SCALE 100

#define TALUS_SCALE_LOW 15
#define GRASS_SCALE_LOW 15
#define SAND_SCALE_LOW 6.69
#define ROCK_SCALE_LOW 15



uniform sampler2D tSampler_heightmap;
uniform sampler2D tSampler_waterGradient;
uniform sampler2D tSampler_terrainNormal;

//grass

uniform sampler2D tSampler_terrainAlbedoGrass;
uniform sampler2D tSampler_terrainAO_Grass;
uniform sampler2D tSampler_terrainNormalGrass;
uniform sampler2D tSampler_terrainRoughnessGrass;


//sand

uniform sampler2D tSampler_terrainAlbedoSand;
uniform sampler2D tSampler_terrainAO_Sand;
uniform sampler2D tSampler_terrainNormalSand;
uniform sampler2D tSampler_terrainRoughnessSand;

//rock

uniform sampler2D tSampler_terrainAlbedoRock;
uniform sampler2D tSampler_terrainAO_Rock;
uniform sampler2D tSampler_terrainNormalRock;
uniform sampler2D tSampler_terrainRoughnessRock;

//talus

uniform sampler2D tSampler_terrainAlbedoTalus;
uniform sampler2D tSampler_terrainAO_Talus;
uniform sampler2D tSampler_terrainNormalTalus;
uniform sampler2D tSampler_terrainRoughnessTalus;

uniform sampler2D tSampler_terrainAlbedoLandSWTR;
uniform sampler2D tSampler_terrainAlbedoWaterRBWD;

uniform vec3 v3SunPos;
uniform vec3 v3EyePos;
uniform vec3 v3LightColor;

uniform mat4 m4ModelX;
uniform mat4 m4ViewX;

uniform float fTerrainFactor;

//shadow
uniform sampler2D tSampler_ShadowMap;
in vec4 lightSpaceFragPos;

in vec2 tesTexCoords;
in vec4 worldPos;
in float distance;
in float terrainHeight;

in vec3 toSunVector;
in vec3 toEyeVector;

const float PI = 3.14159265359;
const vec3 Fdielectric = vec3(0.04);


//Displacement  ---------------REMOVE THIS
//uniform sampler2D tSampler_terrainDisplacementGrass;
//uniform sampler2D tSampler_terrainDisplacementSand;
//uniform sampler2D tSampler_terrainDisplacementRock;
//uniform sampler2D tSampler_terrainDisplacementTalus;

vec3 reinhard(vec3 x) 
{
  return x / (1.0 + x);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////PBR2
float ndfGGX(float cosLh, float roughness)
{
	float alpha   = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float NdotL, float NdotV, float roughness)
{
	//float r = roughness + 1.0;
	//float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.

	//original k

	float k = roughness * 0.5;

	return gaSchlickG1(NdotL, k) * gaSchlickG1(NdotV, k);
}

// Shlick's approximation of the Fresnel factor.
vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
	return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float CalcShadow(void)
{
	vec3 pos = lightSpaceFragPos.xyz*0.5+0.5;

//	float shadowMap = texture(tSampler_ShadowMap,pos.xy).r;

	float shadow = 0;
	vec2 texelSize = 1.0/ vec2(8192.0*1.5f);

//	return shadowMap>pos.z?1.0:0.0;

	for (int x=-3;x<=3;++x)
	{
		for(int y =-3;y<=3;++y)
		{
			float depth = texture(tSampler_ShadowMap,pos.xy + vec2(x,y)*texelSize).r;
			shadow+= depth>pos.z?1.0:0.0;
		}
	}
	return shadow/47.0;
	//return 1.0;

}


vec4 getIslandColor(vec4 landMap,vec4 waterMap,float flow)
{
	//land
	vec4 grassColor=vec4(0.567647,0.672549,0.219607,1.0);
	vec4 rockColor=vec4(0.443137,0.368627,0.333333,1.0);
	vec4 talusColor=vec4(0.576470,0.556862,0.490196,1.0);
	vec4 roughnessColor=vec4(0.266666,0.266666,0.266666,1.0);
	vec4 beachSandColor=vec4(0.97647058,0.91764705,0.74509803,1.0);
	vec4 errosionColor=grassColor+0.2;
	//water
	vec4 waterGradient=texture(tSampler_waterGradient,vec2(waterMap.a,0.0));
	vec4 BaseWaterColor=vec4(0.20392156,0.41960784,0.41568627,1.0);

	//landcolor
	vec4 LandColor=mix(mix(mix(grassColor,rockColor,landMap.r)+talusColor*landMap.g*1.2,roughnessColor,landMap.a),errosionColor,flow*0.0);//
	LandColor=mix(LandColor,talusColor,landMap.b);
	LandColor=mix(LandColor,beachSandColor,waterMap.g);

	//watercolor
	vec4 WaterColor=mix(BaseWaterColor,waterGradient,0.57813);//0.57813
	WaterColor=mix(WaterColor,waterGradient*waterMap.r*2,0.20313);//0.20313

	return mix(LandColor,WaterColor,waterMap.b);
}

vec3 getIslandAlbedo(vec4 landMap,vec4 waterMap,float flow)
{
	//grass
	vec3 grassAlbedo=pow(texture(tSampler_terrainAlbedoGrass,tesTexCoords*GRASS_SCALE),vec4(1)).rgb;
	//rock
	vec3 rockAlbedo=pow(texture(tSampler_terrainAlbedoRock,tesTexCoords*ROCK_SCALE),vec4(1)).rgb;
	//sand
	vec3 beachSandAlbedo=pow(texture(tSampler_terrainAlbedoSand,tesTexCoords*SAND_SCALE),vec4(2.2)).rgb;
	//talus
	vec3 talusAlbedo=pow(texture(tSampler_terrainAlbedoTalus,tesTexCoords*TALUS_SCALE),vec4(1)).rgb;
	//roughness
	vec3 roughnessColor=vec3(0.266666,0.266666,0.266666);

    //final
	vec3 landAlbedo = mix(mix(mix(grassAlbedo*2.0,rockAlbedo,landMap.r*landMap.r*landMap.r),///slope
	roughnessColor,landMap.a*distance*0.8),//roughness
	talusAlbedo,landMap.b)//talus
	+landMap.g*distance*0.5 //wear
	+flow*max(distance,0.6)*0.4 //errosion
	;


	//water
	vec3 waterGradient=texture(tSampler_waterGradient,vec2(waterMap.a,0.0)).rgb;//depth
	vec3 BaseWaterColor=vec3(0.20392156,0.41960784,0.41568627);

	//watercolor
	vec3 WaterAlbedo=mix(BaseWaterColor,waterGradient,0.57813);//0.57813
	WaterAlbedo=mix(WaterAlbedo,waterGradient*waterMap.r*2,0.20313);//0.20313

	//return mix(mix(landAlbedo,WaterAlbedo,clamp(waterMap.b+waterMap.g,0,1)),beachSandAlbedo.rgb*1.4,waterMap.g);

	landAlbedo = beachSandAlbedo;//test
	return landAlbedo;//test
}

vec3 getIslandNormal(vec4 landMap,vec4 waterMap,float flow)
{
	//grass
	vec3 grassNormal = texture(tSampler_terrainNormalGrass,tesTexCoords*GRASS_SCALE).xyz*2.0-1.0;
	grassNormal.z*=0.56;
	//rock
	vec3 rockNormal = texture(tSampler_terrainNormalRock,tesTexCoords*ROCK_SCALE).xyz*2.0-1.0;
	rockNormal.z*=1.0;
	//talus
	vec3 talusNormal = texture(tSampler_terrainNormalTalus,tesTexCoords*TALUS_SCALE).xyz*2.0-1.0;
	talusNormal.z*=1.0;
	//beach
	vec3 beachNormal = texture(tSampler_terrainNormalSand,tesTexCoords*SAND_SCALE).xyz*2.0-1.0;
	beachNormal.z*=1.0;

	//final
	vec3 landNormal = mix(mix(mix(grassNormal,rockNormal,landMap.r*landMap.r),//slope
	talusNormal,landMap.b)//talus
	,beachNormal,waterMap.g)//sand
	;

	landNormal = beachNormal;//test

	return landNormal;
}

float getIslandAO(vec4 landMap,vec4 waterMap)
{
	//grass
	float grassAO =texture(tSampler_terrainAO_Grass,tesTexCoords*GRASS_SCALE).r;
	//Rock
	float rockAO =texture(tSampler_terrainAO_Rock,tesTexCoords*ROCK_SCALE).r;
	//talus
	float talusAO =texture(tSampler_terrainAO_Talus,tesTexCoords*TALUS_SCALE).r;
	//sand
	float sandAO =texture(tSampler_terrainAO_Sand,tesTexCoords*SAND_SCALE).r;

	//final
	float landAO = mix(mix(mix(grassAO,rockAO,landMap.r*landMap.r)//slope
	,talusAO,landMap.b)//talus
	,sandAO,waterMap.g);//sand

	landAO = sandAO;//test

	return landAO;
}

float getIslandRoughness(vec4 landMap,vec4 waterMap)
{
	//grass
	float grassRoughness =texture(tSampler_terrainRoughnessGrass,tesTexCoords*GRASS_SCALE).r;
	//Rock
	float rockRoughness =texture(tSampler_terrainRoughnessRock,tesTexCoords*ROCK_SCALE).r;
	//talus
	float talusRoughness =texture(tSampler_terrainRoughnessTalus,tesTexCoords*TALUS_SCALE).r;
	//sand
	float sandRoughness =texture(tSampler_terrainRoughnessSand,tesTexCoords*SAND_SCALE).r;

	//final
	float landRoughness = mix(mix(mix(grassRoughness,rockRoughness,landMap.r*landMap.r)//slope
	,talusRoughness,landMap.b)//talus
	,sandRoughness,waterMap.g);//sand

	landRoughness = sandRoughness;//test

	return landRoughness;
}

//ISLAND LOW
vec3 getIslandAlbedo_LOW(vec4 landMap,vec4 waterMap,float flow)
{
	//grass
	vec3 grassAlbedo=pow(texture(tSampler_terrainAlbedoGrass,tesTexCoords*GRASS_SCALE_LOW),vec4(1)).rgb;
	//rock
	vec3 rockAlbedo=pow(texture(tSampler_terrainAlbedoRock,tesTexCoords*ROCK_SCALE_LOW),vec4(1)).rgb;
	//sand
	vec3 beachSandAlbedo=pow(texture(tSampler_terrainAlbedoSand,tesTexCoords*SAND_SCALE_LOW),vec4(2.2)).rgb;
	//talus
	vec3 talusAlbedo=pow(texture(tSampler_terrainAlbedoTalus,tesTexCoords*TALUS_SCALE_LOW),vec4(1)).rgb;
	//roughness
	vec3 roughnessColor=vec3(0.266666,0.266666,0.266666);

    //final
	vec3 landAlbedo = mix(mix(mix(grassAlbedo*1.7,rockAlbedo,landMap.r*landMap.r),///slope
	roughnessColor,landMap.a*distance*0.8),//roughness
	talusAlbedo,landMap.b)//talus
	+landMap.g*distance*0.5 //wear
	+flow*distance*0.3 //errosion
	;
	//water
	vec3 waterGradient=texture(tSampler_waterGradient,vec2(waterMap.a,0.0)).rgb;//depth
	vec3 BaseWaterColor=vec3(0.20392156,0.41960784,0.41568627);

	//watercolor
	vec3 WaterAlbedo=mix(BaseWaterColor,waterGradient,0.57813);//0.57813
	WaterAlbedo=mix(WaterAlbedo,waterGradient*waterMap.r*2,0.20313);//0.20313

	return mix(mix(landAlbedo,WaterAlbedo,clamp(waterMap.b+waterMap.g,0,1)),beachSandAlbedo.rgb*1.4,waterMap.g);
	//return landAlbedo;
}

vec3 getIslandNormal_LOW(vec4 landMap,vec4 waterMap,float flow)
{
	//grass
	vec3 grassNormal = texture(tSampler_terrainNormalGrass,tesTexCoords*GRASS_SCALE_LOW).xyz*2.0-1.0;
	grassNormal.z*=0.56;
	//rock
	vec3 rockNormal = texture(tSampler_terrainNormalRock,tesTexCoords*ROCK_SCALE_LOW).xyz*2.0-1.0;
	rockNormal.z*=1.0;
	//talus
	vec3 talusNormal = texture(tSampler_terrainNormalTalus,tesTexCoords*TALUS_SCALE_LOW).xyz*2.0-1.0;
	talusNormal.z*=1.0;
	//beach
	vec3 beachNormal = texture(tSampler_terrainNormalSand,tesTexCoords*SAND_SCALE_LOW).xyz*2.0-1.0;
	beachNormal.z*=1.0;

	//final
	vec3 landNormal = mix(mix(mix(grassNormal,rockNormal,landMap.r*landMap.r),//slope
	talusNormal,landMap.b)//talus
	,beachNormal,waterMap.g)//sand
	;

	return landNormal;
}

float getIslandAO_LOW(vec4 landMap,vec4 waterMap)
{
	//grass
	float grassAO =texture(tSampler_terrainAO_Grass,tesTexCoords*GRASS_SCALE_LOW).r;
	//Rock
	float rockAO =texture(tSampler_terrainAO_Rock,tesTexCoords*ROCK_SCALE_LOW).r;
	//talus
	float talusAO =texture(tSampler_terrainAO_Talus,tesTexCoords*TALUS_SCALE_LOW).r;
	//sand
	float sandAO =texture(tSampler_terrainAO_Sand,tesTexCoords*SAND_SCALE_LOW).r;

	//final
	float landAO = mix(mix(mix(grassAO,rockAO,landMap.r*landMap.r)//slope
	,talusAO,landMap.b)//talus
	,sandAO,waterMap.g);//sand

	return landAO;
}

float getIslandRoughness_LOW(vec4 landMap,vec4 waterMap)
{
	//grass
	float grassRoughness =texture(tSampler_terrainRoughnessGrass,tesTexCoords*GRASS_SCALE_LOW).r;
	//Rock
	float rockRoughness =texture(tSampler_terrainRoughnessRock,tesTexCoords*ROCK_SCALE_LOW).r;
	//talus
	float talusRoughness =texture(tSampler_terrainRoughnessTalus,tesTexCoords*TALUS_SCALE_LOW).r;
	//sand
	float sandRoughness =texture(tSampler_terrainRoughnessSand,tesTexCoords*SAND_SCALE_LOW).r;

	//final
	float landRoughness = mix(mix(mix(grassRoughness,rockRoughness,landMap.r*landMap.r)//slope
	,talusRoughness,landMap.b)//talus
	,sandRoughness,waterMap.g);//sand

	return landRoughness;
}

void main(void)
{
	if(terrainHeight<-211.8)
	{
		discard;
	}
	else
	{

	//maps
	vec4 SlopeWearTalusRoughness=texture(tSampler_terrainAlbedoLandSWTR,tesTexCoords);
	vec4 RoughnessBeachWaterDepth=texture(tSampler_terrainAlbedoWaterRBWD,tesTexCoords);
	//TBN-----------------------------------------------------------------------------------------------------------
	vec4 normalFlow=texture(tSampler_terrainNormal,tesTexCoords);
	float flow=normalFlow.a;

	vec3 terrainNormal = normalFlow.rbg*2.0-1.0;
	vec3 terrainTangent=vec3(terrainNormal.y,-terrainNormal.x,0.0);//experimental(works for terrain only)
	vec3 terrainBiTangent=vec3(0.0,-terrainNormal.z,terrainNormal.y);//experimental(works for terrain only)

	mat3 TangentToWorldSpaceX=mat3(terrainTangent,terrainBiTangent,terrainNormal);
	//PBR material-------------------------------------------------------------------------------------------------
	vec3 finalAlbedo=getIslandAlbedo(SlopeWearTalusRoughness,RoughnessBeachWaterDepth,flow);
	float finalRoughness = getIslandRoughness(SlopeWearTalusRoughness,RoughnessBeachWaterDepth);
	float finalAO = getIslandAO(SlopeWearTalusRoughness,RoughnessBeachWaterDepth);
	vec3 finalNormal=getIslandNormal(SlopeWearTalusRoughness,RoughnessBeachWaterDepth,flow);

//	vec3 finalAlbedo_LOW=getIslandAlbedo_LOW(SlopeWearTalusRoughness,RoughnessBeachWaterDepth,flow);
//	float finalRoughness_LOW = getIslandRoughness_LOW(SlopeWearTalusRoughness,RoughnessBeachWaterDepth);
//	float finalAO_LOW = getIslandAO_LOW(SlopeWearTalusRoughness,RoughnessBeachWaterDepth);
//	vec3 finalNormal_LOW=getIslandNormal_LOW(SlopeWearTalusRoughness,RoughnessBeachWaterDepth,flow);

//	finalAlbedo=mix(finalAlbedo,finalAlbedo_LOW,distance);
//	finalRoughness=mix(finalRoughness,finalRoughness_LOW,distance);
	//finalAO=mix(finalAO,finalAO_LOW,distance);
	//finalNormal=mix(finalNormal,finalNormal_LOW,distance);

	vec3 finalMetalness = vec3(0.0);
	//PBR vectors----------------------------------------------------------------------------------------------------
	vec3 N = normalize(TangentToWorldSpaceX*finalNormal);// PBR normal instead vec3(0.0,0.0,1.0)
	vec3 L = normalize(v3SunPos);
	vec3 V = normalize(v3EyePos-worldPos.xyz);
	vec3 H = normalize(L+V);


	vec3 F0 = mix(Fdielectric, finalAlbedo, finalMetalness);

	float NdotL = max(dot(N, L), 0.00001);
	float NdotV = max(dot(N, V), 0.00001);
	float NdotH = max(dot(N, H), 0.00001); 

	//vec4 finalColor=getIslandColor(SWTR,RBWD,flow);

		//specular reflection vector
	//vec3 Lr = 2.0 * NdotV * N - V;

	vec3 directLighting = vec3(0);

	vec3 radiance = vec3(1);

				// Calculate Fresnel term for direct lighting. 
		vec3 F  = fresnelSchlick(F0, max(0.00001, dot(H,  V)));
		// Calculate normal distribution for specular BRDF.
		float D = ndfGGX(NdotH, finalRoughness);
		// Calculate geometric attenuation for specular BRDF.
		float G = gaSchlickGGX(NdotL, NdotV, finalRoughness);

		// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
		// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
		// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
		vec3 kd = mix(vec3(1.0) - F, vec3(0.0), 0.0);//metalness=0

		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
		vec3 diffuseBRDF = kd * finalAlbedo;

				// Cook-Torrance specular microfacet BRDF.
		vec3 specularBRDF = (F * D * G) / max(0.00001, 4.0 * NdotL * NdotV);

		// Total contribution for this light.
		directLighting = (diffuseBRDF + specularBRDF) * radiance * NdotL ;//for shadow->//* max(CalcShadow(),0.30)

		float SunTheta=max(dot(L,vec3(0.0,1.0,0.0)),0.0);

		//Ambient
	//	directLighting+=(0.3*pow(finalAO,1.0)*finalAlbedo);
	//	directLighting*=v3LightColor;

	/*
	FragColor =vec4((directLighting),1.0);



	//Height FOG
	float height =texture(tSampler_heightmap,vec2(tesTexCoords.x,1.0-tesTexCoords.y)).r;
	vec3 oldColor = directLighting;
	FragColor =vec4(mix(mix(directLighting,vec3(0.8)*smoothstep(0.1,0.2,SunTheta),min(v3EyePos.y*0.000475,1.0)),directLighting,smoothstep(0.12,0.61,terrainHeight*0.0022)),1.0);
	//FragColor =vec4(vec3(NdotL),0.0);
	FragColor =vec4(mix(FragColor.rgb,oldColor,step(height,0.159)),1.0);
	FragColorMask=vec4(1.0,0.0,0.0,1.0);

	//tonemap
	oldColor.rgb = 1.0-exp(-oldColor.rgb*2.0);
	//gamma correction
	FragColor=vec4(pow(oldColor,vec3(0.454545)),1.0);
	*/
	//shadow
	//	vec2 debugUV = lightSpaceFragPos.xy*0.5+0.5;
	//if(debugUV.x>1.0||debugUV.y>1.0||debugUV.x<0.0||debugUV.y<0.0)
	//{
	//	FragColor=vec4(1.0,0.0,0.0,1.0);
	//}
	Out_WorldSpacePosition = vec4(worldPos.xyz,1.0);
	Out_WorldSpaceNormal = vec4(N,finalRoughness);
	Out_AlbedoAO = vec4(finalAlbedo,finalAO);
	}

} 

