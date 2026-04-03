#version 460 core

out vec4 FragColor;
uniform samplerCube tSamplerReflection;
uniform samplerCube tSamplerRefraction;

in vec3 vsTexCoord;
in vec3 vsNormal;
in vec3 EyeVector;

void main(void)
{
	vec3 transformedNormal = normalize(vsNormal);
	vec3 transformedEyeVector = normalize(EyeVector);

	vec3 reflectedVector = reflect(-transformedEyeVector,transformedNormal);

	vec3 refractedVector =refract(transformedEyeVector,transformedNormal,1.0/1.33);

	vec4 RefractionFragColor =texture(tSamplerRefraction,vec3(-refractedVector.x,-refractedVector.y,refractedVector.z));
	vec4 ReflectionFragColor =texture(tSamplerRefraction,vec3(reflectedVector.x,-reflectedVector.y,reflectedVector.z));
	FragColor =mix(RefractionFragColor,ReflectionFragColor,0.55)*1.4;
}
