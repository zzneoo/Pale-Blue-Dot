#version 460 core
		
in vec2 TexCoord;
uniform sampler2D tSampler_SingleBillboard;
uniform float fIntensity;
out vec4 Frag_Color;

void main(void)
{

	vec4 billboard = texture(tSampler_SingleBillboard,TexCoord);
		
	Frag_Color = vec4(mix(vec3(0.0),billboard.rgb*fIntensity,billboard.a),1.0);

}
