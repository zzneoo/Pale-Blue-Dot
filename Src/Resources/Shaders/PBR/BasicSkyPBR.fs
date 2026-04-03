#version 460 core

const float PI = 3.14159265359;
const vec3 Fdielectric = vec3(0.04);
const float kLengthUnitInMeters = 100.0;
const float AtmoTopRadius = 66200.0;

layout (location = 0 )out vec4 FragColor_frame;
layout (location = 1 )out vec4 CloudHorizonAlpha_frame;

uniform layout(rg32f, binding = 0) writeonly image2D CameraVelocityImage;

uniform float fArtistFactor;
uniform float fTime;
uniform float fExposure;

uniform vec3 v3SunPos;
uniform vec3 v3EyePos;
uniform vec3 v3ColorPicker;
uniform vec3 v3EarthCenter;
uniform vec2 v2SunSize;

uniform mat4 m4PrevViewProjX;
uniform mat4 m4LightViewProjX;

uniform sampler2D Sampler2D_WorldSpacePosition;
uniform sampler2D Sampler2D_WorldSpaceNormal;
uniform sampler2D Sampler2D_AlbedoAO;
uniform sampler2D Sampler2D_SpecRoughAlphaDither;
uniform sampler2D Sampler2D_DefferedVelocity;

//Special
uniform sampler2D Sampler2D_Clouds;
uniform sampler2D Sampler2D_BlueNoise;
uniform sampler2D Sampler2D_CloudShadowMap;
uniform sampler3D Sampler3D_FroxelTransmittance;
uniform sampler3D Sampler3D_FroxelInScatter;

in vec2 TexCoord;
in vec3 v3ViewRay;
in vec4 v4FarWorldPos;

vec3 GetSolarRadiance();
vec3 GetSkyRadiance(vec3 camera, vec3 view_ray, float shadow_length,
    vec3 sun_direction, out vec3 transmittance);
vec3 GetSkyRadianceToPoint(vec3 camera, vec3 point, float shadow_length,
    vec3 sun_direction, out vec3 transmittance);
vec3 GetSunAndSkyIrradiance(
    vec3 p, vec3 normal, vec3 sun_direction, out vec3 sky_irradiance);
vec3 GetSunAndSkyIrradianceFog(
    vec3 p, vec3 normal, vec3 sun_direction, out vec3 sky_irradiance, out vec3 SunGlobalIrradiance);

float getSunDisk(float cosTheta)
{
   float sun_angular_diameter_cos = 0.9997;
   float sundisk = smoothstep(sun_angular_diameter_cos, sun_angular_diameter_cos + 0.00004, cosTheta) * 40;
   return sundisk;
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


vec2 GetLOD_UV(float lodScale,vec2 baseUV,vec2 lodOffset,vec2 aspectCorrectionFactor) //Xoffset-0.25 generic
{
    vec2 aspectCorrectScale = vec2(lodScale*2*aspectCorrectionFactor.x,lodScale*aspectCorrectionFactor.y);
    vec2 finalScale = 64*aspectCorrectScale;
    vec2 offset = (aspectCorrectScale*0.5)+vec2(0.5)-aspectCorrectScale;
    
    return ((vec2(baseUV.s,baseUV.t)*finalScale)-(vec2(48.974530+lodOffset.x,41.22625+lodOffset.y)*aspectCorrectScale)+vec2(offset.x,offset.y));
}

vec3 Nec96(float sunSizeY,float CosTheta)
{
    float centerToEdge = (1.0 - CosTheta)/ (1.0- sunSizeY);
    float mu = sqrt(1.0 - centerToEdge * centerToEdge);
     vec3 u = vec3(1.0, 1.0, 1.0); // some models have u!=1
     vec3 a = vec3(0.397, 0.503, 0.652);
     vec3 factor = 1.0 - u * (1.0 - pow(vec3(mu), a));
     return factor;
}

vec3 HM98(float sunSizeY,float CosTheta)
{
    float centerToEdge = (1.0 - CosTheta)/ (1.0- sunSizeY);
    float mu = sqrt(1.0 - centerToEdge * centerToEdge);
    
     // coefficient for RGB wavelength (680 ,550 ,440)
      vec3 a0 = vec3(0.34685, 0.26073, 0.15248);
      vec3 a1 = vec3(1.37539, 1.27428, 1.38517);
      vec3 a2 = vec3(-2.04425, -1.30352, -1.49615);
      vec3 a3 = vec3(2.70493, 1.47085, 1.99886);
      vec3 a4 = vec3(-1.94290, -0.96618, -1.48155);
      vec3 a5 = vec3(0.55999, 0.26384, 0.44119);

     float mu2 = mu * mu;
     float mu3 = mu2 * mu;
     float mu4 = mu2 * mu2;
     float mu5 = mu4 * mu;
     vec3 factor = a0 + a1 * mu + a2 * mu2 + a3 * mu3 + a4 * mu4 + a5 * mu5;
     return factor;
}

vec3 BiCubicCatmullRom5Tap (sampler2D sampler, vec2 P,vec2 textureSize)
{

    vec2 Weight[3];
	vec2 Sample[3];
    
    vec2 InvSize = vec2(1.0/textureSize);

    vec2 UV =  P * textureSize;
    vec2 tc = floor(UV - 0.5) + 0.5;
	vec2 f = UV - tc;
	vec2 f2 = f * f; 
	vec2 f3 = f2 * f;
    
    vec2 w0 = f2 - 0.5 * (f3 + f);
	vec2 w1 = 1.5 * f3 - 2.5 * f2 + vec2(1.);
	vec2 w3 = 0.5 * (f3 - f2);
	vec2 w2 = vec2(1.) - w0 - w1 - w3;
    
    Weight[0] = w0;
	Weight[1] = w1 + w2;
	Weight[2] = w3;

	Sample[0] = tc - vec2(1.);
	Sample[1] = tc + w2 / Weight[1];
	Sample[2] = tc + vec2(2.);

	Sample[0] *= InvSize;
	Sample[1] *= InvSize;
	Sample[2] *= InvSize;

    float sampleWeight[5];
    sampleWeight[0] = Weight[1].x * Weight[0].y;
    sampleWeight[1] = Weight[0].x * Weight[1].y;
    sampleWeight[2] = Weight[1].x * Weight[1].y;
    sampleWeight[3] = Weight[2].x * Weight[1].y;
    sampleWeight[4] = Weight[1].x * Weight[2].y;

    vec3 Ct = texture(sampler, vec2(Sample[1].x, Sample[0].y)).rgb * sampleWeight[0];    
    vec3 Cl = texture(sampler, vec2(Sample[0].x, Sample[1].y)).rgb * sampleWeight[1];    
    vec3 Cc = texture(sampler, vec2(Sample[1].x, Sample[1].y)).rgb * sampleWeight[2];    
    vec3 Cr = texture(sampler, vec2(Sample[2].x, Sample[1].y)).rgb * sampleWeight[3];    
    vec3 Cb = texture(sampler, vec2(Sample[1].x, Sample[2].y)).rgb * sampleWeight[4];
    
    float WeightMultiplier = 1./(sampleWeight[0]+sampleWeight[1]+sampleWeight[2]+sampleWeight[3]+sampleWeight[4]);
    
    
    return (Ct+Cl+Cc+Cr+Cb)*WeightMultiplier;   
}

//Post Process
vec3 rgb2hsb( in vec3 c )
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz),
                 vec4(c.gb, K.xy),
                 step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r),
                 vec4(c.r, p.yzx),
                 step(p.x, c.r));
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)),
                d / (q.x + e),
                q.x);
}

vec3 hsb2rgb( in vec3 c )
{
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                             6.0)-3.0)-1.0,
                     0.0,
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix(vec3(1.0), rgb, c.y);
}
vec3 changeContrast(vec3 color,float fContrast)
{
    return ((color.rgb - 0.5f) * max(fContrast, 0)) + 0.5f;
}
vec3 adjustContrast(vec3 color, float value)
{
  return 0.5 + (1.0 + value) * (color - 0.5);
}

//ToneMappers
vec3 filmicToneMapping(vec3 color)
{
	color = max(vec3(0.), color - vec3(0.004));
	color = (color * (6.2 * color + .5)) / (color * (6.2 * color + 1.7) + 0.06);
	return color;
}
vec3 Uncharted2ToneMapping(vec3 color , float exposure)
{
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	float W = 11.2;
	color *= exposure;
	color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
	float white = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
	color /= white;
	return color;
}

vec3 simpleReinhardToneMapping(vec3 color)
{

	color *= 1. / (1. + color);
	return color;
}

vec3 unreal(vec3 x) 
{
  return x / (x + 0.155) * 1.019;
}
vec3 aces(vec3 x) 
{
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

float anflare(vec2 uv, float intensity, float stretch,float height, float brightness)
{
    uv.x *= 1.0/(intensity*stretch);
    uv.y *= height;
    return smoothstep(0.009, 0.0, length(uv))*brightness;
}

vec3 IntersectSphereFirstFromOutside(const vec3 pos, const vec3 dir, const float r,out float step)
{

	vec3 L = pos - v3EarthCenter;

	float a = dot(dir, dir);
	float b = 2.0 * dot(dir, L);
	float c = dot(L, L) - (r * r);
	float d = sqrt((b * b) - 4.0 * a * c);

	float p1 = -b - d;//from outside to first intersection
	//float p2 = -b + d;//from outside to second intersection

	float t = p1/ (2.0 * a);
	t = max(0, t);

    step =t;

	return pos + dir * t;
}

vec3 IntersectSphereFromInside(const vec3 pos, const vec3 dir, const float r)
{

	vec3 L = pos - v3EarthCenter;

	float a = dot(dir, dir);
	float b = 2.0 * dot(dir, L);
	float c = dot(L, L) - (r * r);
	float d = sqrt((b * b) - 4.0 * a * c);

	//float p = -b - d;
	float p2 = -b + d;

	float t = p2/ (2.0 * a);
	t = max(0, t);
	return pos + dir * t;
}

vec3 IntersectSphereSecondFromOutside(const vec3 pos, const vec3 dir, const float r,out float step)
{

	vec3 L = pos - v3EarthCenter;

	float a = dot(dir, dir);
	float b = 2.0 * dot(dir, L);
	float c = dot(L, L) - (r * r);
	float d = sqrt((b * b) - 4.0 * a * c);

	//float p1 = -b - d;//from outside to first intersection
	float p2 = -b + d;//from outside to second intersection

	float t = p2/ (2.0 * a);
	t = max(0, t);

    step =t;

	return pos + dir * t;
}
vec3 IntersectSphereMidFromOutside(const vec3 pos, const vec3 dir, const float r,out float step)
{

	vec3 L = pos - v3EarthCenter;

	float a = dot(dir, dir);
	float b = 2.0 * dot(dir, L);
	float c = dot(L, L) - (r * r);
	float d = sqrt((b * b) - 4.0 * a * c);

	float p1 = -b - d;//from outside to first intersection
	float p2 = -b + d;//from outside to second intersection

	float t1 = p1/ (2.0 * a);
	t1 = max(0, t1);

    float t2 = p2/ (2.0 * a);
	t2 = max(0, t2);

    step = (t1+t2)*0.5;

	return pos + dir * step;
}

float scatter(float LoV)
{
    float PI = 3.14;
    float G_SCATTERING = 0.2f;
    float result = 1.0f - G_SCATTERING * G_SCATTERING;
    result /= (4.0f * PI * pow(1.0f + G_SCATTERING * G_SCATTERING - (2.0f * G_SCATTERING) * LoV, 1.5f));
    return result;
}

float linearize_depthVulkan(float d, float zNear, float zFar)
{
	return zNear * zFar / (zFar + d * (zNear - zFar));
}

float Rayleigh(float mu)
{
  return 3.0 / 4.0 * 1.0 / (4.0*PI)*(1.0 + mu*mu);
}

float Mie(float mu, float g)
{
  // Henyey-Greenstein phase function
  return (1.0 - g*g) / ((4.0*PI) * pow(1.0 + g*g - 2.0*g*mu, 1.5));
}

void HeightFog(
  float          inDistance,       // Distance [m]
  float          inCameraPosY,     // Altitude of the camera position [m]
  float          inWorldPosY,      // Altitude of the world position [m]
  float          inSoV,            // dot(sun_dir, view_dir) [-]
  out vec3     outInscatter,     // in-scatter [-]
  out vec3     outTransmittance, // transmittance [-]
  vec3 SkyIrradiance,
  vec3 SunIrradiance)
{

  float mScaleHeight = 2.0;
  float fogDensity = 1.0;
  vec3 mSunColor =SunIrradiance;
  vec3 mAmbColor =SkyIrradiance;
  vec3 mBetaRs = vec3(0.00058023393817123806,0.0013557762447920219,0.0033100005976367734);
  vec3 mBetaMs = vec3(0.00039960000000000006);
  float mBetaMa = 0.0;

  const vec3 beta_t = vec3(mBetaRs + (mBetaMs + mBetaMa));

  // Transmittance
  float t = max(1e-2, (inCameraPosY - inWorldPosY) / mScaleHeight);
  t = (1.0 - exp(-t)) / t * exp(-inWorldPosY / mScaleHeight);
  vec3 transmittance = exp(-inDistance * t * beta_t*fogDensity);
  
  // Inscatter
  vec3 single_r =  mBetaRs * Rayleigh(inSoV);
  vec3 single_m =  mBetaMs * Mie(inSoV, 0.8);
  vec3 inscatter = mSunColor * (single_r + single_m)*100.0;
  inscatter += mAmbColor * (mBetaRs + mBetaMs);
  inscatter /= beta_t;
  
  outInscatter = inscatter * (1.0 - transmittance);
  outTransmittance = transmittance;
  }

  float random(vec2 st) 
  {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
  }

void main(void)
{
    vec4 WorldSpacePosition = texture(Sampler2D_WorldSpacePosition,TexCoord);  
    vec4 SpecRoughAlphaDither = texture(Sampler2D_SpecRoughAlphaDither,TexCoord);
    float GodRayIntensity = 0.0;
    vec3 BlueNoise = texture(Sampler2D_BlueNoise,TexCoord*vec2(3840.0,2160.0)/256.0f).rgb;
   // WorldSpacePosition.xyz = mix(v4FarWorldPos.xyz/v4FarWorldPos.w,WorldSpacePosition.xyz,SpecRoughAlphaDither.b);

	vec3 ViewRay = normalize(v3ViewRay);	
    vec2 DefferedVelocity=vec2(0.0);

    float LinearEyeDepth;
    vec3 FogSkyIrradiance;
    vec3 FogSunIrradiance;
    vec3 RadiancePBR;
    vec3 F0 = Fdielectric;
     //PBR vectors----------------------------------------------------------------------------------------------------
	vec3 L = normalize(v3SunPos);//towards light
	vec3 V = normalize(-ViewRay);//towards camera(ViewRay is Camera to object ray)
	vec3 H = normalize(V+L);//  N+V or N+L

    float toSunViewCosTheta = max(dot(L, ViewRay), 0.0); 

    float AmbientIntensity = 0.77;
    //float AmbientIntensity = 0;

        //RaySphere intersection for Earth
    vec3 ZenithDistance = v3EyePos - v3EarthCenter;//or CameraPos relative to earthcenter
    float radius  = v3EarthCenter.y-20.0*0.1;
    float z_dot_v = dot(ZenithDistance, ViewRay);//rmu?
    float z_dot_z = dot(ZenithDistance, ZenithDistance);
    float ray_earth_center_squared_distance = z_dot_z - z_dot_v * z_dot_v;
    float distance_to_intersection = -z_dot_v - sqrt(radius *  radius - ray_earth_center_squared_distance);

    float EarthAlpha = distance_to_intersection>0.0?1.0:0.0;

    //recheck fog
    if(EarthAlpha>0.5)
    {
        vec3 EarthWorldPos = v3EyePos+ ViewRay*distance_to_intersection;
        WorldSpacePosition.xyz = mix(EarthWorldPos,WorldSpacePosition.xyz,SpecRoughAlphaDither.b);
        LinearEyeDepth = length(WorldSpacePosition.xyz-v3EyePos);

        vec3 sky_irradiance;
        vec3 sun_global_irradiance;
        vec3 sun_irradiance = GetSunAndSkyIrradianceFog(WorldSpacePosition.xyz - v3EarthCenter, vec3(0.0,1.0,0.0), L, sky_irradiance,sun_global_irradiance);//without N.L

       // FogSkyIrradiance =sky_irradiance*0.156;
       // FogSunIrradiance =sun_global_irradiance;
    }

    if(SpecRoughAlphaDither.b>0.0)
    {
	    vec4 WorldSpaceNormal = texture(Sampler2D_WorldSpaceNormal,TexCoord);
	    vec4 AlbedoAO = texture(Sampler2D_AlbedoAO,TexCoord);
        DefferedVelocity = texture(Sampler2D_DefferedVelocity,TexCoord).xy;
        vec3 finalAlbedo = pow(AlbedoAO.rgb,vec3(2.2));//Linearizing from gamma space

        vec4 LightSpacePos = m4LightViewProjX* WorldSpacePosition;
       LightSpacePos/=LightSpacePos.w;
       LightSpacePos.xy*=8.0;
       LightSpacePos.x*=0.567;
       LightSpacePos.xy= LightSpacePos.xy*0.5+0.5;

        float near = 10.0;
        float far = near+20000.0;

        float LinearEyeDepth = linearize_depthVulkan(LightSpacePos.z,near,far);
        vec3 BeerShadowMap = texture(Sampler2D_CloudShadowMap,LightSpacePos.xy).rgb;

        float OpticalDepth = min(BeerShadowMap.b,BeerShadowMap.g*max(0.0,LinearEyeDepth-BeerShadowMap.r));
        float CloudTransmittance = exp(-OpticalDepth*10.0);

       // CloudTransmittance = LinearEyeDepth>BeerShadowMap.r?0.0:1.0;

      //  if(LightSpacePos.x>1.0||LightSpacePos.y>1.0||LightSpacePos.x<0.0||LightSpacePos.y<0.0)
        {
        //    CloudTransmittance=0.0;
        }

      //GOD_RAY Last edit


        vec3 GodStartPos = WorldSpacePosition.xyz;
        vec3 GodEndPos = v3EyePos;
        vec4 CurrWorldPos;
        vec3 ViewRayToCam = normalize(v3EyePos- GodStartPos);
        vec3 path = (GodEndPos-GodStartPos);
        float len = length(path);
        const uint nSteps = 400;//256 offline
        float deltaStepLength = len / nSteps;

        GodStartPos = GodStartPos+ViewRayToCam*(BlueNoise.r*2.0-1.0)*5.0;

        for (int i=1;i<nSteps;i++)
        {
           CurrWorldPos.xyz = (GodStartPos + ViewRayToCam*deltaStepLength*(i));//*fFactor

           CurrWorldPos = (m4LightViewProjX* vec4(CurrWorldPos.xyz,1.0));
           CurrWorldPos/=CurrWorldPos.w;
           CurrWorldPos.xy*=8.0;
           CurrWorldPos.x*=0.567;
           CurrWorldPos.xy= CurrWorldPos.xy*0.5+0.5;

           float LinearEyeDepth = linearize_depthVulkan(CurrWorldPos.z,near,far);
           vec3 BeerShadowMap = texture(Sampler2D_CloudShadowMap,CurrWorldPos.xy).rgb;

           float OpticalDepth = min(BeerShadowMap.b,BeerShadowMap.g*max(0.0,LinearEyeDepth-BeerShadowMap.r));
           float CloudTransmittance = exp(-OpticalDepth*2.0);

             if(CurrWorldPos.x>1.0||CurrWorldPos.y>1.0||CurrWorldPos.x<0.0||CurrWorldPos.y<0.0)
            {
               CloudTransmittance=0.0;
            }

           GodRayIntensity+=(0.004/float(nSteps))*CloudTransmittance;

        }
     //  float OpticalDepth = min(BeerShadowMap.b,BeerShadowMap.g*max(0.0,LinearEyeDepth-BeerShadowMap.r));
     //   float CloudTransmittance = exp(-OpticalDepth);

       // vec3 finalAlbedo = pow(AlbedoAO.rgb,vec3(1.0));// gamma space
	    float finalRoughness = SpecRoughAlphaDither.g;
	    float finalAO = AlbedoAO.a;
        float finalSpec = SpecRoughAlphaDither.r;
	    vec3 finalMetalness = vec3(0.0);

        //Normalized
        vec3 N = normalize(WorldSpaceNormal.xyz);
        //PBR Dot products
	    float NdotL = max(dot(N, L), 0.00001);
	    float NdotV = max(dot(N, V), 0.00001);
	    float NdotH = max(dot(N, H), 0.00001); 

        F0 = mix(Fdielectric, finalAlbedo, finalMetalness);

        finalRoughness = mix(finalRoughness,0.0,finalSpec);
        F0 = mix(Fdielectric,vec3(0.02),finalSpec);//water is 2% & dielectric is approx 4%

        // Calculate Fresnel term for direct lighting. 
	    vec3 F  = fresnelSchlick(F0, max(0.00001, dot( H, V)));
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
	    vec3 diffuseBRDF = kd * finalAlbedo ;

	    // Cook-Torrance specular microfacet BRDF.
	    vec3 specularBRDF = (F * D * G) / (4.0 * NdotL * NdotV);

       // vec3  Terrain_radiance = finalAlbedo * (1.0 / PI) * (sun_irradiance * GetSunVisibility(WorldSpacePosition.xyz, L) +sky_irradiance * GetSkyVisibility(WorldSpacePosition.xyz));
	    // Total contribution for this light.
	    //directLighting = (diffuseBRDF + specularBRDF) * radiance * NdotL; //for shadow->//* max(CalcShadow(),0.30)
	    vec3 PBR = (diffuseBRDF + specularBRDF); //for shadow->//* max(CalcShadow(),0.30)

       //PBR irradiance
        vec3 sky_irradiance;
        vec3 sun_global_irradiance;
        vec3 sun_irradiance = GetSunAndSkyIrradianceFog(WorldSpacePosition.xyz - v3EarthCenter, N, L, sky_irradiance,sun_global_irradiance);
       // vec3 TerrainRad = ((diffuseBRDF + specularBRDF)* (sun_irradiance * GetSunVisibility(WorldSpacePosition.xyz, L)))+sky_irradiance * GetSkyVisibility(WorldSpacePosition.xyz);
        RadiancePBR = PBR * sun_irradiance *CloudTransmittance +sky_irradiance *pow(finalAO*AmbientIntensity,2.0)*diffuseBRDF;

        FogSkyIrradiance =sky_irradiance*0.156;
        FogSunIrradiance =sun_global_irradiance;
     //   vec3 transmittance;
    //    vec3 in_scatter = GetSkyRadianceToPoint(v3EyePos - v3EarthCenter, WorldSpacePosition.xyz - v3EarthCenter, 0.0, L, transmittance);
    //    RadiancePBR = RadiancePBR * transmittance + in_scatter;

         LinearEyeDepth = length(WorldSpacePosition.xyz-v3EyePos);
        float LinearDepth = (LinearEyeDepth-0.01)/(200.0-0.01);

       vec3 transmittanceLUT = texture(Sampler3D_FroxelTransmittance,vec3(TexCoord,LinearDepth)).rgb;
       vec3 inScatterLUT = texture(Sampler3D_FroxelInScatter,vec3(TexCoord,LinearDepth)).rgb;
       // RadiancePBR = max(vec3(0.0),vec3( inScatterLUT));

    //   RadiancePBR = mix(RadiancePBR * transmittance + in_scatter,RadiancePBR* transmittanceLUT + inScatterLUT,fArtistFactor);
       RadiancePBR = RadiancePBR +GodRayIntensity *(1.0-clamp(fArtistFactor*4.0,0.0,1.0));
       RadiancePBR = max(vec3(0.0),vec3(RadiancePBR));
    }
    else
    {
              //GOD_RAY Last edit

        vec3 GodStartPos =v3EyePos;
        vec3 GodEndPos = v3EyePos+ViewRay*1000.0;
        vec4 CurrWorldPos;
     //   vec3 ViewRayToCam = normalize(v3EyePos- GodStartPos);
        vec3 path = (GodEndPos-GodStartPos);
        float len = length(path);
        const uint nSteps = 400;//256 offline
        float deltaStepLength = len / nSteps;

        GodStartPos = GodStartPos+ViewRay*(BlueNoise.r*2.0-1.0)*10.0*0.432;

        float near = 10.0;
        float far = near+20000.0;

        for (int i=1;i<nSteps;i++)
        {
           CurrWorldPos.xyz = (GodStartPos + ViewRay*deltaStepLength*(i));//*fFactor

           CurrWorldPos = (m4LightViewProjX* vec4(CurrWorldPos.xyz,1.0));
           CurrWorldPos/=CurrWorldPos.w;
           CurrWorldPos.xy*=8.0;
           CurrWorldPos.x*=0.567;
           CurrWorldPos.xy= CurrWorldPos.xy*0.5+0.5;

           float LinearEyeDepth = linearize_depthVulkan(CurrWorldPos.z,near,far);
           vec3 BeerShadowMap = texture(Sampler2D_CloudShadowMap,CurrWorldPos.xy).rgb;

            if(CurrWorldPos.x>1.0||CurrWorldPos.y>1.0||CurrWorldPos.x<0.0||CurrWorldPos.y<0.0)
            {
               BeerShadowMap.r=0.0;
            }

           float OpticalDepth = min(BeerShadowMap.b,BeerShadowMap.g*max(0.0,LinearEyeDepth-BeerShadowMap.r));
           float CloudTransmittance = exp(-OpticalDepth*1.0);




           GodRayIntensity+=(0.004/float(nSteps))*CloudTransmittance;

        }


         RadiancePBR = max(vec3(0.0),vec3(GodRayIntensity*(1.0-clamp(fArtistFactor*4.0,0.0,1.0))));
    }
   
    vec3 transmittanceSky;
    vec3 SkyRadiance = GetSkyRadiance(v3EyePos - v3EarthCenter, ViewRay, 0.0, L, transmittanceSky);//+BlueNoise*100.0*fFactor

      //Sun Disk with edgeDarkening
  //vec3 Nec96 = Nec96(sun_size.y,LdotV);
    vec3 HM98 = HM98(v2SunSize.y,toSunViewCosTheta);
    vec3 finalSunDarkFactor = HM98;

    //HeightFog
    vec3 outInscatter,outTransmittance;
    HeightFog( LinearEyeDepth,length(v3EyePos-v3EarthCenter)+v3EarthCenter.y ,length(WorldSpacePosition.xyz-v3EarthCenter)+v3EarthCenter.y, toSunViewCosTheta,outInscatter,outTransmittance,FogSkyIrradiance,FogSunIrradiance);

 vec3  RadianceFinal = vec3(RadiancePBR);
// vec3  RadianceFinal = vec3(RadiancePBR*outTransmittance+outInscatter);
//   RadianceFinal = vec3(sunDisk);
 //POST PROCESS ---------------------------------------------------------------------------------------------------------------------------------
                            //Dither
  //  FragColor_frame.rgb=radianceFinal+BlueNoise*fFactor*0.004;
                          //ToneMapping
    //  FragColor_frame = vec4(vec3(1.0) - exp(-vec3(radianceFinal)  * 10),1.0);
    FragColor_frame.rgb = RadianceFinal;

 //   FragColor_frameTAA.rgb = RadiancePBR;
    
                           //Sun Glow
  //  float glowLarge =  clamp(1.0-(1.0 - toSunViewCosTheta)/ ((1.0- 0.9)*0.1),0.0,1.0);
  //  float glowSmall =  clamp(1.0-(1.0 - toSunViewCosTheta)/ ((1.0- 0.978)*0.02),0.0,1.0);
  //  float FinalGlow = pow(glowSmall*glowSmall*glowSmall*4.0+glowLarge*0.035,2.2);

  //  float SunDotZenith = max(dot(normalize(v3EyePos-v3EarthCenter),L),0.0);

   // FragColor_frame.rgb += FinalGlow*smoothstep(0.1,0.173,SunDotZenith)*(1.0-SpecRoughAlphaDither.b);
   
      FragColor_frame.rgb =vec3(RadianceFinal);

      float fadeOut = 1.0-clamp((fArtistFactor-0.75)*4.0,0.0,1.0);

    FragColor_frame.rgb = unreal(FragColor_frame.rgb * fExposure);
    FragColor_frame.a = SpecRoughAlphaDither.b;

   //Test
 //  FragColor_frame.rgb = Clouds.rgb;
                        //Gamma Correction
//  FragColor_frame.rgb=RadianceFinal;
  FragColor_frame.rgb =pow(FragColor_frame.rgb,vec3(0.454545))*fadeOut;
// FragColor_frame.rgb =mix(FragColor_frame.rgb,Clouds.rgb,Clouds.a);
 // FragColor_frame.rgb =texture(Sampler3D_FroxelTransmittance,vec3(TexCoord,0.0)).rgb;
 //  FragColor_frame.rgb =vec3(GodRayIntensity);

                         //CONTRAST
   // FragColor_frame.rgb = adjustContrast(FragColor_frame.rgb,fFactor);

                              //HSB
 //  FragColor_frame.rgb = rgb2hsb(FragColor_frame.rgb);
 //   FragColor_frame.g*=1.2;//Saturation
 //  FragColor_frame.b*=2.0*fFactor;//Brightness
 //  FragColor_frame.rgb = hsb2rgb(FragColor_frame.rgb);

 // FragColor_frameTAA.rgb =pow(FragColor_frameTAA.rgb,vec3(0.454545));
                              //SunGlare
//  vec3 SunGlare = texture(Sampler2D_SunGlare,TexCoord).rgb+ (smoothstep(0.6,1.0,toSunViewCosTheta+BlueNoise.r*0.02)*0.069);
 // FragColor_frame.rgb +=SunGlare;
 // FragColor_frameTAA.rgb +=SunGlare;
                                //Alpha
 // FragColor_frame.a = EarthAlpha;
  //FragColor_frameTAA.a = SpecRoughAlphaDither.b;

                           //Test
 // FragColor_frame.rgb =vec3(EarthAlpha);

 float HeightFactorForCloudMask = (length(v3EyePos-vec3(0.0,-63600.0,0.0))>63800.0)?1.0:(1.0-EarthAlpha);

 CloudHorizonAlpha_frame = vec4(HeightFactorForCloudMask,0.0,0.0,0.0);
  //CameraVelocity
  vec4 PrevWorldPos = m4PrevViewProjX*(v4FarWorldPos/v4FarWorldPos.w);
  PrevWorldPos/=PrevWorldPos.w;
  PrevWorldPos.xy=PrevWorldPos.xy*0.5+0.5;


  vec2 CameraVelocity = TexCoord - PrevWorldPos.xy;
   CameraVelocity = mix(CameraVelocity,DefferedVelocity,SpecRoughAlphaDither.b);
   //CameraVelocity = DefferedVelocity;

  imageStore(CameraVelocityImage, ivec2(gl_FragCoord.xy), vec4(CameraVelocity, 0.0, 1.0));
}
