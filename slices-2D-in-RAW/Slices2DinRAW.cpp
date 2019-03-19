#include <iostream>
#include "volume.h"

#include "GL/glut.h"
#pragma comment(lib, "glut32.lib")

#define WINDOW_TITLE "Volume Slicing"
using namespace std;

int m_iWidth = 800;		//width of the viewport
int m_iHeight = 600;	//height of the viewport

CVolume m_volume;

enum Axis {Axial = 0, Coronal, Sagittal};

// variables to handle the slice
int m_arrCurrentSlice[] {0, 0, 0};	//integer to point the current slice
unsigned int m_arrTextureID[]{ 0, 0, 0 };	//opengl ID for texture

// Update the Axial texture invoking the getAxialSlice function from volume class
void changeTextureAxial(int iIndex)
{
	puchar slice = m_volume.getAxialSlice(m_arrCurrentSlice[Axial]);
	
	//bind the texture
	glBindTexture(GL_TEXTURE_2D, m_arrTextureID[Axial]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_volume.m_ncols, m_volume.m_nrows, 0, GL_RGB, GL_UNSIGNED_BYTE, slice);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	free(slice);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Update the Coronal texture invoking the getCoronalSlice function from volume class
void changeTextureCoronal(int iIndex)
{
	puchar slice = m_volume.getCoronalSlice(m_arrCurrentSlice[Coronal]);
	//create the texture
	glBindTexture(GL_TEXTURE_2D, m_arrTextureID[Coronal]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_volume.m_ncols, m_volume.m_nslices, 0, GL_RGB, GL_UNSIGNED_BYTE, slice);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	free(slice);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Update the Sagittal texture invoking the getSagittalSlice function from volume class
void changeTextureSagittal(int iIndex)
{
	puchar slice = m_volume.getSagittalSlice(m_arrCurrentSlice[Sagittal]);
	//create the texture
	glBindTexture(GL_TEXTURE_2D, m_arrTextureID[Sagittal]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_volume.m_nrows, m_volume.m_nslices, 0, GL_RGB, GL_UNSIGNED_BYTE, slice);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	free(slice);
	glBindTexture(GL_TEXTURE_2D, 0);
}

///
/// Function designed to be use in the glutKeyboardFunc
/// @param key represents the key pressed in the keyboard
/// 
void keyboardDown(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		changeTextureAxial(++m_arrCurrentSlice[Axial]);
		std::cout << "Axial slice: " << m_arrCurrentSlice[Axial] << " / " << m_volume.m_nslices << std::endl;
		glutPostRedisplay();
			break;
	case '2':
		changeTextureAxial(--m_arrCurrentSlice[Axial]);
		std::cout << "Axial slice: " << m_arrCurrentSlice[Axial] << " / " << m_volume.m_nslices << std::endl;
		glutPostRedisplay();
				break;
	case '3':
		changeTextureCoronal(++m_arrCurrentSlice[Coronal]);
		std::cout << "Coronal slice: " << m_arrCurrentSlice[Coronal] << " / " << m_volume.m_nrows << std::endl;
		glutPostRedisplay();
		break;
	case '4':
		changeTextureCoronal(--m_arrCurrentSlice[Coronal]);
		std::cout << "Coronal slice: " << m_arrCurrentSlice[Coronal] << " / " << m_volume.m_nrows << std::endl;
		glutPostRedisplay();
		break;
	case '5':
		changeTextureSagittal(++m_arrCurrentSlice[Sagittal]);
		std::cout << "Sagittal slice: " << m_arrCurrentSlice[Sagittal] << " / " << m_volume.m_ncols << std::endl;
		glutPostRedisplay();
		break;
	case '6':
		changeTextureSagittal(--m_arrCurrentSlice[Sagittal]);
		std::cout << "Sagittal slice: " << m_arrCurrentSlice[Sagittal] << " / " << m_volume.m_ncols << std::endl;
		glutPostRedisplay();
		break;
	case 'Q':
	case 'q':
	case  27:   // ESC
		exit(0);
	}
}

/// Function invoked on idle time of program
void idle(void)
{
	glutPostRedisplay();
}

/// All the drawing code here
void draw()
{
	glClear(GL_DEPTH_BUFFER_BIT);

	////////////////////////////////
	// from left to right, the axial
	glViewport(0, 0, m_iWidth / 3, m_iHeight);
	glScissor(0, 0, m_iWidth / 3, m_iHeight);
	glEnable(GL_SCISSOR_TEST);	// the scissor is only to obtain different background colors
	glClearColor(0.1, 0.1, 0.1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);	//removing the scissor, it is possible apply glClearColor to all viewport
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, m_arrTextureID[Axial]);
	//begin - rendering a simple quad with its texture coordinates
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -0.5, 0);

		glTexCoord2f(1, 0);
		glVertex3f(+1, -0.5, 0);

		glTexCoord2f(1, 1);
		glVertex3f(+1, +0.5, 0);

		glTexCoord2f(0, 1);
		glVertex3f(-1, +0.5, 0);
	glEnd();

	////////////////////////////////
	// from left to right, the coronal
	glViewport(m_iWidth / 3, 0, 1 * m_iWidth / 3, m_iHeight);
	glScissor(m_iWidth / 3, 0, 1 * m_iWidth / 3, m_iHeight);
	glEnable(GL_SCISSOR_TEST);
	glClearColor(0.2, 0.2, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, m_arrTextureID[Coronal]);
	//begin - rendering a simple quad with its texture coordinates
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -0.5, 0);

		glTexCoord2f(1, 0);
		glVertex3f(+1, -0.5, 0);

		glTexCoord2f(1, 1);
		glVertex3f(+1, +0.5, 0);

		glTexCoord2f(0, 1);
		glVertex3f(-1, +0.5, 0);
	glEnd();


	////////////////////////////////
	// from left to right, the coronal
	glViewport(2 * m_iWidth / 3, 0, 1 * m_iWidth / 3, m_iHeight);
	glScissor(2 * m_iWidth / 3, 0, 1 * m_iWidth / 3, m_iHeight);
	glEnable(GL_SCISSOR_TEST);
	glClearColor(0.3, 0.3, 0.3, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glBindTexture(GL_TEXTURE_2D, m_arrTextureID[Sagittal]);
	//begin - rendering a simple quad with its texture coordinates
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -0.5, 0);

		glTexCoord2f(1, 0);
		glVertex3f(+1, -0.5, 0);

		glTexCoord2f(1, 1);
		glVertex3f(+1, +0.5, 0);

		glTexCoord2f(0, 1);
		glVertex3f(-1, +0.5, 0);
	glEnd();

	glutSwapBuffers();
}

/// Is invoked when window is resized, modifying the projection matrix and viewport
void reshape(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	m_iWidth = width;
	m_iHeight = height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

/// Insert all your initialization objects and OpenGL status
bool initialize()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//read the volume
	string name = "CTHeadBabyClean256x256x94.raw";	//a raw binary-format volume
	if (!m_volume.readFile(name, 94, 256, 256))
		std::cout << "error opening the file" << std::endl;
	
	glGenTextures(1, &m_arrTextureID[Axial]);
	glGenTextures(1, &m_arrTextureID[Coronal]);
	glGenTextures(1, &m_arrTextureID[Sagittal]);

	changeTextureAxial(m_arrCurrentSlice[Axial]);
	changeTextureCoronal(m_arrCurrentSlice[Coronal]);
	changeTextureSagittal(m_arrCurrentSlice[Sagittal]);
	return true;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(m_iWidth, m_iHeight);
	glutCreateWindow(WINDOW_TITLE);
	if (!initialize()) return 1;
	glutDisplayFunc(draw);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardDown);
	glutMainLoop();
	return 0;
}