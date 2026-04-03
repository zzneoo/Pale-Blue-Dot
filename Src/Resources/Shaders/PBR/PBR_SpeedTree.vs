#version 460 core
	
#extension GL_ARB_shader_draw_parameters : require

layout (binding = 0, std140) uniform MODEL_MATRIX_BLOCK
{
	mat4    model_matrix[1024];
};
	
layout(location=0)in vec4 vPosition;
layout(location=1)in vec2 vTexCoord;
layout(location=2)in vec4 vNormal;
layout(location=3)in vec4 vTangent;

uniform mat4 m4ModelViewProjX;
uniform mat4 m4ModelX;

uniform vec3 v3SunPos;
uniform vec3 v3EyePos;

out vec4 outColor;
out float treeWorldHeight;
out vec2 outTexCoord;
out vec3 toLightVectorTangentSpace;
out vec3 toCameraVectorTangentSpace;

void main(void)
{

	outTexCoord = vec2(vTexCoord.x,1.0-vTexCoord.y);

	mat3 m3ModelX =mat3(m4ModelX);
	mat3 m3PerInstanceModelX =mat3(model_matrix[gl_BaseInstanceARB]);

	vec3 surfaceTangent = normalize(m3PerInstanceModelX*vTangent.xyz);
	vec3 surfaceNormal = normalize(m3PerInstanceModelX*vNormal.xyz);
	vec3 surfaceBitangent = normalize((cross(surfaceNormal,surfaceTangent)));

	vec3 WorldPos = (model_matrix[gl_BaseInstanceARB]*vPosition).xyz;
	treeWorldHeight = WorldPos.y;

	outColor = vec4(surfaceTangent,1.0);

	mat3 toTangentSpace = mat3
	(
	surfaceTangent.x,surfaceBitangent.x,surfaceNormal.x ,
	surfaceTangent.y,surfaceBitangent.y,surfaceNormal.y ,
	surfaceTangent.z,surfaceBitangent.z,surfaceNormal.z 
	);

	toLightVectorTangentSpace = toTangentSpace*(v3SunPos-WorldPos);
	toCameraVectorTangentSpace = toTangentSpace*(v3EyePos-WorldPos);

	gl_Position = m4ModelViewProjX * model_matrix[gl_BaseInstanceARB]* vPosition;
}
