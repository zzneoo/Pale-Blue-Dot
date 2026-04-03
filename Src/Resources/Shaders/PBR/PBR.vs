#version 460 core
		
layout(location=0)in vec4 vPosition;
layout(location=1)in vec2 vTexCoord;
layout(location=2)in vec4 vNormal;
layout(location=3)in vec4 vTangent;

uniform mat4 m4ModelViewProjX;
uniform mat4 m4ModelX;
uniform mat4 m4ViewX;
uniform mat4 m4PrevViewX;
uniform mat4 m4ProjX;

uniform vec3 v3SunPos;
uniform vec3 v3EyePos;

uniform float fTime;

out vec4 outColor;
out vec2 outTexCoord;
out vec3 toLightVectorTangentSpace;
out vec3 toCameraVectorTangentSpace;
out vec3 surfaceNormalFs;

out vec2 vel;

float nrand( vec2 n )
{
	return fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

void main(void)
{

	outTexCoord = vTexCoord;

	mat3 m3ModelX = mat3(m4ModelX);

	vec3 surfaceTangent = normalize(m3ModelX*vTangent.xyz);
	vec3 surfaceNormal = normalize(m3ModelX*vNormal.xyz);
		surfaceNormalFs = surfaceNormal;
	vec3 surfaceBitangent = normalize(cross(surfaceNormal,surfaceTangent));

	vec3 WorldPos = (m4ModelX*vPosition).xyz;

	outColor = vec4(surfaceBitangent,1.0);

	mat3 toTangentSpace = mat3
	(
	surfaceTangent.x,surfaceBitangent.x,surfaceNormal.x ,
	surfaceTangent.y,surfaceBitangent.y,surfaceNormal.y ,
	surfaceTangent.z,surfaceBitangent.z,surfaceNormal.z 
	);

	toLightVectorTangentSpace = toTangentSpace*(v3SunPos-WorldPos);
	toCameraVectorTangentSpace = toTangentSpace*(v3EyePos-WorldPos);

	//gl_Position = m4ModelViewProjX * vPosition;
	gl_Position = m4ProjX*m4ViewX*m4ModelX * vPosition;

}
