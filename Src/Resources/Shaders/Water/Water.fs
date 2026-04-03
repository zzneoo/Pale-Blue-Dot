#version 460 core

layout (location=0)out vec4 FragColor;
layout (location=1)out vec4 FragColorMask;

in vec2 vsTexCoords;
in float waterHeight;
in float eyeDistance;
in float originDistance;
//in vec3 color;
in vec3 toSunVector;
in vec3 toEyeVector;//per pixel
uniform vec3 v3EyeDir;//worldEye dir
uniform vec3 v3EyePos;

uniform vec3 v3ColorFactor; //Gui
uniform vec3 v3SpecularColor; //Gui
uniform float fWaterFactor;//used for smoothwater near terrain

const float PI = 3.14159265359;
const vec3 Fdielectric = vec3(0.020018673);

uniform sampler2D tSampler_heightmap;
uniform sampler2D tSampler_normalmap;
uniform sampler2D tSampler_refraction;
uniform sampler2D tSampler_reflection;
uniform sampler2D tSampler_terrainDepth;

vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
	return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}

void main(void)
{

	// vec4 normalFlow=texture(tSampler_terrainNormal,tesTexCoords);
	
	// vec3 terrainNormal = normalFlow.rbg*2.0-1.0;
	// vec3 terrainTangent=vec3(terrainNormal.y,-terrainNormal.x,0.0);//experimental(works)
	// vec3 terrainBiTangent=vec3(0.0,-terrainNormal.z,terrainNormal.y);//experimental(works)

	// mat3 toTangentSpace=mat3(terrainTangent,terrainBiTangent,terrainNormal);
	//PBR material-------------------------------------------------------------------------------------------------
	// vec3 finalAlbedo=getIslandAlbedo(SWTR,RBWD,flow);
	// float finalRoughness = texture(tSampler_terrainRoughnessGrass,tesTexCoords*15).r;
	// float finalAO = texture(tSampler_terrainAO_Grass,tesTexCoords*15).r;
	// vec3 finalNormal=texture(tSampler_terrainNormalGrass,tesTexCoords*15).xyz*2.0-1.0;
	// vec3 finalMetalness = vec3(0.0);
	//PBR vectors----------------------------------------------------------------------------------------------------
	// vec3 N = normalize(toTangentSpace*finalNormal);// PBR normal instead vec3(0.0,0.0,1.0)
	// vec3 L = normalize(v3SunPos);
	// vec3 V = normalize(v3EyePos-worldPos.xyz);

	// vec3 F0 = mix(Fdielectric, finalAlbedo, finalMetalness);

	vec3 N =texture(tSampler_normalmap,vsTexCoords*8).rbg;
	float R =texture(tSampler_heightmap,vsTexCoords*8).r;
	vec3 N_Low =texture(tSampler_normalmap,vsTexCoords*1.0).rbg;

	N=mix(N,N_Low,clamp(smoothstep(0.02,0.1,eyeDistance*0.00002),0.0,1.0));

	float HeightLow =texture(tSampler_heightmap,vsTexCoords*0.01).r;

	vec3 L = normalize(toSunVector);
	vec3 V = normalize(toEyeVector);
	vec3 H = normalize(L+V);

	N.g=(N.g+HeightLow*0.0);
	N=mix(N*2.0-1.0,vec3(N.x*0.1,0.7,N.z*0.1),smoothstep(0.0,1.0,originDistance));



	 float NdotL = max(dot(N, L), 0.0);
	// float NdotV = max(dot(N, V), 0.0);
	// float NdotH = max(dot(N, H), 0.3); 

	R=pow(1.0-R,3.0);
	vec3 color = vec3(mix(mix(vec3(0.1,0.4,1.0),vec3(0.2,0.8,0.8),NdotL),vec3(0.1,0.4,1.0),R));

	float specular = pow(max(dot(V, (reflect(-L,N))),0.0),64.0);

	//FRESNEL
	vec3 F0 = mix(vec3(0.02), color, 0.0);
	vec3 F  = fresnelSchlick(F0, max(0.0, NdotL));

	//vec3 finalColor=mix((color*NdotL*0.3)+NdotV*vec3(0.0,0.1,0.07),(color*NdotL)+specular,F);
	vec3 finalColor=(color*NdotL);

	//Refraction test

	vec2 screenUVs=gl_FragCoord.xy/vec2(1920.0,1080.0);

	float cutoff=(dot(v3EyeDir,vec3(0.0,1.0,0.0)));
	float sunTheta=max(dot(toSunVector,vec3(0.0,1.0,0.0)),0.0);

	vec4 refract =texture(tSampler_refraction,(screenUVs)+N.xz*0.022*(1.0-min(v3EyePos.y*0.000475,1.0)));
	vec4 reflect =texture(tSampler_reflection,vec2(screenUVs.x,(1.0-screenUVs.y)+(cutoff*-2.1))+N.xz*0.072);


	//Edge smooth
	float near=0.1*max(v3EyePos.y,1.0);
	float far=20000.0*max(v3EyePos.y,1.0);
	float depth = gl_FragCoord.z;

	float groundDepth = texture(tSampler_terrainDepth,gl_FragCoord.xy/vec2(1920,1080)).r;

	float waterDistance= 2*near*far/(far+near -(2*depth-1)*(far-near));
	float groundDistance= 2*near*far/(far+near -(2*groundDepth-1)*(far-near));

	float alpha =clamp((groundDistance-waterDistance)*fWaterFactor,0.0,1.0);


	//FragColor =vec4(vec3(refract)*vec3(0.7,0.9,1.0),0.0);
	FragColor =vec4(mix(vec3(reflect),vec3(refract)*vec3(0.8,0.99,1.0)*NdotL+mix(0.0,0.1,smoothstep(0.05,0.2,sunTheta)),smoothstep(0.3,0.8,1.0-clamp(cutoff+0.4,0.0,1.0))),0.0);  //change reflect*0.5 
	FragColor.rgb+=specular*1.2*v3SpecularColor*alpha;

		//HEIGHT FOG
	FragColor =vec4(mix(mix(FragColor.rgb,vec3(0.8)*smoothstep(0.1,0.2,sunTheta),min(v3EyePos.y*0.000475,1.0)),FragColor.rgb,smoothstep(0.2,1.0,waterHeight*0.074)),1.0);
//	FragColor =vec4(vec3(originDistance),0.0);

	FragColorMask=vec4(1.0,0.0,0.0,1.0);

	FragColor.a=mix(alpha,1.0,min(v3EyePos.y*0.000475,1.0));

} 

