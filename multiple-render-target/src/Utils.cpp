#include "Utils.h"
#include "../GL/glew.h"
#include "../include/FreeImage.h"
#include <string>
#include <iostream>
using namespace std;
#pragma comment(lib, "FreeImage")
//#define printOpenGLError() printOglError(__FILE__, __LINE__)

CUtils* CUtils::m_pInstance = 0;

CUtils* CUtils::getInstance()
{
	if(m_pInstance == 0)
		m_pInstance = new CUtils;
	return m_pInstance;
}

CUtils::CUtils()
{
	m_uIdTexture = 0;
}

uchar* CUtils::getBytesFromTexture(std::string strFilename)
{
	FIBITMAP* data = FreeImage_Load(FreeImage_GetFIFFromFilename(strFilename.c_str()), strFilename.c_str());
	BYTE* bytes = (BYTE*)FreeImage_GetBits(data);
	//uchar* bytesToRet = bytes;
	//bytes = NULL;
	//FreeImage_Unload(data);
	//return bytesToRet;
	return bytes;
}

uchar* CUtils::getInfoFromTexture(std::string strFilename, int* iWidth, int* iHeight)
{
	FIBITMAP* data = FreeImage_Load(FreeImage_GetFIFFromFilename(strFilename.c_str()), strFilename.c_str());
	*iWidth = FreeImage_GetWidth(data);
	*iHeight = FreeImage_GetHeight(data);
	BYTE* pointer = (uchar*)FreeImage_GetBits(data);
	return pointer;
}

bool CUtils::setOpenGLExt()
{
	glewInit();
	
	if (glewIsSupported("GL_VERSION_2_0"))
		cout << "Vertex & Fragment shaders are supported (OpenGL 2.0 ready)" << endl;
	else
	{
		if(GLEW_ARB_vertex_shader)
			cout << "Vertex shader is supported." << endl;
		else
			cout << "Vertex shader is not supported." << endl;
		
		if(GLEW_ARB_fragment_shader)
			cout << "Fragment shader is supported." << endl;
		else
			cout << "Fragment shader is not supported." << endl;
	}

	if(GLEW_ARB_geometry_shader4)
		cout << "Geometry shader4 is supported." << endl;
	else
		cout << "Geometry shader4 is not supported." << endl;

	if(GLEW_ARB_tessellation_shader)
		cout << "Tessellation shader is supported." << endl;
	else
		cout << "Tessellation shader4 is not supported." << endl;
	
	if(GLEW_ARB_texture_cube_map_array)
		cout << "GL_TEXTURE_CUBE_MAP_ARRAY supported" << endl;

	if(GLEW_ARB_framebuffer_object)
		cout << "GL_FRAMEBUFFEROBJECT supported" << endl;


	if (glewIsSupported("GL_VERSION_3_3"))
	{
		cout << "Ready for OpenGL 3.3." << endl;
		return true;
	}
	else
	{
		cout << "OpenGL 3.3. is not supported." << endl;
		//return false;
	}
	
	return true;
}

//int printOglError(char *file, int line)
//{
//    //
//    // Returns 1 if an OpenGL error occurred, 0 otherwise.
//    //
//    GLenum glErr;
//    int    retCode = 0;
// 
//    glErr = glGetError();
//    while (glErr != GL_NO_ERROR)
//    {
//        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
//        retCode = 1;
//        glErr = glGetError();
//    }
//    return retCode;
//}

/*
glm::mat4x4 matrixLookAt(glm::vec3 eye, glm::vec3 at, glm::vec3 up)
{
	glm::mat4x4 matrix(1.0);
	glm::vec3 f = glm::normalize(at - eye);
	up = glm::normalize(up);
	glm::vec3 s = glm::cross(f, up);
	glm::vec3 u = glm::cross(s, f);
	matrix = glm::mat4x4(glm::vec4(s,0),glm::vec4(u,1),glm::vec4(-f,0),glm::vec4(0,0,0,1));
	matrix = glm::inverse(matrix);//format to OGL1
	matrix = matrix * glm::translate(glm::mat4x4(1.0f), -eye);
	return matrix;
}
*/

/*
void FrameToPPM( const char *f, unsigned char *data, int width, int height )
{
  FILE *out = fopen(f, "wb");
  if (!out) 
  {
	  fprintf( stderr, "***Error: Unable to capture frame.  fopen() failed!!\n");
	  return;
  }
  
  fprintf(out, "P6\n# File captured by Chris Wyman's OpenGL framegrabber\n");  
  fprintf(out, "%d %d\n", width, height);
  fprintf(out, "%d\n", 255); 
  
  for ( int y = height-1; y >= 0; y-- )
	  fwrite( data+(3*y*width), 1, 3*width, out );

  fprintf(out, "\n");
  fclose(out);
}

int flag = true;
void saveFile(int w,int h,std::string str)
{
	int bytes = w*h*3;
  //GLubyte *buffer = (GLubyte*)malloc(bytes);
	uchar *buffer = (uchar*)malloc(bytes);
	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	//glReadPixels(0, 0, w, h, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, buffer);
	//glReadPixels(0, 0, w, h, GL_DEPTH_STENCIL, GL_UNSIGNED_INT, buffer);
	FrameToPPM(str.c_str(), buffer, 512, 512);
	//freopen(str.c_str(), "w", stdout);
	//for(int k = 0; k < bytes; k++)
	//	//std::cout << buffer[k];
	//	//printf("%c",buffer[k]);
	//	putchar(buffer[k]);
	//fclose (stdout);
}
*/