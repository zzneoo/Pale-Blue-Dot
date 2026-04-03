#version 460 core

out vec4 FragColor;
uniform samplerCube tSamplerCube;

in vec3 vsTexCoord;

void main(void)
{
	FragColor =texture(tSamplerCube,vec3(vsTexCoord.x,1.0-vsTexCoord.y,vsTexCoord.z));
//	FragColor =vec4(1);
}
