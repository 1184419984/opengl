#pragma once
#include <GL/glew.h>

class ShaderHandle {
public:
	ShaderHandle(const char* vertexPath, const char* fragmentPath);
	void use();
	GLuint getProgram() { return m_program; }

private:
	GLuint m_program;
};