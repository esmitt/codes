#include "GL/glew.h"	//this line is mandatory at first place!
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "src/Utils.h"
#include "src/GLSLProgram.h"
#include <stdio.h>
#include <iostream>
using namespace std;

//debug
#include "include/imdebug.h"
#include "include/imdebuggl.h"
#pragma comment(lib, "imdebug.lib")

#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32.lib")

const int ENVIRONMENT = 0;
const int OBJECT = 1;
const float ANGLE = 45.f;
const float FOV = 90.f;
const float NCP = 0.01f;
const float FCP = 20.f;
#define WIDTH_CUBE_TEXTURE 512
#define HEIGHT_CUBE_TEXTURE 512

//parameters
float m_gfRatio = 1.0f;
int m_iWidth = 1;		//width of the viewport
int m_iHeight = 1;		//height of the viewport

//matrices
glm::mat4x4 m_projMatrix; // Store the projection matrix
glm::mat4x4 m_viewMatrix;	 // Store the view matrix  
glm::mat3x3 m_normalMatrix;	 // Store the normal matrix in eye space
glm::mat4 m_modelMatrix;	// Store the model matrix 
glm::vec4 eye;	//eye's position

CGLSLProgram m_MRTProgram;
CGLSLProgram m_SecondPassProgram;
CGLSLProgram m_Environment;

//six layers
const int NUMBER_OF_CUBE_TEXTURES = 6;
const int NUMBER_OF_FACES_OF_CUBE = 6;

uint m_uFBOname;
uint m_uRendername[2];
uint cubemap[2];

void initialize()
{
	if(!CUtils::getInstance()->setOpenGLExt())
	{
		std::cout << "Sorry, your machine not support the extensions" << std::endl;
		exit(0);
	}
	glEnable(GL_DEPTH_TEST);
	m_MRTProgram.loadShader("shaders/1stpass.vert", VERTEX);
	m_MRTProgram.loadShader("shaders/1stpass.frag", FRAGMENT);
	m_SecondPassProgram.loadShader("shaders/2ndpass.vert", VERTEX);
	m_SecondPassProgram.loadShader("shaders/2ndpass.frag", FRAGMENT);
	m_Environment.loadShader("shaders/environment.vert", VERTEX);
	m_Environment.loadShader("shaders/environment.frag", FRAGMENT);

	//m_MRTProgram.create_link();
	m_MRTProgram.create();
	glBindFragDataLocation(m_MRTProgram.getId(), 0, "vFragColor0");
	glBindFragDataLocation(m_MRTProgram.getId(), 1, "vFragColor1");
	m_MRTProgram.link();
	m_MRTProgram.enable();
		m_MRTProgram.addAttribute("vVertex");
		m_MRTProgram.addUniform("mProjection");
		m_MRTProgram.addUniform("mModel");
		m_MRTProgram.addUniform("mView");
	m_MRTProgram.disable();

	m_Environment.create();
	//glBindFragDataLocation(m_Environment.getId(), 0, "vFragColor0");
	//glBindFragDataLocation(m_Environment.getId(), 1, "vFragColor1");
	m_Environment.link();
	m_Environment.enable();
		m_Environment.addAttribute("vVertex");
		m_Environment.addUniform("mProjection");
		m_Environment.addUniform("mModel");
		m_Environment.addUniform("mView");
		m_Environment.addUniform("sampler");
		//glUniform1i(m_Environment.getLocation("sampler"), 1);
	m_Environment.disable();
	

	m_SecondPassProgram.create_link();
	m_SecondPassProgram.enable();
//		m_SecondPassProgram.addAttribute("vVertex");
		m_SecondPassProgram.addUniform("mProjection");
		m_SecondPassProgram.addUniform("mModel");
		m_SecondPassProgram.addUniform("mView");
	m_SecondPassProgram.disable();

	
	/////////
	int k;
	glGenTextures(2, cubemap);
	
//	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[0]);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	_ASSERT(glGetError() == GL_NO_ERROR);
	for (k = 0; k < 6; k++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + k, 0, GL_RGBA8,
		WIDTH_CUBE_TEXTURE, HEIGHT_CUBE_TEXTURE, 0, GL_RGBA, GL_FLOAT, NULL);

//	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[1]);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	_ASSERT(glGetError() == GL_NO_ERROR);
	for (k = 0; k < 6; k++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + k, 0, GL_RGBA8,
		WIDTH_CUBE_TEXTURE, HEIGHT_CUBE_TEXTURE, 0, GL_RGBA, GL_FLOAT, NULL);

	////////////////////////////

	glGenFramebuffers(1, &m_uFBOname);
	//glGenRenderbuffers(2, m_uRendername);
	//
	//glBindRenderbuffer(GL_RENDERBUFFER, m_uRendername[0]);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, WIDTH_CUBE_TEXTURE, HEIGHT_CUBE_TEXTURE);
	//
	//glBindRenderbuffer(GL_RENDERBUFFER, m_uRendername[1]);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, WIDTH_CUBE_TEXTURE, HEIGHT_CUBE_TEXTURE);

	glBindFramebuffer(GL_FRAMEBUFFER, m_uFBOname);
	GLenum draws [2] ={GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_uRendername[0]);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, m_uRendername[1]);
	glDrawBuffers(2, draws);

	for (k = 0; k < 6; k++)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[0]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + k, cubemap[0], 0);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[1]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_CUBE_MAP_POSITIVE_X + k, cubemap[1], 0);
	}
	
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[0]);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cubemap[0], 0);

	//glActiveTexture(GL_TEXTURE0 + 1);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[1]);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, cubemap[1], 0);

	GLenum fbok = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  _ASSERT(fbok == GL_FRAMEBUFFER_COMPLETE);
	_ASSERT(glGetError() == GL_NO_ERROR);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Terminate()
{
	std::cout << "terminate function." << std::endl;
	glDeleteFramebuffers(1, &m_uFBOname);
	glDeleteTextures(2, cubemap);
}


void reshape(int w, int h)
{
	if(h == 0)h = 1;
	m_gfRatio = float(w) / float(h);
	m_iWidth = w;
	m_iHeight = h;
	glViewport(0, 0, w, h);
	m_projMatrix = glm::perspective(ANGLE, m_gfRatio, NCP, FCP);
	m_MRTProgram.enable();
		glUniformMatrix4fv(m_MRTProgram.getLocation("mProjection"), 1, GL_FALSE, &m_projMatrix[0][0]);
	m_MRTProgram.disable();
	m_SecondPassProgram.enable();
		glUniformMatrix4fv(m_SecondPassProgram.getLocation("mProjection"), 1, GL_FALSE, &m_projMatrix[0][0]);
	m_SecondPassProgram.disable();
	
}
float f = 0;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.0,0.0,0.5,1);
	m_modelMatrix= glm::mat4x4(1.0);
	m_viewMatrix = glm::translate(glm::mat4x4(1.0), glm::vec3(0,0,-0.5)) * glm::rotate(glm::mat4x4(1.0), f, glm::vec3(1,0,0));
	f+= 1.5;
	if(f > 360) f -= 360;
	//
	//m_MRTProgram.enable();
	//	glutSolidTeapot(0.15);
	//	glUniformMatrix4fv(m_MRTProgram.getLocation("mView"), 1, GL_FALSE, &m_viewMatrix[0][0]);		
	//	glUniformMatrix4fv(m_MRTProgram.getLocation("mModel"), 1, GL_FALSE, &m_modelMatrix[0][0]);
	//	glUniformMatrix4fv(m_MRTProgram.getLocation("mProjection"), 1, GL_FALSE, &m_projMatrix[0][0]);
	////end
	//m_MRTProgram.disable();
	
	int k;
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_uFBOname);	
	glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT);
	//glPushAttrib(GL_ENABLE_BIT);
	//glPushAttrib(GL_CURRENT_BIT);
	glViewport(0, 0, WIDTH_CUBE_TEXTURE, HEIGHT_CUBE_TEXTURE);


	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[0]);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cubemap[0], 0);
	//for(k = 0; k < 6; k++)
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + k, cubemap[0], 0);

	//glActiveTexture(GL_TEXTURE0 + 1);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[1]);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, cubemap[1], 0);
	//for(k = 0; k < 6; k++)
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_CUBE_MAP_POSITIVE_X + k, cubemap[1], 0);
	
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_uRendername[0]);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, m_uRendername[1]);

	for(int k = 0; k < 6; k++)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[0]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + k, cubemap[0], 0);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[1]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_CUBE_MAP_POSITIVE_X + k, cubemap[1], 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//glClearColor(0.5,0,0,1);
		m_MRTProgram.enable();
			//glutWireTeapot(0.15);
			//glutSolidTeapot(0.15);
			glutSolidSphere(2.15,20,19);	
			glUniformMatrix4fv(m_MRTProgram.getLocation("mView"), 1, GL_FALSE, &m_viewMatrix[0][0]);
			glUniformMatrix4fv(m_MRTProgram.getLocation("mModel"), 1, GL_FALSE, &m_modelMatrix[0][0]);
			glUniformMatrix4fv(m_MRTProgram.getLocation("mProjection"), 1, GL_FALSE, &m_projMatrix[0][0]);
		m_MRTProgram.disable();
	}
	//glDisable(GL_TEXTURE_CUBE_MAP);
	glPopAttrib();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//glActiveTexture(GL_TEXTURE0 + 1);
	//glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, cubemap[1]);
	//imdebugTexImagef(GL_TEXTURE_CUBE_MAP_POSITIVE_X, cubemap[1], GL_RGB);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, cubemap[0]);
	//imdebugTexImagef(GL_TEXTURE_CUBE_MAP_POSITIVE_X, cubemap[0], GL_RGB);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_iWidth, m_iHeight);
	
	m_viewMatrix = glm::translate(glm::mat4x4(1.0), glm::vec3(-0.15,0,-1.0)) * glm::rotate(glm::mat4x4(1.0), f, glm::vec3(1,0,0));
	//environment
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[0]);
	m_Environment.enable();
			glutSolidSphere(0.15,20,19);
			glUniformMatrix4fv(m_Environment.getLocation("mView"), 1, GL_FALSE, &m_viewMatrix[0][0]);
			glUniformMatrix4fv(m_Environment.getLocation("mModel"), 1, GL_FALSE, &m_modelMatrix[0][0]);
			glUniformMatrix4fv(m_Environment.getLocation("mProjection"), 1, GL_FALSE, &m_projMatrix[0][0]);
			glUniform1i(m_Environment.getLocation("sampler"), 0);
		m_Environment.disable();

	m_viewMatrix = glm::translate(glm::mat4x4(1.0), glm::vec3(0.15,0,-1.0)) * glm::rotate(glm::mat4x4(1.0), f, glm::vec3(1,0,0));
	//environment
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[1]);
	m_Environment.enable();
			glutSolidSphere(0.15,20,19);
			glUniformMatrix4fv(m_Environment.getLocation("mView"), 1, GL_FALSE, &m_viewMatrix[0][0]);
			glUniformMatrix4fv(m_Environment.getLocation("mModel"), 1, GL_FALSE, &m_modelMatrix[0][0]);
			glUniformMatrix4fv(m_Environment.getLocation("mProjection"), 1, GL_FALSE, &m_projMatrix[0][0]);
			glUniform1i(m_Environment.getLocation("sampler"), 1);
		m_Environment.disable();
	
		
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glutSwapBuffers();
  glutPostRedisplay();
}

	//float kk = 0.25f;
	//float d = 0.75f;

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap[0]);
	//glBegin(GL_QUADS);
	//	//glMultiTexCoord3f(GL_TEXTURE0, -1,-1,1);
	//	glTexCoord3f(0,0,0);
	//	glVertex3f(-kk + d, -kk+ d, -1);

	//	glTexCoord3f(0,1,0);
	//	//glMultiTexCoord3f(GL_TEXTURE0, -1,-1,-1);
	//	glVertex3f(kk+ d, -kk+ d, -1);

	//	glTexCoord3f(0,1,1);
	//	//glMultiTexCoord3f(GL_TEXTURE0, -1,-1,-1);
	//	glVertex3f(kk+ d, kk+ d, -1); 
	//
	//	glTexCoord3f(0,0,1);
	//	//glMultiTexCoord3f(GL_TEXTURE0, -1,1,1);
	//	glVertex3f(-kk+ d, kk+ d, -1);
	//glEnd();
	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0); 

	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_TEXTURE_CUBE_MAP);


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(800,600);
	glutCreateWindow("GLUTTemplate");	
	glutCreateMenu(NULL);
	initialize();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//GUI construction
  glutCloseFunc(Terminate) ;
	glutMainLoop();
	return 0;
}
