#pragma once

#include "GLee.h"

#include <iostream>

class Shader
{
protected:
    static unsigned long getFileLength(std::ifstream& file);

public:
    Shader();
    ~Shader();
    
    static void bind_shader();
    static int loadshader(char* filename, GLchar** ShaderSource, unsigned long *len);
    static void unloadshader(GLubyte** ShaderSource);
    static void compile(GLuint shader_object, UINT8 *msg);
    static void link(GLuint vertex_object, GLuint fragment_object, UINT8 *msg);
};

