#version 460 core

out vec4 FragColor;

uniform sampler2D tSampler;
uniform float fBlendFactor;

in vec2 TexCoord;

void main(void)
{
	vec4 scene=texture(tSampler,TexCoord);


	FragColor = mix(scene,vec4(0.0,0.0,0.0,scene.a),fBlendFactor);
}
