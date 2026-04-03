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

	int x = gl_InstanceID % 1024;
    int y = gl_InstanceID / 1024;
    vec2 offs = vec2(x, y);
 
    vsTexCoords = (vertices[gl_VertexID].xz + offs + vec2(0.5)) / 1024.0;
    gl_Position = vertices[gl_VertexID] + vec4(float(x - 512), 0.0,
                                               float(y - 512), 0.0);

}
