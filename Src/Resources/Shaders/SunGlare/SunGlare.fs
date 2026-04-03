#version 460 core
		
in vec2 TexCoord;
uniform sampler2D tSampler_StockFlare;
uniform mat4 m4ModelViewProjX;
uniform vec3 fFactorPitchYaw;

out vec4 Frag_Color;

float anflare(vec2 uv, float intensity, float stretch,float height, float brightness)
{
    uv.x *= 1.0/(intensity*stretch);
    uv.y *= height;
    return smoothstep(0.009, 0.0, length(uv))*brightness;
}
float anflare2(vec2 uv, float intensity, float stretch,float height, float brightness)
{
    uv.x *= 1.0/(intensity*stretch);
    uv.y *= height;
    return clamp(smoothstep(0.009, 0.0, length(vec2(0.0,uv.y)))*smoothstep(0.5,0.45,abs((TexCoord.x-0.5)))*brightness,0.0,1.0)*0.372;//
}

vec3 unreal(vec3 x) 
{
  return x / (x + 0.155) * 1.019;
}

void main(void)
{
	vec2 UV = TexCoord-0.5;
	float scaleFactor =2.0;
	vec2 scaledUV =(UV*scaleFactor)+0.5;
	float stockFlare = texture(tSampler_StockFlare,scaledUV).r;//
	

	float angle = atan(UV.y, UV.x);
	float diffract = abs(sin((angle*3.0+cos(angle*9.0-fFactorPitchYaw.z*0.5)+fFactorPitchYaw.y*0.5)*2.21))/8.0*(abs(sin(angle*9.-fFactorPitchYaw.y*0.5)));///1.
	//float diffract = (abs(sin(angle*9.)))/1.;
	float flare1 =anflare(vec2(UV.x,UV.y+0.00122),50.7,1.0,5.4,0.9);
	float flare2 =anflare(vec2(UV.x,UV.y-0.00122),50.7,1.0,5.4,0.9);
	float flare3 =anflare2(UV,50.7,1.0,1.04,0.952);
	
	float longFactor = smoothstep(0.4,1.0,abs(sin(angle*1.5-fFactorPitchYaw.z*0.5+cos(angle*4.16+fFactorPitchYaw.y*0.5))))*(1.0-length(UV*1.824));
	float shortFactor = max(1.0-length(UV*10.0),0.0);
	float finalFactor = max(longFactor,shortFactor);
	
	float diffractionIntensity = 1.5;
	float flareIntensity = 0.47;
	vec3 ColoredFlare = vec3(1.0,0.8675,0.723)*((diffract*diffract*finalFactor*4.46+stockFlare)*diffractionIntensity+(flare1+flare2+flare3)*flareIntensity);
	Frag_Color = vec4((ColoredFlare),1.0);
	//Frag_Color.rgb = vec3((flare3));
	//Frag_Color.rgb = vec3((stockFlare));
}
