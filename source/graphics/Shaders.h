#pragma once
#include "../core/Window/Window.h"

namespace Shaders
{
    extern GLuint texQuadProgram, colQuadProgram;

    static const GLchar* texQuadVS =
	"#version 430\n"
	"layout(location=0) in vec3 pos;\n"
	"layout(location=1) in vec2 uv;\n"
	"layout(location=0) out vec2 UV;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(pos, 1);\n"
	"	UV.x = uv.x;\n"
	"	UV.y = -uv.y;\n"
	"}\n";
    static const GLchar* texQuadFS =
	"#version 430\n"
	"uniform sampler2D tex;\n"
	"layout(location=0) in vec2 uv;\n"
	"out vec4 out_color;\n"
	"void main()\n"
	"{\n"
	"	out_color = texture(tex, uv);\n"
	"}\n";

    static const GLchar* colBoxVS =
    "#version 430\n"
    "layout(location=0) in vec3 pos;\n"
    "void main()\n"
    "{\n"
    "	gl_Position = vec4(pos, 1);\n"
    "}\n";
    static const GLchar* colBoxFS =
    "#version 430\n"
    "out vec3 out_color;\n"
    "void main()\n"
    "{\n"
    "	out_color = vec3(1,0,0);\n"
    "}\n";

    void ShaderErrorLog(const char* msg, GLuint shader);
    void ProgramErrorLog(const char* msg, GLuint program);
    GLuint SetupVertexShader(const GLchar* vs);
    GLuint SetupFragmentShader(const GLchar* fs);
    GLuint SetupProgram(GLuint vs_int, GLuint fs_int);
    void SetupShaders();
};