#version 330

//in variables
in vec4 vVertexColor;
smooth in vec2 vTexCoordFrag;

//out variables
out vec4 vFragColor;
uniform sampler2D sTexSampler0;

void main(void)
{
	//vFragColor = vec4(vTexCoordFrag,1,1);
	vFragColor = texture(sTexSampler0, vTexCoordFrag);
	//vFragColor = vec4(vTexCoordFrag.st,1, 1);
}