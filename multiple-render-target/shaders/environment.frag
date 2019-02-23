#version 330
layout(location = 0) out vec4 vFragColor;

uniform samplerCube sampler;
in vec3 vVertexColor;

void main(void)
{
	//vFragColor = vec4(vVertexColor,1);
	vFragColor = texture(sampler, vVertexColor);
}