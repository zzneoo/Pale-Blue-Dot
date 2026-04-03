#version 460 core

uniform vec2 v2Halton23;
		
out vec2 TexCoord;
out vec2 v2NDC;

uniform mat4 m4InvViewX;
uniform mat4 m4InvProjX;

out vec3 v3ViewRay;

void main(void)
{
	const vec2 vertices[] = vec2[](vec2(1.0, 1.0),
                                   vec2( -1.0, 1.0),
                                   vec2(-1.0, -1.0),
                                   vec2( 1.0, -1.0));

	TexCoord = vertices[gl_VertexID] * 0.5 + 0.5;
	v2NDC = vertices[gl_VertexID];
	
	
	//float projectionExtentY = -tan(radians(0.5*50));
	//float projectionExtentX = projectionExtentY*(1920.0/1080.0);
	
	//vec2 projectionExtents = vec2(projectionExtentX,projectionExtentY);
	//v2ViewRay = vec2(v2NDC) * projectionExtents ;//+jitter + (v2Halton23*2.0-1.0)/vec2(1920.0,1080.0)*0.5;
	
	vec4 vertex = vec4(v2NDC,0.0,1.0);
	v3ViewRay=(m4InvViewX * vec4((m4InvProjX * vertex).xyz, 0.0)).xyz;
	
	gl_Position = vec4(vertices[gl_VertexID],0.0,1.0);
}
