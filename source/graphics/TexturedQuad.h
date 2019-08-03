#pragma once
#include "../core/Window/Window.h"
#include <string>

namespace TexturedQuad
{
    struct TexQuadObj
    {
        GLuint vbo, ebo, tex;
    };

    TexQuadObj CreateQuad(std::string texSource);
    void UpdateQuad();

    void InitQuadDrawing();
    void BindQuad(TexQuadObj quad);
    void DrawQuad();
};