#version 460 core
		
out vec2 TexCoord;

void main(void)
{
	const vec2 vertices[] = vec2[](vec2(-1.0, -1.0 ),
                                   vec2( 1.0, -1.0 ),
                                   vec2( -1.0, 1.0 ),
                                   vec2( 1.0, 1.0 ));

	TexCoord = vertices[gl_VertexID] * 0.5 + 0.5;
	gl_Position = vec4(vertices[gl_VertexID],0.0,1.0);
}
