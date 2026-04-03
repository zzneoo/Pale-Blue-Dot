#version 460 core

const float PI = 3.14159265359;
const vec3 Fdielectric = vec3(0.04);
const float kLengthUnitInMeters = 100.0;
const float AtmoTopRadius = 66200.0;

layout (location = 0 )out vec4 FragColor_frame;

uniform layout(rg32f, binding = 0) writeonly image2D CameraVelocityImage;

uniform float fFactor;
uniform float fTime;
uniform float fExposure;

uniform vec3 v3SunPos;
uniform vec3 v3EyePos;
uniform vec3 v3ColorPicker;
uniform vec3 v3WaterTint;
uniform vec3 v3EarthCenter;

uniform vec2 v2SunSize;
uniform vec2 v2CloudScaleVisibility;

uniform mat4 m4EarthRotationX;
uniform mat4 m4PrevViewProjX;
uniform mat4 m4EarthCloudRotationX;
uniform mat4 m4LightViewProjX;

uniform sampler2D Sampler2D_WorldSpacePosition;
uniform sampler2D Sampler2D_WorldSpaceNormal;
uniform sampler2D Sampler2D_AlbedoAO;
uniform sampler2D Sampler2D_SpecRoughAlphaDither;
uniform sampler2D Sampler2D_DefferedVelocity;

uniform sampler2D Sampler2D_EarthAlbedoSpec;//Water in alpha
uniform sampler2D Sampler2D_EarthNormal;
uniform sampler2D Sampler2D_EarthClouds;

uniform sampler2D Sampler2D_EarthWaterNormal;
uniform sampler2D Sampler2D_EarthLodBlendMap;//same as AlphaDither
uniform sampler2D Sampler2D_CloudShadowMap;

//Special
uniform sampler2D Sampler2D_WhirlNoise;
uniform sampler2D Sampler2D_Clouds;
uniform sampler2D Sampler2D_WhirlTexture;
uniform sampler2D Sampler2D_SunGlare;
uniform sampler2D Sampler2D_BlueNoise;

in vec2 TexCoord;
in vec3 v3ViewRay;
in vec4 v4FarWorldPos;

const vec3 kSphereCenter = vec3(0.0, 1000.0, 0.0) / kLengthUnitInMeters;
const float kSphereRadius = 1000.0 / kLengthUnitInMeters;
const vec3 kSphereAlbedo = vec3(0.8);
const vec3 kGroundAlbedo = vec3(0.0, 0.0, 0.04);

vec3 GetSolarRadiance();
vec3 GetSkyRadiance(vec3 camera, vec3 view_ray, float shadow_length,
    vec3 sun_direction, out vec3 transmittance);
vec3 GetSkyRadianceToPoint(vec3 camera, vec3 point, float shadow_length,
    vec3 sun_direction, out vec3 transmittance);
vec3 GetSunAndSkyIrradiance(
    vec3 p, vec3 normal, vec3 sun_direction, out vec3 sky_irradiance);

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
vec3 Uncharted2ToneMapping(vec3 color)
{
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	float W = 11.2;
	float exposure = 20.0;
	color *= exposure;
	color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
	float white = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
	color /= white;
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

void main(void)
{
    vec4 WorldSpacePosition = texture(Sampler2D_WorldSpacePosition,TexCoord);  
    vec4 SpecRoughAlphaDither = texture(Sampler2D_SpecRoughAlphaDither,TexCoord);

	vec3 ViewRay = normalize(v3ViewRay);	
    vec4 EarthWorldPos=v4FarWorldPos;
    vec3 GodRayWorldPosSpace=v4FarWorldPos.xyz;
    vec2 DefferedVelocity=vec2(0.0);

    vec3 RadiancePBR;
    vec3 F0 = Fdielectric;
     //PBR vectors----------------------------------------------------------------------------------------------------
	vec3 L = normalize(v3SunPos);//towards light
	vec3 V = normalize(-ViewRay);//towards camera(ViewRay is Camera to object ray)
	vec3 H = normalize(V+L);//  N+V or N+L

    float toSunViewCosTheta = max(dot(L, ViewRay), 0.0); 

    float AmbientIntensity = 1.7;

                                       //BlueNoise
    vec3 BlueNoise =texture(Sampler2D_BlueNoise,gl_FragCoord.xy/256.0).rgb*2.0-1.0;

    vec4 Clouds  = texture(Sampler2D_Clouds,TexCoord);

    //Intersections   
    //RaySphere intersection for Earth
    vec3 ZenithDistance = v3EyePos - v3EarthCenter;//or CameraPos relative to earthcenter
    float z_dot_v = dot(ZenithDistance, ViewRay);//rmu?
    float z_dot_z = dot(ZenithDistance, ZenithDistance);
    float ray_earth_center_squared_distance = z_dot_z - z_dot_v * z_dot_v;
    float distance_to_intersection = -z_dot_v - sqrt( v3EarthCenter.y * v3EarthCenter.y - ray_earth_center_squared_distance);

        //RaySphere intersection for Clouds
    float CloudRadius =300.0-v3EarthCenter.y;
    float distance_to_CloudIntersection = -z_dot_v - sqrt( CloudRadius * CloudRadius - ray_earth_center_squared_distance);

    float distance_to_AtmoTopIntersection = -z_dot_v - sqrt( AtmoTopRadius * AtmoTopRadius - ray_earth_center_squared_distance);

    float distance_to_moonIntersection;
    float moonRadius = 17374.0;//100 meter per unit
    {
         vec3 ZenithDistance = v3EyePos - (vec3(0.0,-moonRadius,-1500000) + 3844000*0.7f);//or CameraPos relative to earthcenter
         float z_dot_v = dot(ZenithDistance, ViewRay);//rmu?
         float z_dot_z = dot(ZenithDistance, ZenithDistance);
         float ray_moon_center_squared_distance = z_dot_z - z_dot_v * z_dot_v;
          distance_to_moonIntersection = -z_dot_v - sqrt( moonRadius * moonRadius - ray_moon_center_squared_distance);
    }

   //GodRay Space Pos
    vec3 EarthPosForSpace = v3EyePos + ViewRay * distance_to_intersection;
    float EarthAlpha = distance_to_intersection>0.0?1.0:0.0;
    float MoonAlpha = distance_to_moonIntersection>0.0?1.0:0.0;
    float WorldAlpha = SpecRoughAlphaDither.b>0.0?1.0:0.0;
    GodRayWorldPosSpace = mix(GodRayWorldPosSpace.xyz,EarthPosForSpace,EarthAlpha);
    GodRayWorldPosSpace = mix(GodRayWorldPosSpace,WorldSpacePosition.xyz,WorldAlpha);
 //   GodRayWorldPos = m4LightViewProjX *GodRayWorldPos;

    vec4 CurrWorldPos;
    vec3 GodStartPos;
    vec3 GodEndPos ;
    vec3 ViewRayToCam;

    if(length(v3EyePos-v3EarthCenter)>AtmoTopRadius)
    {
        GodStartPos = GodRayWorldPosSpace;
        ViewRayToCam = normalize(v3EyePos- GodStartPos);
         GodEndPos = IntersectSphereFromInside(GodStartPos,ViewRayToCam,AtmoTopRadius);
    }
    else
    {
        // GodStartPos = IntersectSphereFromInside(v3EyePos,ViewRay,AtmoTopRadius);
         GodStartPos = GodRayWorldPosSpace;
          ViewRayToCam = normalize(v3EyePos- GodStartPos);
         GodEndPos = v3EyePos;
    }


    vec3 path = (GodEndPos-GodStartPos);
    float len = length(path);
    const uint nSteps = 0;//256 offline
    float deltaStepLength = len / nSteps;
    float GodRayIntensity = 0.0;

    for (int i=1;i<nSteps;i++)
    {

         CurrWorldPos.xyz = (GodStartPos + ViewRayToCam*deltaStepLength*(i));//*fFactor

         CurrWorldPos = (m4LightViewProjX* vec4(CurrWorldPos.xyz,1.0));
         CurrWorldPos/=CurrWorldPos.w;
         CurrWorldPos.xy= CurrWorldPos.xy*0.5+0.5;
         float near = 10.0;
        float far = near+100000.0;

         float LinearEyeDepth = linearize_depthVulkan(CurrWorldPos.z,near,far);
         vec3 BeerShadowMap = texture(Sampler2D_CloudShadowMap,CurrWorldPos.xy).rgb;

         float OpticalDepth = min(BeerShadowMap.b,BeerShadowMap.g*max(0.0,LinearEyeDepth-BeerShadowMap.r));
         float CloudTransmittance = exp(-OpticalDepth*2);

         float depth = BeerShadowMap.r;
       

       //  if(depth>CurrWorldPos.z || abs(CurrWorldPos.x-0.5)>0.5 ||abs(CurrWorldPos.y-0.5)>0.5)
       //  if( abs(CurrWorldPos.x-0.5)>0.5 ||abs(CurrWorldPos.y-0.5)>0.5)
         {
          //  GodRayIntensity= mix(GodRayIntensity,(depth<CurrWorldPos.z)?0.0:1.0,1.0/(i+1));
          // GodRayIntensity+= scatter(toSunViewCosTheta)*0.2;
          GodRayIntensity+=(1/float(nSteps))*CloudTransmittance;
         }

    }

    GodRayIntensity = 1.0;

    //CloudShadow2D
        float CloudShadow2D = 1.0;
        if (distance_to_intersection > 0.0) 
        {
         vec3 point = v3EyePos + ViewRay * distance_to_intersection;
         vec3 N = normalize(point - v3EarthCenter); 
         //cloudShadow2D
         vec4 NormalForCloudShadow = normalize(m4EarthCloudRotationX * vec4(N,0.0));
         vec2 UVForCloudShadow;
         UVForCloudShadow.s = 0.5 + atan(-NormalForCloudShadow.x,NormalForCloudShadow.z)/(2.0*PI);
         UVForCloudShadow.t = 0.5 - asin(NormalForCloudShadow.y)/PI;

         //CloudShadow
    //     UVForCloudShadow.t*=(0.9+0.1*0.964);
     //    UVForCloudShadow.s*=(1.0+0.1*0.04);
          CloudShadow2D = clamp((1.0-texture(Sampler2D_EarthClouds,UVForCloudShadow*v2CloudScaleVisibility.x).r*2.0),0.0,1.0);
        }

    if(SpecRoughAlphaDither.b>0.0)
    {
	    vec4 WorldSpaceNormal = texture(Sampler2D_WorldSpaceNormal,TexCoord);//roughness in Alpha channel
	    vec4 AlbedoAO = texture(Sampler2D_AlbedoAO,TexCoord);
        DefferedVelocity = texture(Sampler2D_DefferedVelocity,TexCoord).xy;
        vec3 finalAlbedo = pow(AlbedoAO.rgb,vec3(2.2));//Linearizing from gamma space

        vec4 LightSpacePos = m4LightViewProjX* WorldSpacePosition;
        LightSpacePos/=LightSpacePos.w;
        LightSpacePos.xy= LightSpacePos.xy*0.5+0.5;

        float near = 10.0;
        float far = near+100000.0;

        float LinearEyeDepth = linearize_depthVulkan(LightSpacePos.z,near,far);
        vec3 BeerShadowMap = texture(Sampler2D_CloudShadowMap,LightSpacePos.xy).rgb;

        float OpticalDepth = min(BeerShadowMap.b,BeerShadowMap.g*max(0.0,LinearEyeDepth-BeerShadowMap.r));
        float CloudTransmittance = exp(-OpticalDepth);

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

        finalRoughness = mix(1.0,finalRoughness,finalSpec);
        F0 = mix(Fdielectric,vec3(0.117),finalSpec);//water is 2% & dielectric is approx 4%

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

       //PBR radiance
        vec3 sky_irradiance;
        vec3 sun_irradiance = GetSunAndSkyIrradiance(WorldSpacePosition.xyz - v3EarthCenter, N, L, sky_irradiance);
       // vec3 TerrainRad = ((diffuseBRDF + specularBRDF)* (sun_irradiance * GetSunVisibility(WorldSpacePosition.xyz, L)))+sky_irradiance * GetSkyVisibility(WorldSpacePosition.xyz);
        RadiancePBR = PBR * sun_irradiance*CloudTransmittance*mix(1.0,CloudShadow2D,smoothstep(0.4,0.6,1.0))  +sky_irradiance *finalAO*AmbientIntensity*diffuseBRDF;//*CloudShadow2D

        vec3 transmittance;
        vec3 in_scatter = GetSkyRadianceToPoint(v3EyePos - v3EarthCenter, WorldSpacePosition.xyz - v3EarthCenter-vec3(0.0,75.0*(1.0-v2CloudScaleVisibility.y),0.0), 0.0, L, transmittance);
        RadiancePBR = RadiancePBR * transmittance + in_scatter*clamp(pow(GodRayIntensity,mix(20.0,12.9,1.0))*1.43,0.0,1.0);

        RadiancePBR = max(vec3(0.0),vec3(RadiancePBR));
     //   RadiancePBR = max(vec3(0.0),vec3(finalAlbedo));
    }


    // Compute the radiance reflected by the ground, if the ray intersects it.
    float EarthCloudAlpha = 0.0;
    vec3 EarthRadiance = vec3(0.0);
    float EarthClouds;
    vec3 EarthCloudLight;
    vec2 UV;
    float CamToWorldOriginLength=0.0;

    //EarthClouds
    if(distance_to_CloudIntersection>0.0)
    {
        vec3 point = v3EyePos + ViewRay * distance_to_CloudIntersection;
        vec3 N = normalize(point - v3EarthCenter); 
       vec4 tempNormal = normalize(m4EarthCloudRotationX * vec4(N,0.0));
         UV.s = 0.5 + atan(-tempNormal.x,tempNormal.z)/(2.0*PI);
         UV.t = 0.5 - asin(tempNormal.y)/PI;

       float whirl1Scale =0.336831;
       vec2 PosWhirlNoise_UV =  GetLOD_UV(whirl1Scale,UV,vec2(-1.664,8.74),vec2(0.666,1.0));//
       vec3 WhirlNoise = texture(Sampler2D_WhirlNoise,PosWhirlNoise_UV).rgb;
        vec2 FractUV = fract(PosWhirlNoise_UV);
        vec2 WholeUV = PosWhirlNoise_UV-FractUV;

        vec2 RotatedUV = WholeUV+WhirlNoise.st;
       float sin_factor = sin(fTime*1.52);
       float cos_factor = cos(fTime*1.52);
       RotatedUV = vec2(RotatedUV.x - 0.5 , RotatedUV.y - 0.5) * mat2(cos_factor, sin_factor, -sin_factor, cos_factor);
       RotatedUV+=0.5;

       float Whirl1 = texture(Sampler2D_WhirlTexture,RotatedUV*0.307).r;

         EarthClouds = BiCubicCatmullRom5Tap(Sampler2D_EarthClouds,vec2(UV*v2CloudScaleVisibility.x),vec2(20000,10000)).r;
         EarthClouds = mix(EarthClouds,Whirl1+0.1,WhirlNoise.b*0.8)*clamp(pow(GodRayIntensity,20.0)*1.43,0.0,1.0);

         EarthCloudAlpha =1.0;
    }

    vec3 camera = v3EyePos-v3EarthCenter;
    float r = length(camera);
    float rmu = dot(camera, ViewRay);
    float distance_to_top_atmosphere_boundary = -rmu - sqrt(rmu * rmu - r * r + 66200.0 * 66200.0);

     
      float AtmoStep;
      vec4 AtmoWorldPos = vec4(IntersectSphereMidFromOutside(v3EyePos,ViewRay,66200.0,AtmoStep),1.0);//63600
    if(AtmoStep>0.0)
    {
        EarthWorldPos=AtmoWorldPos;
    }

    if (distance_to_intersection > 0.0) 
    {
        vec3 point = v3EyePos + ViewRay * distance_to_intersection;
        EarthWorldPos =vec4(point,1.0);
         CamToWorldOriginLength = clamp((length(v3EyePos-point)/30000.0),0.0,1.0);

        vec3 N = normalize(point - v3EarthCenter); 
         vec4 tempNormal = normalize(m4EarthRotationX * vec4(N,0.0));
         UV.s = 0.5 + atan(-tempNormal.x,tempNormal.z)/(2.0*PI);
         UV.t = 0.5 - asin(tempNormal.y)/PI;

         //cloudShadow2D
         vec4 NormalForCloudShadow = normalize(m4EarthCloudRotationX * vec4(N,0.0));
         vec2 UVForCloudShadow;
         UVForCloudShadow.s = 0.5 + atan(-NormalForCloudShadow.x,NormalForCloudShadow.z)/(2.0*PI);
         UVForCloudShadow.t = 0.5 - asin(NormalForCloudShadow.y)/PI;

         //CloudShadow
       //  float CloudShadow = (1.0-texture(Sampler2D_EarthClouds,UVForCloudShadow).r)*fFactor;
         //EarthAlbedo,Normal
         vec4 EarthAlbedoSpec = texture(Sampler2D_EarthAlbedoSpec,UV);//dont change these uvs
         EarthAlbedoSpec.rgb = mix( EarthAlbedoSpec.rgb, EarthAlbedoSpec.rgb*v3WaterTint, EarthAlbedoSpec.a);
       //  vec3 EarthNormal = normalize(texture(Sampler2D_EarthNormal,UV).rgb*2.0-1.0);//dont change these uvs
         vec3 EarthNormal = normalize(BiCubicCatmullRom5Tap(Sampler2D_EarthNormal,UV,vec2(16384.0,8192.0)).rgb*2.0-1.0);//dont change these uvs
         EarthNormal.rg*=1.6; //boost normalmap
         EarthNormal=normalize(EarthNormal);
        vec3 T = normalize( cross( N,vec3(0,1,0)) );  
       // T = normalize(T - dot(T, N) * N);
        vec3 B = cross( T,N );
       // T = cross( B,N );
        mat3 TBN  = mat3( T, B, N );

       vec2 WaterUV=UV;
       float sin_factor = sin(fFactor*5);
       float cos_factor = cos(fFactor*5);
     //  WaterUV = vec2(WaterUV - 0.5) * mat2(cos_factor, sin_factor, -sin_factor, cos_factor);
     //  WaterUV+=0.5;
       WaterUV*=100.0;

         vec3 EarthWaterNormal = texture(Sampler2D_EarthWaterNormal,(WaterUV)-fTime*0.470).rgb;
         vec3 EarthWaterNormal1 = texture(Sampler2D_EarthWaterNormal,(WaterUV)+fTime*0.420).rgb;
         vec3 EarthWaterNormal2 = texture(Sampler2D_EarthWaterNormal,(WaterUV*20)+fTime*0.5).rgb;
         vec3 EarthWaterNormal3 = texture(Sampler2D_EarthWaterNormal,(WaterUV*20)-fTime*0.5).rgb;

         EarthWaterNormal.rgb=EarthWaterNormal.rbg;
         EarthWaterNormal1.rgb=EarthWaterNormal1.rbg;

         EarthWaterNormal=normalize(EarthWaterNormal*2-1);
         EarthWaterNormal1=normalize(EarthWaterNormal1*2-1);
         EarthWaterNormal =normalize(EarthWaterNormal+EarthWaterNormal1);
         EarthWaterNormal.b=0.978;
         EarthWaterNormal.rg*=0.2;

     //    EarthNormal=(TBN*mix(EarthNormal,EarthWaterNormal,EarthAlbedoSpec.a));//EarthAlbedoSpec.a //uncomment for water
         EarthNormal=TBN*EarthNormal;

      //   EarthWaterNormal = EarthWaterNormal.rgb;
      //   EarthWaterNormal2 =normalize(EarthWaterNormal2+EarthWaterNormal3);

       //  EarthWaterNormal =2.0* mix(EarthWaterNormal,EarthWaterNormal2,1.0-CamToWorldOriginLength)-1.0;
       //  EarthWaterNormal = TBN *vec3(EarthWaterNormal);

       //  EarthWaterNormal = mix(EarthWaterNormal,N,smoothstep(0.4,1.0,CamToWorldOriginLength));

         float NdotV_Earth = max(0.00001, dot( EarthNormal, V));
         float NdotL_Earth = max(0.00001, dot( EarthNormal, L));
         float NdotH_Earth = max(0.00001, dot( EarthNormal, H));
         float HdotV_Earth = max(0.00001, dot( V, H));
         //specular
         float Rough =mix(1.0,0.470,EarthAlbedoSpec.a);
         float metalness = 0.0;

         F0 = mix(Fdielectric,vec3(0.117),EarthAlbedoSpec.a);//water is 2% & dielectric is approx 4%

         vec3 Fresnel  = fresnelSchlick(F0, (HdotV_Earth));//verify this
         float Distribution = ndfGGX(NdotH_Earth, Rough);
	     float GGX = gaSchlickGGX(NdotL_Earth, NdotV_Earth, Rough);

         //Offset algorithm for scale = (scale*0.5)+0.5-(scale)
        // float scale;
        // scale = 0.0822555;//0.026 for LOD2
        // vec2 EarthLOD4_UV =  GetLOD_UV(scale,vec2(u,v),vec2(-2.764105344,-2.167995456),vec2(1.0,1.0));//
        // vec4 EarthLOD4 = texture(Sampler2D_EarthLOD4,EarthLOD4_UV);//+vec2(-5.0*fFactor,5.0)

        vec3 kd = mix(vec3(1.0) - Fresnel, vec3(0.0), 0.0);//metalness=0

         //CLoudShadowEarth
         vec4 LightSpacePos = m4LightViewProjX* EarthWorldPos;
        LightSpacePos/=LightSpacePos.w;
        LightSpacePos.xy= LightSpacePos.xy*0.5+0.5;

        float near = 10.0;
        float far = near+100000.0;

        float LinearEyeDepth = linearize_depthVulkan(LightSpacePos.z,near,far);
        vec3 BeerShadowMap = texture(Sampler2D_CloudShadowMap,LightSpacePos.xy).rgb;

        float OpticalDepth = min(BeerShadowMap.b,BeerShadowMap.g*max(0.0,LinearEyeDepth-BeerShadowMap.r));
        float CloudTransmittance = exp(-OpticalDepth*10);


        //Earth PBR
        vec3 EarthDiffuseBRDF =kd* pow(EarthAlbedoSpec.rgb,vec3(2.2)) ;
        vec3 EarthSpecularBRDF = ((Fresnel * Distribution * GGX) / ( 4.0 * NdotL_Earth * NdotV_Earth));//*(EarthAlbedo.a)
        // Compute the radiance reflected by the ground.
        vec3 sky_irradiance;
        vec3 sun_irradiance = GetSunAndSkyIrradiance( point - v3EarthCenter, EarthNormal, L, sky_irradiance); //this N is not from deffered normal
        EarthRadiance = (EarthDiffuseBRDF+EarthSpecularBRDF)*CloudTransmittance*sun_irradiance*mix(1.0,CloudShadow2D,smoothstep(0.4,0.6,1.0))  +max(sky_irradiance,vec3(0.0000))*AmbientIntensity*EarthDiffuseBRDF ;//skyIrrad * AO & SunIrrad * Shadow  //FarEarth 0.000012 //*CloudShadow2D 

        vec3 transmittance;
        vec3 in_scatter = GetSkyRadianceToPoint(v3EyePos - v3EarthCenter, point - v3EarthCenter, 0.0, L, transmittance);
        EarthRadiance = EarthRadiance * transmittance + in_scatter*GodRayIntensity;
       // EarthRadiance = (EarthDiffuseBRDF+EarthSpecularBRDF);

       EarthCloudLight=vec3(1.0)*max(dot(N,L),0.0)+sky_irradiance*AmbientIntensity;
    //    EarthRadiance = vec3(EarthSpecularBRDF);//test
    }

    vec3 transmittanceSky;
    vec3 SkyRadiance = GetSkyRadiance(v3EyePos - v3EarthCenter, ViewRay, 0.0, L, transmittanceSky);//+BlueNoise*100.0*fFactor

      //Sun Disk with edgeDarkening
  //vec3 Nec96 = Nec96(sun_size.y,LdotV);
    vec3 HM98 = HM98(v2SunSize.y,toSunViewCosTheta);
    vec3 finalSunDarkFactor = HM98;

    // If the view ray intersects the Sun, add the Sun radiance.
 //   if (toSunViewCosTheta > v2SunSize.y)
  //  {
  //      float sunBlendFactor =clamp(smoothstep(v2SunSize.y, v2SunSize.y + 0.0002808, toSunViewCosTheta) * 160,0.0,1.0);
 //       SkyRadiance = mix(SkyRadiance,max(((SkyRadiance + transmittanceSky * GetSolarRadiance())*finalSunDarkFactor*0.02),SkyRadiance),sunBlendFactor);
 //   }

    float EarthCloudVisibility = clamp(pow(EarthClouds,2.5),0.0,1.0);

    RadiancePBR = mix(EarthRadiance,RadiancePBR, SpecRoughAlphaDither.b);
    RadiancePBR = mix(RadiancePBR, EarthCloudLight, EarthCloudVisibility);//Earth and Sky
    RadiancePBR = mix(SkyRadiance, RadiancePBR, EarthAlpha+(1.0-EarthAlpha)*SpecRoughAlphaDither.b);//Earth and Sky

 vec3  RadianceFinal = vec3(RadiancePBR);//Earth and Sky
 //POST PROCESS ---------------------------------------------------------------------------------------------------------------------------------
                            //Dither
  //  FragColor_frame.rgb=radianceFinal+BlueNoise*fFactor*0.004;
                          //ToneMapping
    //  FragColor_frame = vec4(vec3(1.0) - exp(-vec3(radianceFinal)  * 10),1.0);
    FragColor_frame.rgb = RadianceFinal;
 //   FragColor_frameTAA.rgb = RadiancePBR;
 //  FragColor_frame.rgb +=(smoothstep(0.6,1.0,toSunViewCosTheta+BlueNoise.r*0.02)*0.094392)*0.01*0.139;//sun bloom
    FragColor_frame.rgb = unreal(FragColor_frame.rgb*fExposure*v3ColorPicker);

   //Test
 //  FragColor_frame.rgb = Clouds.rgb;
                        //Gamma Correction
//  FragColor_frame.rgb=RadianceFinal;
   if(distance_to_moonIntersection>0.0)
   {
       vec3 point = v3EyePos + ViewRay * distance_to_moonIntersection;
       vec3 MoonNormal = normalize(point- (vec3(0.0,-moonRadius,-1500000) + 3844000*0.7f));

       vec2 MoonUV;
       MoonUV.s = 0.5 + atan(MoonNormal.x,MoonNormal.z)/(2.0*PI);
       MoonUV.t = 0.5 - asin(MoonNormal.y)/PI;

       vec3 MoonAlbedo = pow(texture(Sampler2D_SunGlare,MoonUV).rgb,vec3(2.2));

       FragColor_frame.rgb = 8.7*MoonAlbedo*vec3(max(dot(MoonNormal,L),0.0047));
   }

  FragColor_frame.rgb =pow(FragColor_frame.rgb,vec3(0.454545));

// FragColor_frame.rgb =mix(FragColor_frame.rgb,Clouds.rgb,Clouds.a);
 // FragColor_frame.rgb =vec3(GodRayIntensity);

                         //CONTRAST
 //   FragColor_frame.rgb = adjustContrast(FragColor_frame.rgb,1.0);

                            //HSB
 //  FragColor_frame.rgb = rgb2hsb(FragColor_frame.rgb);
 //   FragColor_frame.g*=1.0+0.34;//Saturation
 //  FragColor_frame.b*=2.0*0.4;//Brightness
 //  FragColor_frame.rgb = hsb2rgb(FragColor_frame.rgb);

 // FragColor_frameTAA.rgb =pow(FragColor_frameTAA.rgb,vec3(0.454545));
                              //SunGlare
 // vec3 SunGlare = texture(Sampler2D_SunGlare,TexCoord).rgb+ ;
 // FragColor_frame.rgb +=SunGlare;
 // FragColor_frameTAA.rgb +=SunGlare;
                                //Alpha
  FragColor_frame.a = min(EarthAlpha+MoonAlpha,1.0);
  //FragColor_frameTAA.a = SpecRoughAlphaDither.b;

                           //Test
 // FragColor_frame.rgb =vec3(Clouds);
  //FragColor_frameTAA.rgb =vec3(RadiancePBR*1.5);


  //CameraVelocity
  vec4 PrevEarthWorldPos = m4PrevViewProjX*EarthWorldPos;
  PrevEarthWorldPos/=PrevEarthWorldPos.w;
  PrevEarthWorldPos.xy=PrevEarthWorldPos.xy*0.5+0.5;


  vec2 CameraVelocity = TexCoord - PrevEarthWorldPos.xy;
   CameraVelocity = mix(CameraVelocity,DefferedVelocity,SpecRoughAlphaDither.b);
   //CameraVelocity = DefferedVelocity;

  imageStore(CameraVelocityImage, ivec2(gl_FragCoord.xy), vec4(CameraVelocity, 0.0, 1.0));
}
