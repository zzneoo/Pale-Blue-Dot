#version 460 core

out vec4 FragColor;

uniform sampler2D tSamplerBackground;
uniform sampler2D tSamplerFlare;
uniform sampler2D tSamplerDeffered;
uniform sampler2D tSamplerClouds;
uniform sampler2D tSamplerFarClouds;
uniform sampler2D tSamplerExtra;

uniform vec2 v2InvResolution;

in vec2 TexCoord;

//Sharpness Filter
vec3 SharpnessFilter(sampler2D inTexture ,vec2 UV,float intensity, vec2 invTexRes)// 0.0 to 1.0
{
    vec3 sum = texture(inTexture, UV + vec2(-1.0,-1.0)*invTexRes).rgb * -intensity
             + texture(inTexture, UV + vec2(1.0,1.0)*invTexRes).rgb * -intensity
             + texture(inTexture, UV + vec2(-1.0,1.0)*invTexRes).rgb * -intensity
             + texture(inTexture, UV + vec2(1.0,-1.0)*invTexRes).rgb * -intensity
             + texture(inTexture, UV + vec2(0.0,-1.0)*invTexRes).rgb * -intensity
             + texture(inTexture, UV + vec2(0.0, 1.0)*invTexRes).rgb * -intensity
             + texture(inTexture, UV + vec2(-1.0,0.0)*invTexRes).rgb * -intensity
             + texture(inTexture, UV + vec2(1.0,0.0)*invTexRes).rgb * -intensity
             + texture(inTexture, UV + vec2(0.0,0.0)*invTexRes).rgb * (intensity*8.0+1.0);
    
	return sum;
}

vec4 BartWronskiDownsample(sampler2D tSampler,vec2 UV,vec2 InvRes)
{
    vec4 col = vec4(0.0);
    col += 0.37487566 * texture(tSampler, UV + vec2(-0.75777156,-0.75777156)*InvRes);
    col += 0.37487566 * texture(tSampler, UV + vec2(0.75777156,-0.75777156)*InvRes);
    col += 0.37487566 * texture(tSampler, UV + vec2(0.75777156,0.75777156)*InvRes);
    col += 0.37487566 * texture(tSampler, UV + vec2(-0.75777156,0.75777156)*InvRes);
    
    col += -0.12487566 * texture(tSampler, UV + vec2(-2.90709914,0.0)*InvRes);
    col += -0.12487566 * texture(tSampler, UV + vec2(2.90709914,0.0)*InvRes);
    col += -0.12487566 * texture(tSampler, UV + vec2(0.0,-2.90709914)*InvRes);
    col += -0.12487566 * texture(tSampler, UV + vec2(0.0,2.90709914)*InvRes); 
	
	return col;
}

void main(void)
{
	vec4 DefferedColor=BartWronskiDownsample(tSamplerDeffered,TexCoord,v2InvResolution);
	//vec4 DefferedColor=vec4(texture(tSamplerDeffered,TexCoord).rgb,1.0);
	// DefferedColor.a=texture(tSamplerDeffered,TexCoord).a;
    vec4 Clouds = texture(tSamplerClouds,TexCoord);
    vec4 FarClouds = texture(tSamplerFarClouds,TexCoord);
	vec4 FlareColor=texture(tSamplerFlare,TexCoord);
	vec4 BackgroundColor=texture(tSamplerBackground,TexCoord);
	float CloudsHorizonMask=texture(tSamplerExtra,TexCoord).r;//only ground clouds

	//DefferedColor.rgb = SharpnessFilter(tSamplerDeffered,TexCoord,0.07,1.0/vec2(1920.0,1080.0));
	//BackgroundColor.rgb = SharpnessFilter(tSamplerBackground,TexCoord,0.1,1.0/vec2(1920.0,1080.0));
	vec3 Layer0 = mix(BackgroundColor.rgb,FarClouds.rgb,FarClouds.a*CloudsHorizonMask);//sky-FarClouds
	vec3 Layer1 = mix(Layer0,Clouds.rgb,Clouds.a*CloudsHorizonMask);//sky-Clouds
	vec3 Layer2 = Layer1*(1.0-DefferedColor.a)+DefferedColor.rgb;//DefferedPBR
	vec3 Layer3 = Layer2*(1.0-pow(FlareColor.r,2.0))+FlareColor.rgb*clamp(FlareColor.r+1.0*FlareColor.r,0.0,1.0);//Flare

	//FragColor = vec4(mix(DefferedColor.rgb,FlareColor.rgb,FlareColor.r),1.0);
	FragColor = vec4(Layer3,1.0);
	//FragColor = vec4(DefferedColor.rgb*(1.0-pow(FlareColor.r,2.0))+FlareColor.rgb*clamp(FlareColor.r+1.0*FlareColor.r,0.0,1.0),1.0);
}
