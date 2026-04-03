#version 460 core

out vec4 FragColor;
in vec2 TexCoord;

//extra
uniform vec3 v3SunPos;
uniform vec3 v3EyePos;
uniform vec3 v3EyeDir;

uniform float fVignette;

//matrices
uniform mat4 m4InvProjX;
uniform mat4 m4InvViewX;
uniform mat4 m4LightSpaceX;

//scene
uniform sampler2D tSampler2D_CloudTexture;
uniform sampler2D tSampler2D_ShadowMapTexture;
uniform sampler2D tSampler2D_DepthTexture;
uniform sampler2D tSampler2D_SkyTexture;
uniform sampler2D tSampler2D_SceneTexture;
uniform sampler2D tSampler2D_SceneMaskTexture;
//postProcess
uniform sampler2D tSampler2D_GodRaysTexture;

float gaussianKernel3x3[9]={0.077847,0.123317,0.077847,
							0.123317,0.195346,0.123317,
							0.077847,0.123317,0.077847 };
							
float gaussianKernel5x5[25]=

						{	0.003765,0.015019,0.023792,0.015019,0.003765,
							0.015019,0.059912,0.094907,0.059912,0.015019,
							0.023792,0.094907,0.150342,0.094907,0.023792,
							0.015019,0.059912,0.094907,0.059912,0.015019,
							0.003765,0.015019,0.023792,0.015019,0.003765 };

vec3 orangeColor = vec3(0.992156,0.721568,0.274509);
vec3 brightYellow = vec3(0.976470,0.843137,0.809803);

float scatter(float LoV)
{
    float PI = 3.14;
    float G_SCATTERING = 0.2f;
    float result = 1.0f - G_SCATTERING * G_SCATTERING;
    result /= (4.0f * PI * pow(1.0f + G_SCATTERING * G_SCATTERING - (2.0f * G_SCATTERING) * LoV, 1.5f));
    return result;
}

float nrand( vec2 n )
{
	return fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

float linearize_depth(float d, float zNear, float zFar)
{
	float z_n = 2.0f * d - 1.0f;
	return 2.0f * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
}

void main(void)
{
	vec2 TexelSize = 1.0/vec2(1920.0,1080.0);
	
	float cosTheta = max(dot(normalize(v3SunPos),normalize(v3EyeDir)),0.0);
	float SunTheta = max(dot(normalize(v3SunPos),vec3(0,1,0)),0.0);

	//ray
	vec2 ndcRay = (2.0 * (TexCoord)-1.0);//-1 to 1

	vec4 ray_view = m4InvProjX *  vec4(ndcRay, 1, 1);
	//ray_view = vec4(ray_view.xyz, 0.0);

	vec3 worldDir = (m4InvViewX * ray_view).xyz;
	worldDir = normalize(worldDir);

	float cutoff = dot(worldDir, vec3(0.0, 1.0, 0.0));
	
	//world pos
	float worldDepth = texture(tSampler2D_DepthTexture,TexCoord).r;

	vec4 fragPos = m4InvViewX* m4InvProjX *  vec4(ndcRay, worldDepth*2.0-1.0, 1);
	fragPos/=fragPos.w;
//	fragPos = m4InvViewX * fragPos;

	vec3 FragToCam = (v3EyePos-fragPos.xyz);
	float rayMarchDistance = length(FragToCam);
	
	float stepSize = rayMarchDistance/32.0 ;


//scene
	vec4 skyColor = texture(tSampler2D_SkyTexture,TexCoord);
	//vec4 cloudColor = texture(tSampler2D_CloudTexture,TexCoord);
	vec4 sceneColor = texture(tSampler2D_SceneTexture,TexCoord);

//PostProcess
//GOD RAYS & Clouds
	float GodRaysColor=0.0;
	vec4 cloudColor;// = texture(tSampler2D_CloudTexture,TexCoord);

//	GodRaysColor += texture(tSampler2D_GodRaysTexture,TexCoord+vec2(-TexelSize.x,TexelSize.y)).r *gaussianKernel3x3[0];
//	GodRaysColor += texture(tSampler2D_GodRaysTexture,TexCoord+vec2(0.0,TexelSize.y)).r			 *gaussianKernel3x3[1];
//	GodRaysColor += texture(tSampler2D_GodRaysTexture,TexCoord+vec2(TexelSize.x,TexelSize.y)).r  *gaussianKernel3x3[2];
//	GodRaysColor += texture(tSampler2D_GodRaysTexture,TexCoord+vec2(-TexelSize.x,0.0)).r		 *gaussianKernel3x3[3];
//	GodRaysColor += texture(tSampler2D_GodRaysTexture,TexCoord+vec2(0.0,0.0)).r					 *gaussianKernel3x3[4];
//	GodRaysColor += texture(tSampler2D_GodRaysTexture,TexCoord+vec2(TexelSize.x,0.0)).r			 *gaussianKernel3x3[5];
//	GodRaysColor += texture(tSampler2D_GodRaysTexture,TexCoord+vec2(-TexelSize.x,-TexelSize.y)).r*gaussianKernel3x3[6];
//	GodRaysColor += texture(tSampler2D_GodRaysTexture,TexCoord+vec2(0.0,-TexelSize.y)).r		 *gaussianKernel3x3[7];
//	GodRaysColor += texture(tSampler2D_GodRaysTexture,TexCoord+vec2(TexelSize.x,-TexelSize.y)).r *gaussianKernel3x3[8];

//int counter=0;
//for(int i=-2 ; i<=2 ; i++)
//{
//	for(int j=-2 ; j<=2 ; j++)
//	{
//		GodRaysColor += texture(tSampler2D_GodRaysTexture,TexCoord+vec2(TexelSize.x*j,TexelSize.y*i)).r *gaussianKernel5x5[counter];
//		cloudColor += texture(tSampler2D_CloudTexture,TexCoord+vec2(TexelSize.x*j,TexelSize.y*i)) *gaussianKernel5x5[counter];
//		counter+=1;
//	}
//}

		GodRaysColor = texture(tSampler2D_GodRaysTexture,TexCoord).r ;
		cloudColor = texture(tSampler2D_CloudTexture,TexCoord) ;

//volume fog
vec4 curWorldPos;
float intensity=0;


float myRand= nrand(gl_FragCoord.xy);

vec3 startPos = (fragPos.xyz+normalize(FragToCam)*stepSize*myRand);

//curWorldPos = fragPos.xyz;

for (int i=0;i<32;i++)
{

 curWorldPos.xyz = (startPos + normalize(FragToCam)*stepSize*(i));

 curWorldPos = (m4LightSpaceX* vec4(curWorldPos.xyz,1.0));
 curWorldPos/=curWorldPos.w;
 curWorldPos.xyz= curWorldPos.xyz*0.5+0.5;

 float depth = texture(tSampler2D_ShadowMapTexture,curWorldPos.xy).r;

 if(depth>curWorldPos.z)
 {
   // intensity= mix(intensity,(depth<curWorldPos.z)?0.0:1.0,1.0/(i+1));
   intensity+= scatter(cosTheta)*0.2;
 }

}
//intensity*=0.2;

//extra
//vec2 cloudDepth_HighAltitudeAlpha = texture(tSampler2D_CloudDepthTexture,TexCoord).rg;
//float cloudDepth = cloudDepth_HighAltitudeAlpha.r;
//float cloudHighAltitudeAlpha = cloudDepth_HighAltitudeAlpha.g;

float atmosphericBlend =clamp((pow(smoothstep(0.0,0.3,max(cutoff,0.0)),0.4)),0,1);
float blendBlend =1.0-smoothstep(0.0,0.1,max(cutoff,0.0));

float skyCloudMask = mix(atmosphericBlend*(cloudColor.a),(cloudColor.a),smoothstep(0.75,0.9,1.0-SunTheta));

float sceneMask = 1.0-texture(tSampler2D_SceneMaskTexture,TexCoord).r;

//do scene blend
	vec4 cloudySky=vec4(mix(skyColor.rgb,cloudColor.rgb,skyCloudMask),1.0);
	FragColor = cloudySky;//removes black edges //   color/(0.000001+cloudColor.a)
	if(cutoff<0.0005&&cutoff>0.0)//horizon artefact correction
	{
		FragColor.rgb = mix(FragColor.rgb,vec3(cloudySky),sceneMask);
	}
	FragColor = vec4(mix(sceneColor.rgb,cloudySky.rgb,sceneMask),1.0);

	float smoothSunFactor =smoothstep(0.1,0.2,SunTheta);
//do post process
//	FragColor.rgb = vec3(skyColor);
	float finalGodRays=GodRaysColor*pow(cosTheta,3.0)*smoothSunFactor;
	FragColor.rgb += (finalGodRays);//*mix(orangeColor,brightYellow,smoothstep(0.4,0.6,SunTheta
//	FragColor.rgb = vec3(sceneColor);

//SKY FOG
//'	FragColor.rgb = vec3(mix(FragColor.rgb,vec3(0.8),blendBlend*sceneMask*min(v3EyePos.y*0.000175,1.0)));

//VIGNETTE
	float d = distance(TexCoord,vec2(0.5));
	float p = smoothstep(0.3,0.55,d)*0;//*fVignette
	//float p = 0.0;//
	FragColor = vec4(mix(FragColor.rgb,vec3(0.0),p),FragColor.a);//+intensity
//	FragColor = texture(tSampler2D_ShadowMapTexture,TexCoord.xy).rrrr;

//	FragColor.rgb=mix(FragColor.rgb,vec3(0.8),intensity);
//	FragColor = vec4(vec3(sceneColor),1.0);
	FragColor = vec4(vec3(cloudColor),1.0);//cloudColor+
	//FragColor = vec4(linearize_depth(texture(tSampler2D_ShadowMapTexture,TexCoord).r,0.1,20000.0)/20000.0);
//CONTRAST
//	float contrast=0.2;
//	FragColor.rgb = (FragColor.rgb - 0.5) * (1.0 + contrast) + 0.5;
}


