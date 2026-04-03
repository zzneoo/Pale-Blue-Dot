#version 460 core

layout (location = 0 )out vec4 FragColor_frame;
layout (location = 1 )out vec4 FragColor_frame_extra;//farClouds

uniform sampler2D tSamplerCurrFrameNearest;
uniform sampler2D tSamplerCurrFrameLinear;
uniform sampler2D tSamplerCurrExtra;

uniform sampler2D tSamplerPrevFrame;
uniform sampler2D tSamplerPrevExtra;
uniform sampler2D tSamplerCurrVelocity;


uniform vec3 v3CameraPosition;

uniform mat4 m4ProjX;
uniform mat4 m4PrevViewX;
uniform mat4 m4ViewX;

uniform float fTime;
uniform int iBlockPixelID;

in vec2 TexCoord;

vec4 BiCubicCatmullRom5Tap (sampler2D sampler, vec2 P,vec2 textureSize)
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

    vec4 Ct = texture(sampler, vec2(Sample[1].x, Sample[0].y)) * sampleWeight[0];    
    vec4 Cl = texture(sampler, vec2(Sample[0].x, Sample[1].y)) * sampleWeight[1];    
    vec4 Cc = texture(sampler, vec2(Sample[1].x, Sample[1].y)) * sampleWeight[2];    
    vec4 Cr = texture(sampler, vec2(Sample[2].x, Sample[1].y)) * sampleWeight[3];    
    vec4 Cb = texture(sampler, vec2(Sample[1].x, Sample[2].y)) * sampleWeight[4];
    
    float WeightMultiplier = 1./(sampleWeight[0]+sampleWeight[1]+sampleWeight[2]+sampleWeight[3]+sampleWeight[4]);
    
    
    return (Ct+Cl+Cc+Cr+Cb)*WeightMultiplier;   
}

vec3 RGB2YCoCg(vec3 RGB)
{
    vec3 o;
    o.x =  0.25*RGB.r + 0.5*RGB.g + 0.25*RGB.b;
    o.y =  0.5*RGB.r - 0.5*RGB.b;
    o.z = -0.25*RGB.r + 0.5*RGB.g - 0.25*RGB.b;
    return o;
}

vec3 YCoCg2RGB(vec3 YCoCg)
{
    vec3 o;
    o.r = YCoCg.x + YCoCg.y - YCoCg.z;
    o.g = YCoCg.x + YCoCg.z;
    o.b = YCoCg.x - YCoCg.y - YCoCg.z;
    return o;
}

void main(void)
{
	ivec2 tempID=ivec2(gl_FragCoord)%4;
	int id=tempID.y*4+tempID.x;

	vec2 velocity=texture(tSamplerCurrVelocity,TexCoord).xy;
	vec2 prevUV=TexCoord-velocity;

	ivec2 pixelOffset;
	pixelOffset.x = iBlockPixelID % 4;
	pixelOffset.y = iBlockPixelID / 4;

	vec4 finalColor;
	vec4 finalColorExtra;
	vec4 CurrColor=texture(tSamplerCurrFrameNearest,TexCoord);
	vec4 CurrColorExtra=texture(tSamplerCurrExtra,TexCoord);

	bool boundryUVCheck = (abs(prevUV.x-0.5)>0.5)||(abs(prevUV.y-0.5)>0.5);

	vec4 PrevColor=boundryUVCheck?CurrColor:texture(tSamplerPrevFrame,prevUV);
	vec4 PrevColorExtra=boundryUVCheck?CurrColorExtra:texture(tSamplerPrevExtra,prevUV);

	
	vec4 mean = BiCubicCatmullRom5Tap(tSamplerCurrFrameLinear,TexCoord,vec2(480.0,270.0));
	mean.rgb = RGB2YCoCg(mean.rgb);
	vec4 stdDev = sqrt(mean * mean-mean);

	vec4 minCol = mean - stdDev;
	vec4 maxCol = mean + stdDev;

	//PrevColor = clamp(vec4(RGB2YCoCg(PrevColor.rgb),PrevColor.a),minCol,maxCol);
	//PrevColor.rgb = YCoCg2RGB(PrevColor.rgb);

	finalColor = PrevColor;
	finalColorExtra = PrevColorExtra;
	if(id==iBlockPixelID)
	{
		finalColor = CurrColor;
		finalColorExtra = CurrColorExtra;
	}

//	FragColor_frame = 0.05*bP*CurrColor+(1.0-0.05*bP)*finalColor;
	//FragColor_frame = mix(finalColor,PrevColor,0.05);
	FragColor_frame = (finalColor);
	FragColor_frame_extra =finalColorExtra;

}
