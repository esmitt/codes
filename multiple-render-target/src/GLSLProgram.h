#pragma once

#include "../GL/glew.h"
#include "../GL/freeglut.h"
#include <gl\GL.h>
#include <string>
#include <map>

typedef enum {VERTEX = 0, FRAGMENT, GEOMETRY, TESSELATION} SHADERTYPE;

/// For now, just 1 vertex's type can be attach to the program
/// In order add more than 1 vertex's type to the program, the shaders should be named (eg. "shader1", "shader2" & so on)
class CGLSLProgram
{
public:
	GLuint m_uIdProgram;		//id of the program
	GLuint m_vIdShader[4];	//ids of the loaded shaders; the 4th is empty always

	CGLSLProgram(void);
	~CGLSLProgram(void);

	void loadShader(std::string strFileName, SHADERTYPE typeShader);
	void create();
	void create_link();
	void link();
	void enable();
	void disable();
	void addAttribute(std::string strParName);
	void addUniform(std::string strParName);
	GLint getLocation(std::string strParName);
	GLuint getId();
private:
	std::map<std::string, GLint> m_mapVarShader;
	bool loadShaderFile(std::string strFilename, GLuint iHandle);
	void checkLinkingErrors();
};

