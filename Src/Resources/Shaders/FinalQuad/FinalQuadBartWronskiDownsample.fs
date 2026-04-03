#version 460 core

out vec4 FragColor;

uniform sampler2D tSampler0;

in vec2 TexCoord;

vec3 BiCubicCatmullRom5Tap(sampler2D sampler, vec2 P,vec2 textureSize)
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

vec3 BartWronskiDownsample(sampler2D tSampler,vec2 UV,vec2 InvRes)
{
    vec3 col = vec3(0.0);
    col += 0.37487566 * texture(tSampler, UV + vec2(-0.75777156,-0.75777156)*InvRes).xyz;
    col += 0.37487566 * texture(tSampler, UV + vec2(0.75777156,-0.75777156)*InvRes).xyz;
    col += 0.37487566 * texture(tSampler, UV + vec2(0.75777156,0.75777156)*InvRes).xyz;
    col += 0.37487566 * texture(tSampler, UV + vec2(-0.75777156,0.75777156)*InvRes).xyz;
    
    col += -0.12487566 * texture(tSampler, UV + vec2(-2.90709914,0.0)*InvRes).xyz;
    col += -0.12487566 * texture(tSampler, UV + vec2(2.90709914,0.0)*InvRes).xyz;
    col += -0.12487566 * texture(tSampler, UV + vec2(0.0,-2.90709914)*InvRes).xyz;
    col += -0.12487566 * texture(tSampler, UV + vec2(0.0,2.90709914)*InvRes).xyz; 
	
	return col;
}

void main(void)
{
	vec4 finalColor=vec4(BartWronskiDownsample(tSampler0,TexCoord,1.0/vec2(7680,4320)),1.0);
	//vec4 finalColor=vec4(BiCubicCatmullRom5Tap(tSampler0,TexCoord,vec2(1920,1080)),1.0);
	FragColor = finalColor;
}
