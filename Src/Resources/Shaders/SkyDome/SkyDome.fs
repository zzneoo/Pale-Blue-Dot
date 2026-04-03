#version 460 core

layout (location=0)out vec4 FragColor;
//layout (location=1)out vec4 FragColorBW;

uniform vec3 v3LightPos;
uniform sampler2D tSampler2D_BlueNoise;
uniform unsigned int iFrameID;
uniform float fSkyFactor;

const float c_goldenRatioConjugate = 0.61803398875;

#define PI 3.14159265

vec2 resolution=vec2(1920,1080);

in vec3 v3Direction;
in vec3 RayleighColor;
in vec3 MieColor;

in float SunTheta;

float getMiePhase(float fCos, float fCos2, float g, float g2)
{
	return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos2) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
}

// Calculates the Rayleigh phase function
float getRayleighPhase(float fCos2)
{
	//return 1.0;
	return 0.75 + 0.75*fCos2;
}

float get_sun_phase(vec3 ray_dir, vec3 sun_dir, float g)
{
	float t = acos(dot(ray_dir, sun_dir)) ;
	float term_a = 1 - g * g;
	float term_b = 1 + g * g;
	float term_c = 2 * g * cos(t);
	float term_d = 4 * PI * pow(term_b - term_c, 3.0 / 2.0);
	float phase = term_a / term_d;
	return phase;
}

float getSunDisk(float cosTheta)
{
	float sun_angular_diameter_cos = 0.9997 ;
	float sundisk = smoothstep(sun_angular_diameter_cos, sun_angular_diameter_cos + 0.00004, cosTheta)*40;

	return sundisk;
}

vec3 U2ToneMapping(vec3 color)
 {
	const float A = 0.15;
	const float B = 0.50;
	const float C = 0.10;
	const float D = 0.20;
	const float E = 0.02;
	const float F = 0.30;

	color= (((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F);
//	color = pow(color, vec3(0.4545454545));
	return color;
 }

vec3 Uncharted2ToneMapping(vec3 color,float Exposure)
{
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	float W = 11.2;
	float exposure = Exposure;
	color *= exposure;
	color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
	float white = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
	color /= white;
//	color = pow(color, vec3(0.4545454545));
	return color;
}

vec3 filmicToneMapping(vec3 color)
{
	color = max(vec3(0.), color - vec3(0.004));
	color = (color * (6.2 * color + .5)) / (color * (6.2 * color + 1.7) + 0.06);
	return color;
}

vec3 SimpleToneMap(vec3 color)
{
	float exposure = 2.5;
	color=1.0 - exp(-color * exposure);
	color = pow(color, vec3(0.4545454545));
	return color;
}

vec3 simpleReinhardToneMapping(vec3 color)
{
	color *= 1. / (1. + color);
	color = pow(color, vec3(0.4545454545));
	return color;
}

vec3 lumaBasedReinhardToneMapping(vec3 color)
{
	float luma = dot(color, vec3(0.2126, 0.7152, 0.0722));
	float toneMappedLuma = luma / (1. + luma);
	color *= toneMappedLuma / luma;
	color = pow(color, vec3(0.4545454545));
	return color;
}

void main(void)
{
	 float g=-0.913;//0.9
	float g2=g*g;

	//BlueNoise
	vec3 BlueNoise  = texture(tSampler2D_BlueNoise, vec2(gl_FragCoord.xy /256.0)).xyz;
	//BlueNoise = fract(BlueNoise + float(iFrameID % 256) * c_goldenRatioConjugate);

	vec3 lightDir = normalize(v3LightPos);
	vec3 eyeDir = normalize(v3Direction);

	float fCos = dot(lightDir, eyeDir) ;
	float fCos2 =fCos*fCos;

//	float sunPhase=get_sun_phase(-eyeDir,lightDir,0.991);
	
	FragColor = vec4( RayleighColor*getRayleighPhase(fCos2)+(getMiePhase(fCos,fCos2,g,g2) * MieColor),1.0);//with Mie Scattering
//	FragColor = vec4( RayleighColor*getRayleighPhase(fCos2),1.0);//


	float SdotR = dot(-eyeDir, lightDir);

	float SunDisk = getSunDisk(SdotR);

	//FragColor.rgb+=FragColor.rgb*mix(SunDisk,0.5*getMiePhase(fCos,fCos2,-0.75,0.75*0.75)+SunDisk,mix(0.359,0.2,smoothstep(0.3,0.6,SunTheta)));//0.359
	FragColor.rgb+=(FragColor.rgb*0.017*getMiePhase(fCos,fCos2,-0.75,0.75*0.75)*SunDisk);//0.359

	FragColor.xyz+=(FragColor.xyz*(BlueNoise.r*2.0-1.0))*0.1;

	float angle=max(dot(lightDir,vec3(0.0,1.0,0.0)),0.0);
	float exposure=(angle)*2;


	FragColor.rgb=Uncharted2ToneMapping(FragColor.rgb,1.0+exposure);

//	FragColor=1.0-exp(-FragColor*(1.0+exposure));//try other tone mapping
//	FragColor.rgb=Uncharted2ToneMapping(FragColor.rgb+sun*RayleighColor,1.0+exposure);
//	FragColor.rgb=U2ToneMapping(FragColor.rgb);
//	FragColor.rgb=vec3(sun);
	FragColor.a=1.0;

//	FragColor.rgb=vec3;
}
