#version 330

layout(location = 0) out vec4 vFragColor0;
layout(location = 1) out vec4 vFragColor1;

in vec4 vVertexColor;

void main(void)
{
	vFragColor0 = vVertexColor;
	vFragColor1 = 1.0f - vVertexColor;
}