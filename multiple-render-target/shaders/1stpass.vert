#version 330

uniform mat4 mView, mProjection, mModel;

in vec4 vVertex;
out vec4 vVertexColor;

void main()
{
	gl_Position = mProjection * mView * mModel * vVertex;
	vVertexColor = mView * mModel * vVertex;
}