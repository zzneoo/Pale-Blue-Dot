#version 460 core
		
layout(location=0)in vec4 vPosition;
layout(location=1)in vec2 vTexCoord;
layout(location=2)in vec4 vNormal;
layout(location=3)in vec4 vTangent;
layout(location=4)in ivec4 vJointIndices;
layout(location=5)in vec4 vJointWeights;

#define CONFIG_MAX_BONES_PER_VERTEX 4

uniform mat4 m4ModelX; //model matrix
uniform mat4 m4ViewX; //view matrix
uniform mat4 m4ProjX; //projection matrix

uniform mat4 u_jointTransforms[91];//REGEX_REPLACE_VAR_NUM_BONES1 will be replaced by string substitution

uniform vec3 v3SunPos;
uniform vec3 v3EyePos;

out vec2 outTexCoord;

out vec3 toLightVectorTangentSpace;
out vec3 toCameraVectorTangentSpace;

out vec3 color;  

void main()
{

    mat4 mv_matrix = m4ViewX * m4ModelX;
    
    mat4 boneTransform = mat4(0.0);
    for(int i = 0; i < CONFIG_MAX_BONES_PER_VERTEX; i++)
    {
        boneTransform += u_jointTransforms[vJointIndices[i]] * vJointWeights[i];
    } 
    
    mat3 normalTransform = mat3(transpose(inverse(boneTransform)));
    vec4 localPos        = boneTransform * vPosition;
    vec3 localNormal     = normalTransform * vNormal.xyz;
    vec3 localTangent     = (normalTransform * vTangent.xyz);
    vec4 eye_coords      = mv_matrix * localPos;
    gl_Position = m4ProjX * eye_coords;

    outTexCoord = vTexCoord;

    mat3 m3ModelX =mat3(m4ModelX);

	vec3 surfaceTangent = normalize(m3ModelX*localTangent);
	vec3 surfaceNormal = normalize(m3ModelX*localNormal);
	vec3 surfaceBitangent = normalize(cross(surfaceNormal,surfaceTangent));

	vec3 WorldPos = (m4ModelX*localPos).xyz;

	mat3 toTangentSpace = mat3
	(
	surfaceTangent.x,surfaceBitangent.x,surfaceNormal.x ,
	surfaceTangent.y,surfaceBitangent.y,surfaceNormal.y ,
	surfaceTangent.z,surfaceBitangent.z,surfaceNormal.z 
	);

	toLightVectorTangentSpace = toTangentSpace*(v3SunPos-WorldPos);
	toCameraVectorTangentSpace = toTangentSpace*(v3EyePos-WorldPos);
}