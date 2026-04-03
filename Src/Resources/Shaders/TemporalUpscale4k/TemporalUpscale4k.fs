#version 460 core

layout (location = 0 )out vec4 FragColor_frame;

const float FLT_EPS = 0.00000001;

uniform sampler2D tSamplerCurrFrameLinear;
uniform sampler2D tSamplerPrevFrame;

uniform sampler2D tSamplerCurrVelocity;

uniform int iBlockPixelID;
uniform int iBlockWidth;

uniform vec2 v2Resolution;

in vec2 TexCoord;

vec4 BiCubicCatmullRom5Tap(sampler2D sampler, vec2 P,vec2 textureSize)
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

vec3 clip_aabb(vec3 aabb_min, vec3 aabb_max, vec3 p, vec3 q)
{

		vec3 r = q - p;
		vec3 rmax = aabb_max - p.xyz;
		vec3 rmin = aabb_min - p.xyz;

		const float eps = FLT_EPS;

		if (r.x > rmax.x + eps)
			r *= (rmax.x / r.x);
		if (r.y > rmax.y + eps)
			r *= (rmax.y / r.y);
		if (r.z > rmax.z + eps)
			r *= (rmax.z / r.z);

		if (r.x < rmin.x - eps)
			r *= (rmin.x / r.x);
		if (r.y < rmin.y - eps)
			r *= (rmin.y / r.y);
		if (r.z < rmin.z - eps)
			r *= (rmin.z / r.z);

		return p + r;
}

void main(void)
{
	ivec2 tempID=ivec2(gl_FragCoord)%iBlockWidth;
	int id=tempID.y*iBlockWidth+tempID.x;

	vec2 velocity=texture(tSamplerCurrVelocity,TexCoord).xy;
	vec2 prevUV=TexCoord-velocity;
	
	vec4 finalColor;
//	vec4 CurrentFrame = BiCubicCatmullRom5Tap(tSamplerCurrFrameLinear,TexCoord,vec2(1920,1080));
	vec4 CurrentFrame = texture(tSamplerCurrFrameLinear,TexCoord);
	vec4 PreviousFrame;
	if(id==iBlockPixelID)
	{	
		finalColor=CurrentFrame;	
	}
	else     
	{
		//finalColor=(abs(prevUV.x-0.5)>0.5)||(abs(prevUV.y-0.5)>0.5)?texture(tSamplerCurrFrameLinear,TexCoord):vec4(BiCubicCatmullRom5Tap(tSamplerPrevFrame,prevUV,vec2(3840,2160)),1.0);//velocity points outside the screen  //University of Gothenburg
		finalColor=(abs(prevUV.x-0.5)>0.5)||(abs(prevUV.y-0.5)>0.5)?CurrentFrame:BiCubicCatmullRom5Tap(tSamplerPrevFrame,prevUV,v2Resolution);//velocity points outside the screen  //University of Gothenburg
		//finalColor=(abs(prevUV.x-0.5)>0.5)||(abs(prevUV.y-0.5)>0.5)?texture(tSamplerCurrFrameLinear,TexCoord):texture(tSamplerPrevFrame,prevUV);//velocity points outside the screen  //University of Gothenburg

	}
		
		CurrentFrame.rgb = RGB2YCoCg(CurrentFrame.rgb);
		//PreviousFrame = vec4((finalColor.rgb),finalColor.a);
		PreviousFrame = vec4(RGB2YCoCg(finalColor.rgb),finalColor.a);
		
		float luminanceCurr = CurrentFrame.r;
		float luminancePrev = PreviousFrame.r;

		float unbiased_diff = abs(luminanceCurr - luminancePrev) / max(luminanceCurr, max(luminancePrev, 0.2));
		float unbiased_weight = 1.0 - unbiased_diff;
		float unbiased_weight_sqr = unbiased_weight * unbiased_weight;
		float k_feedback = mix(0.88, 0.97, unbiased_weight_sqr);
		
		CurrentFrame.rgb = YCoCg2RGB(CurrentFrame.rgb);
		PreviousFrame.rgb = YCoCg2RGB(PreviousFrame.rgb);
		
		//FragColor_frame = finalColor;
		//FragColor_frame = mix(CurrentFrame,PreviousFrame,k_feedback);
		FragColor_frame =finalColor;
		//FragColor_frame.a =finalColor.a;
}
