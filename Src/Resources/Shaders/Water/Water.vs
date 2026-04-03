#version 460 core

#define INFINITY 2000000000

out vec2 vsTexCoords;
out vec3 color;
out float eyeDistance; 
out float originDistance;; 
out float waterHeight;

uniform mat4 m4ModelX;
uniform mat4 m4ViewX;
uniform mat4 m4ProjX;

uniform vec3 v3EyePos;
uniform vec3 v3SunPos;

uniform float fWaterFactor;

out vec3 toSunVector;
out vec3 toEyeVector;

uniform sampler2D tSampler_heightmap;

//PROJECTED grid

vec2 getImagePlane(vec2 uv)
{
  // http://www.terathon.com/gdc07_lengyel.pdf
  // Extracting aspect and focal from projection matrix:
  // e = focal, h = height, w = width, P = projection matrix
  // P = | e   0       0   0 |
  //     | 0   e/(h/w) 0   0 |
  //     | 0   0       .   . |
  //     | 0   0       -1  0 |
  float focal = m4ProjX[0].x;
  float aspect = m4ProjX[1].y;

  // Fix coordinate aspect and scale
  return vec2( ( uv.x - 0.5 ) * aspect*1.0, ( uv.y - 0.5 ) * focal*1.5 );
}

mat3 getRotation()
{
  // Extract the 3x3 rotation matrix from the 4x4 view matrix
  return mat3(
    m4ViewX[0].xyz,
    m4ViewX[1].xyz,
    m4ViewX[2].xyz
  );
}

vec3 getCameraPos( in mat3 rotation )
{
  // Xc = R * Xw + t
  // c = - R.t() * t <=> c = - t.t() * R
  return - m4ViewX[3].xyz * rotation;
}

vec3 getCamRay( in mat3 rotation, in vec2 screenUV )
{
  // Compute camera ray then rotate it in order to get it in world coordinate
  return vec3( screenUV.x, screenUV.y, m4ProjX[0].x ) * rotation;
}

vec3 intersectPlane( in vec3 source, in vec3 dir, in vec3 normal, float height )
{
  // Compute the distance between the source and the surface, following a ray, then return the intersection
  // http://www.cs.rpi.edu/~cutler/classes/advancedgraphics/S09/lectures/11_ray_tracing.pdf
  float distance = ( - height - dot( normal, source ) ) / dot( normal, dir );
  if( distance < 0.0 )
    return source + dir * distance;
  else
    return - ( vec3( source.x, height, source.z ) + vec3( dir.x, height, dir.z ) * INFINITY );
}

void main(void)
{
    const vec4 verticesQuads[] = vec4[](vec4(0.5, 0.0, -0.5, 1.0),
                                        vec4( -0.5, 0.0, -0.5, 1.0),
                                        vec4(-0.5, 0.0,  0.5, 1.0),
                                        vec4( 0.5, 0.0,  0.5, 1.0));

   // int x = gl_InstanceID & 63;
  //  int y = gl_InstanceID >> 6;

	int x = gl_InstanceID % 256;
    int y = gl_InstanceID / 256;
    vec2 offs = vec2(x, y);
 
  vec2 tempPos = ((verticesQuads[gl_VertexID].xz +vec2(float(x - 128),float(y-128)))/256.0);
  tempPos=tempPos+0.5;
  tempPos.y=(1.0-tempPos.y);


// Extract camera position and rotation from the view matrix
    mat3 cameraRotation = getRotation();
vec3 camPosition = getCameraPos( cameraRotation );

// Extract coordinate of the vertex on the image plan
vec2 screenUV = getImagePlane(tempPos);

// Compute the ray from camera to world
vec3 ray = getCamRay( cameraRotation, screenUV );
vec3 finalPos = intersectPlane( v3EyePos, ray, vec3(0,1,0), 0.0 );

float distance = length( finalPos );
if( distance > INFINITY )
  finalPos *= INFINITY / distance;

  vsTexCoords=vec2(finalPos.xz)*0.001;

  float height =texture(tSampler_heightmap,vsTexCoords*8).r;

  //color =vec3(smoothstep(0.0,1.0,distance*0.00002));

  vec4 WorldPos=vec4(finalPos.x,height*5,finalPos.z,1.0);

  waterHeight=WorldPos.y;

   toSunVector=normalize(v3SunPos);
   toEyeVector=normalize(v3EyePos-WorldPos.xyz);

   vec4 eyeCoords=m4ViewX*WorldPos;

   eyeDistance = length(eyeCoords);
   originDistance = distance*0.00002;

    gl_Position=m4ProjX*eyeCoords;

}
