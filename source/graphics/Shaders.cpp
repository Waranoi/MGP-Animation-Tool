#include "Shaders.h"

GLuint Shaders::colQuadProgram = 0;
GLuint Shaders::texQuadProgram = 0;

void Shaders::ShaderErrorLog(const char* msg, GLuint shader)
{
    GLint log_length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 1)
    {
        GLchar* buf = new GLchar[log_length];
        glGetShaderInfoLog(shader, log_length, NULL, buf);
        printf("%s %s %d\n", msg, buf, log_length);
        delete[] buf;
    }
}

void Shaders::ProgramErrorLog(const char* msg, GLuint program)
{
    GLint log_length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 1)
    {
        GLchar* buf = new GLchar[log_length];
        glGetProgramInfoLog(program, log_length, NULL, buf);
        printf("%s %s \n", msg, buf);
        delete[] buf;
    }
}

GLuint Shaders::SetupVertexShader(const GLchar* vs)
{
    GLuint vs_int = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs_int, 1, &vs, NULL);
    glCompileShader(vs_int);
    ShaderErrorLog("[VERTEX SHADER COMPILE ERROR]:", vs_int);
    return vs_int;
}

GLuint Shaders::SetupFragmentShader(const GLchar* fs)
{
    GLuint fs_int = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs_int, 1, &fs, NULL);
    glCompileShader(fs_int);
    ShaderErrorLog("[FRAGMENT SHADER COMPILE ERROR]:", fs_int);
    return fs_int;
}

GLuint Shaders::SetupProgram(GLuint vs_int, GLuint fs_int)
{
    //Create a program object
    GLuint program = glCreateProgram();
    glAttachShader(program, vs_int);
    glAttachShader(program, fs_int);
    glLinkProgram(program);
    ProgramErrorLog("[PROGRAM LINK ERROR]:", program);
    return program;
}

void Shaders::SetupShaders()
{
    GLuint vs_int, fs_int;

    // Setup textured quad program
    vs_int = SetupVertexShader(texQuadVS);
    fs_int = SetupFragmentShader(texQuadFS);
    texQuadProgram = SetupProgram(vs_int, fs_int);

    // Setup hitbox program
    vs_int = SetupVertexShader(colBoxVS);
    fs_int = SetupFragmentShader(colBoxFS);
    colQuadProgram = SetupProgram(vs_int, fs_int);
}