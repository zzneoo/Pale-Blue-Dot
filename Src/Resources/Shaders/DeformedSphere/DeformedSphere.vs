#version 460 core

out vec2 vsTexCoords;
//out vec2 tesTexCoords;

void main(void)
{
    const vec4 vertices[] = vec4[](vec4(-0.5, 0.0, -0.5, 1.0),
                                   vec4( 0.5, 0.0, -0.5, 1.0),
                                   vec4(-0.5, 0.0,  0.5, 1.0),
                                   vec4( 0.5, 0.0,  0.5, 1.0));

   // int x = gl_InstanceID & 63;
  //  int y = gl_InstanceID >> 6;

	int x = gl_InstanceID % 32;
    int y = gl_InstanceID / 32;
    vec2 offs = vec2(x, y);
 
    vsTexCoords = (vertices[gl_VertexID].xz + offs + vec2(0.5)) / vec2(32.0,32.0);
    vec4 DeformedPosition = vertices[gl_VertexID] + vec4(float(x - 16), 0.0,
                                               float(y - 16), 1.0);
	

	
	float lat = -vsTexCoords.x  *3.141592 *2.0 ;
	float lon =  vsTexCoords.y  *3.141592 ;
	
	vsTexCoords = vec2(vsTexCoords.x,1-vsTexCoords.y);
	
	DeformedPosition.x = cos(lat)*sin(lon);
	DeformedPosition.y = cos(lon);
	DeformedPosition.z = sin(lat)*sin(lon);
	
	gl_Position = DeformedPosition;
}
