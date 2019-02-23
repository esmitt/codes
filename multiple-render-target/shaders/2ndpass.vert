#version 330

uniform mat4 mView, mProjection, mModel;

in vec4 vVertex;
in vec2 vTexCoord;

out vec4 vVertexColor;
smooth out vec2 vTexCoordFrag;

void main()
{
	vVertexColor = normalize(vec4(vTexCoord,1,1));
	//vVertexColor = normalize(vVertex);
	gl_Position = mProjection * mView * mModel * vVertex;
	vTexCoordFrag = vTexCoord;
}