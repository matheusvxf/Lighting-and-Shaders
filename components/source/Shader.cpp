#include "Shader.h"
#include "main.h"

#include <fstream>

static char frag[] = "resources/shader.frag";
static char vertex[] = "resources/shader.vert";

Shader::Shader()
{
}

Shader::~Shader()
{
}

unsigned long Shader::getFileLength(std::ifstream& file)
{
    if (!file.good()) return 0;

    unsigned long pos = file.tellg();
    file.seekg(0, std::ios::end);
    unsigned long len = file.tellg();
    file.seekg(std::ios::beg);

    return len;
}

int Shader::loadshader(char* filename, GLchar** ShaderSource, unsigned long* len)
{
    std::ifstream file;
    file.open(filename, std::ios::in); // opens as ASCII!
    if (!file) return -1;

    *len = getFileLength(file);

    if (*len == 0) return -2;   // Error: Empty File 

    *ShaderSource = (GLchar*) new char[*len + 1];
    if (*ShaderSource == 0) return -3;   // can't reserve memory

    // len isn't always strlen cause some characters are stripped in ascii read...
    // it is important to 0-terminate the real length later, len is just max possible value... 
    (*ShaderSource)[*len] = 0;

    unsigned int i = 0;
    while (file.good())
    {
        (*ShaderSource)[i] = file.get();       // get character from file.
        if (!file.eof())
            i++;
    }

    (*ShaderSource)[i] = 0;  // 0-terminate it at the correct position

    file.close();

    return 0; // No Error
}

void Shader::unloadshader(GLubyte** ShaderSource)
{
    if (*ShaderSource != 0)
        delete[] * ShaderSource;
    *ShaderSource = 0;
}

void Shader::bind_shader()
{
    GLchar *VertexShaderSource, *FragmentShaderSource;
    GLint vlength, flength;

    loadshader(vertex, &VertexShaderSource, (unsigned long*)&vlength);
    loadshader(frag, &FragmentShaderSource, (unsigned long*)&flength);

    Globals::vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    Globals::fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSourceARB(Globals::vertexShaderObject, 1, (const GLchar**)&VertexShaderSource, &vlength);
    glShaderSourceARB(Globals::fragmentShaderObject, 1, (const GLchar**)&FragmentShaderSource, &flength);

    Shader::compile(Globals::vertexShaderObject, (UINT8*)"vertex");
    Shader::compile(Globals::fragmentShaderObject, (UINT8*)"fragment");
    Shader::link(Globals::vertexShaderObject, Globals::fragmentShaderObject, (UINT8*)"vertex and fragment");
}

void Shader::compile(GLuint shader_object, UINT8 *msg)
{
    GLint compiled;
    GLint blen = 0;
    GLsizei slen = 0;

    glCompileShaderARB(shader_object);

    glGetObjectParameterivARB(shader_object, GL_COMPILE_STATUS, &compiled);
    if (compiled)
    {
        std::cout << msg << " shader compiled!" << std::endl;
    }
    else
    {
        glGetShaderiv(shader_object, GL_INFO_LOG_LENGTH, &blen);
        if (blen > 1)
        {
            GLchar* compiler_log = (GLchar*)malloc(blen);
            glGetInfoLogARB(shader_object, blen, &slen, compiler_log);
            std::cout << "compiler_log:\n" << compiler_log;
            free(compiler_log);
        }
    }
}

void Shader::link(GLuint vertex_object, GLuint fragment_object, UINT8 *msg)
{
    GLint ProgramObject = glCreateProgram();
    GLint linked;
    GLint blen = 0;
    GLsizei slen = 0;

    glAttachShader(ProgramObject, vertex_object);
    glAttachShader(ProgramObject, fragment_object);

    glLinkProgramARB(ProgramObject);

    glGetProgramivARB(ProgramObject, GL_LINK_STATUS, &linked);
    if (linked)
    {
        std::cout << msg << " shader linked!" << std::endl;
        if (Globals::per_pixel_lighting)
            glUseProgram(ProgramObject);
        else
            glUseProgram(0);
    }
    else
    {
        glGetShaderiv(ProgramObject, GL_INFO_LOG_LENGTH, &blen);
        if (blen > 1)
        {
            GLchar* compiler_log = (GLchar*)malloc(blen);
            glGetInfoLogARB(ProgramObject, blen, &slen, compiler_log);
            std::cout << "compiler_log:\n" << compiler_log;
            free(compiler_log);
        }
    }
}