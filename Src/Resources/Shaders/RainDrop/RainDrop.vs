#version 460 core
		
layout(location=0)in vec4 vPosition;
layout(location=2)in vec4 vNormal;

uniform mat4 m4ProjX;
uniform mat4 m4ViewX;
uniform mat4 m4ModelX;

uniform vec3 v3EyePos;

out vec3 vsTexCoord;
out vec3 vsNormal;
out vec3 EyeVector;

void main(void)
{
	vec4 worldSpace = m4ModelX*vPosition;
	vec4 eyeSpace = m4ViewX*worldSpace;

	gl_Position =m4ProjX* eyeSpace;

	EyeVector = (worldSpace.xyz-v3EyePos);

	vsTexCoord = vPosition.xyz;
	vsNormal = (vNormal).xyz;
}
