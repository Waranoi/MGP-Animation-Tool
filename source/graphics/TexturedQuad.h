#pragma once
#include "../core/Window/Window.h"
#include <string>
#include "vmath.h"

namespace TexturedQuad
{
    struct TexQuadObj
    {
        GLuint vbo, ebo, tex;
    };
    
    // texSource is the absolute path to the .png source that will be used as the texture.
    TexQuadObj CreateQuad(std::string texSource);
    // texOrig and texSize defines the piece of texSource to be displayed. Values are expected to be in pixel space. 
    TexQuadObj CreateQuad(std::string texSource, Vector2i texOrig, Vector2i texSize);
    // Update your quad.
    TexQuadObj UpdateQuad(TexQuadObj quad);

    // Init before you draw any quads.
    void InitQuadDrawing();
    // Draw and bind your quads as needed.
    void BindQuad(TexQuadObj quad);
    void DrawQuad();
};