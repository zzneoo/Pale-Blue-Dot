#version 460 core
	
#extension GL_ARB_shader_draw_parameters : require

layout (binding = 0, std140) uniform MODEL_MATRIX_BLOCK
{
	mat4    model_matrix[1024];
};
	
layout(location=0)in vec4 vPosition;
layout(location=1)in vec2 vTexCoord;

uniform mat4 m4ModelViewProjX;

out vec2 outTexCoord;


void main(void)
{

	outTexCoord = vec2(vTexCoord.x,1.0-vTexCoord.y);

	gl_Position = m4ModelViewProjX * model_matrix[gl_BaseInstanceARB]* vPosition;
}
