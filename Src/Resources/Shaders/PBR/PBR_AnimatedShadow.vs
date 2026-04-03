#version 460 core
		
layout(location=0)in vec4 vPosition;

layout(location=4)in ivec4 vJointIndices;
layout(location=5)in vec4 vJointWeights;

#define CONFIG_MAX_BONES_PER_VERTEX 4

uniform mat4 m4ModelX; //model matrix
uniform mat4 m4ViewX; //view matrix
uniform mat4 m4ProjX; //projection matrix

uniform mat4 u_jointTransforms[91];//REGEX_REPLACE_VAR_NUM_BONES1 will be replaced by string substitution

  
void main()
{

    mat4 mv_matrix = m4ViewX * m4ModelX;
    
    mat4 boneTransform = mat4(0.0);
    for(int i = 0; i < CONFIG_MAX_BONES_PER_VERTEX; i++)
    {
        boneTransform += u_jointTransforms[vJointIndices[i]] * vJointWeights[i];
    } 
    
    vec4 localPos        = boneTransform * vPosition;
    vec4 eye_coords      = mv_matrix * localPos;
    gl_Position = m4ProjX * eye_coords;

}