#version 460 core

out vec4 FragColor;

uniform sampler2D tSamplerSceneMask;
uniform sampler2D tSamplerCloudMask;

in vec2 TexCoord;

void main(void)
{
	float SceneMask=1.0-texture(tSamplerSceneMask,TexCoord).r;
	float CloudMask=1.0-texture(tSamplerCloudMask,TexCoord).a;

	FragColor = vec4(SceneMask*CloudMask,0,0,1);
}
