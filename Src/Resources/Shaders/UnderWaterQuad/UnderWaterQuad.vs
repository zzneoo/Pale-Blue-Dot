#version 460 core

uniform mat4 m4ModelViewProjX;		
uniform mat4 m4ModelX;		

out float vsHeight;

void main(void)
{
	const vec2 vertices[] = vec2[](vec2(1.0, 1.0),
                                   vec2( -1.0, 1.0),
                                   vec2(-1.0, -1.0),
                                   vec2( 1.0, -1.0));
	
	vsHeight = (m4ModelX * vec4(vertices[gl_VertexID].x,0.0,vertices[gl_VertexID].y,1.0)).y;
	
	gl_Position =m4ModelViewProjX* vec4(vertices[gl_VertexID].x,0.0,vertices[gl_VertexID].y,1.0);
}
