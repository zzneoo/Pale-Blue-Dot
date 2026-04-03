#version 460 core
		
#define PI 3.14159265

layout(location=0)in vec4 vPosition;

uniform float fSkyFactor;

uniform mat4 m4ModelViewProjX;

uniform vec3 v3CameraPos;		
uniform vec3 v3LightPos;		
uniform vec3 v3InvWavelength;

uniform float fOuterRadius;			// The outer (atmosphere) radius
uniform float fInnerRadius;			// The inner (planetary) radius

uniform float fKrESun;				// Kr * ESun
uniform float fKmESun;				// Km * ESun

uniform float fKr4PI;				// Kr * 4 * PI
uniform float fKm4PI;				// Km * 4 * PI

uniform float fScale;				// 1 / (fOuterRadius - fInnerRadius)
uniform float fScaleDepth;		// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
uniform float fScaleOverScaleDepth;	// fScale / fScaleDepth

const int nSamples = 3;
const float fSamples = 3.0;

out vec3 v3Direction;
out vec3 RayleighColor;
out vec3 MieColor;

out float SunTheta;

float scale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

void main(void)
{

SunTheta = clamp(dot(normalize(v3LightPos),vec3(0,1,0)),0,1);

// Get the ray from the camera to the vertex, and its length (which is the far point of the ray passing through the atmosphere)
	vec3 v3Pos = vPosition.xyz*fOuterRadius;
	vec3 v3Ray = v3Pos - v3CameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;

		// Calculate the ray's starting position, then calculate its scattering offset
	vec3 v3Start = v3CameraPos;
	float fHeight = length(v3Start);
//	float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));//
	float fDepth = exp(fScaleOverScaleDepth * mix(0.082,0.0,smoothstep(0.07,0.165,SunTheta)));//0.139
	float fStartAngle = dot(v3Ray, v3Start) / fHeight;
	float fStartOffset = fDepth*scale(fStartAngle);

	// Initialize the scattering loop variables
	//gl_FrontColor = vec4(0.0, 0.0, 0.0, 0.0);
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	vec3 v3SampleRay = v3Ray * fSampleLength;
	vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	// Now loop through the sample rays
	vec3 v3FrontColor = vec3(0.0, 0.0, 0.0);
	for(int i=0; i<nSamples; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fLightAngle = dot(normalize(v3LightPos), v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
		vec3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}


	// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
	MieColor= v3FrontColor * fKmESun;
	RayleighColor = v3FrontColor * (v3InvWavelength * fKrESun);

	v3Direction = v3CameraPos - v3Pos;

	gl_Position = m4ModelViewProjX * vPosition;

}
