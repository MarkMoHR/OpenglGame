#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <gl/glew.h>

class Shader
{
public:
	GLuint Program;

    Shader();

    void load(const GLchar* vertexPath, const GLchar* fragmentPath);

	void Use();
};
