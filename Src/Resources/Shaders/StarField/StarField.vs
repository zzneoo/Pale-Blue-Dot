#version 460 core

uniform mat4 m4ModelViewProjX;
uniform float fIntensityFactor;

out vec3 color;

float random (in vec2 _st)
{
    return fract(sin(dot(_st.xy,vec2(12.9898,78.233)))*43758.5453123);
}

void main(void)
{
	vec4 numbers =  vec4(gl_InstanceID*0.1435,gl_InstanceID*0.2356,gl_InstanceID*0.9678,gl_InstanceID*0.3532)/300000.0;
	int index = gl_InstanceID%4;
	float param =  (gl_InstanceID/300000.0);
	vec2 v2Param = vec2(param,param*0.7);
	vec3 InstancePosition = vec3(random(v2Param*numbers.x),random(v2Param*numbers.y),random(v2Param*numbers.z));
	
	InstancePosition =normalize(InstancePosition*2.0-1.0);
	InstancePosition = InstancePosition*5000000.0;
		
	gl_Position = m4ModelViewProjX*vec4(InstancePosition-vec3(0.0,63600.0,0.0),1.0);
	gl_PointSize =1.0+param*5.0+smoothstep(0.95,1.0,param)*8.0;
	
	color = vec3(pow(random(v2Param),10.0));
	color+=(color*0.4);
	color*=vec3(0.7+random(v2Param*8765)*0.3,0.7+random(v2Param*0.474747)*0.3,0.8+random(v2Param*0.25545)*0.2);
	
	float Intensity = fIntensityFactor;
	color*=Intensity;
}
