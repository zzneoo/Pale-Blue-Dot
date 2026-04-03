#version 460 core

layout (location=0)out vec4 FragColor;
layout (location=1)out vec4 FragColorMask;

uniform sampler2D Sampler2D_Albedo;
uniform sampler2D Sampler2D_Normal;
uniform sampler2D Sampler2D_Roughness;
uniform sampler2D Sampler2D_AO;

in vec2 outTexCoord;
in vec4 outColor;

in vec3 toLightVectorTangentSpace;
in vec3 toCameraVectorTangentSpace;

in vec3 surfaceNormalFs;
in vec2 vel;

const float PI = 3.14159265359;
const vec3 Fdielectric = vec3(0.04);

uniform vec3 v3TintColor;
// ----------------------------------------------------------------------------
vec3 reinhard2(vec3 x) 
{
	const float L_white = 4.0;
	return (x * (1.0 + x / (L_white * L_white))) / (1.0 + x);
}


vec3 tonemapFilmic(vec3 x)
{
	vec3 X = max(vec3(0.0), x - 0.004);
	vec3 result = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);
	return pow(result, vec3(2.2));
}


vec3 uncharted2Tonemap(vec3 x)
{
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	float W = 11.2;
	return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}


vec3 uncharted2(vec3 color) 
{
	const float W = 11.2;
	float exposureBias = 2.0;
	vec3 curr = uncharted2Tonemap(exposureBias * color);
	vec3 whiteScale = 1.0 / uncharted2Tonemap(vec3(W));
	return curr * whiteScale;
}


vec3 unreal(vec3 x)
{
	return x / (x + 0.155) * 1.019;
}

vec3 reinhard(vec3 color)
{
	const float gamma = 2.2;
	const float exposure = 1.0;
	const float pureWhite = 1.0;

	float luminance = dot(color, vec3(0.2126, 0.7152, 0.0722));
	float mappedLuminance = (luminance * (1.0 + luminance / (pureWhite * pureWhite))) / (1.0 + luminance);

	vec3 mappedColor = (mappedLuminance / luminance) * color;
	return vec3(pow(mappedColor, vec3(1.0 / gamma)));
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
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
	return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
	return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}

void main(void)
{
	FragColorMask=vec4(1.0,0.0,0.0,1.0);//colorAttachment1
	//PBR data
	float roughness = texture(Sampler2D_Roughness,outTexCoord).r;
	float AO = texture(Sampler2D_AO,outTexCoord).r;
	vec3 albedo = pow(texture(Sampler2D_Albedo,outTexCoord).rgb,vec3(1));
	vec3 normal = texture(Sampler2D_Normal,outTexCoord).rgb*2.0-1.0;
	vec3 metalness = vec3(0);

	vec3 F0 = mix(Fdielectric, albedo, metalness);

	vec3 N = normal;
	vec3 L = normalize(toLightVectorTangentSpace);
	vec3 V = normalize(toCameraVectorTangentSpace);
	vec3 H = normalize(L+V);

	float NdotL = max(dot(N, L), 0.0);
	float NdotV = max(dot(N, V), 0.0);
	float NdotH = max(dot(N, H), 0.0);

	//specular reflection vector
	vec3 Lr = 2.0 * NdotV * N - V;

	vec3 directLighting = vec3(0);

	vec3 radiance = vec3(1);

			// Calculate Fresnel term for direct lighting. 
		vec3 F  = fresnelSchlick(F0, max(0.0, dot(H,  V)));
		// Calculate normal distribution for specular BRDF.
		float D = ndfGGX(NdotH, roughness);
		// Calculate geometric attenuation for specular BRDF.
		float G = gaSchlickGGX(NdotL, NdotV, roughness);

		// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
		// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
		// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
		vec3 kd = mix(vec3(1.0) - F, vec3(0.0), 0.0);//metalness=0

		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
		vec3 diffuseBRDF = kd * albedo;

				// Cook-Torrance specular microfacet BRDF.
		vec3 specularBRDF = (F * D * G) / max(0.00001, 4.0 * NdotL * NdotV);

		// Total contribution for this light.
		directLighting += (diffuseBRDF + specularBRDF) * radiance * NdotL;

				//Ambient
		directLighting+=(0.3*pow(AO,1.0)*albedo);

		//tint
		directLighting*=v3TintColor;

//	FragColor =vec4(uncharted2(directLighting),1.0);
//	FragColor =vec4(directLighting,1.0);
	FragColor =vec4(vec3(max(dot(normal,L),0.32)),1.0);
//	FragColor =vec4(vec2(vel),0.0,1.0);
}
/*
//dont need PBR learnOpenGL anymore
	 // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);   
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 nominator    = NDF * G * F; 
    float denominator = 4 * NdotV * NdotL + 0.001; // 0.001 to prevent divide by zero.
    vec3 specular = nominator / denominator;

	// kS is equal to Fresnel
    vec3 kS = F;

	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - 0.0;//metallic=0	
	
	// scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);

	Lo += (kD * albedo / PI + specular) * radiance * NdotL; 

	vec3 ambient = vec3(0.3) * albedo * AO;
    
    vec3 color = ambient + Lo;

	// HDR tonemapping
   // color = color / (color + vec3(1.0));
   color = tonemapFilmic(color);
    // gamma correct
    color = pow(color, vec3(1.0/2.2));
	*/