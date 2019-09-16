#pragma once
#include "../core/Window/Window.h"
#include <string>
#include "vmath.h"

struct TextureQuadObject
{
    GLuint vbo, ebo, tex;
    TextureQuadObject();
    ~TextureQuadObject();

    // Rule of three, deleting copy constructor and copy assignment operator as a lazy solution to handling copying of allocated data
    TextureQuadObject(const TextureQuadObject& other) = delete; // copy constructor
    TextureQuadObject& operator=(const TextureQuadObject& other) = delete; // copy assignment

};

namespace TexturedQuad
{
    struct TexQuadObj
    {
        GLuint vbo, ebo, tex;
        TexQuadObj();
    };
    
    // texSource is the absolute path to the .png source that will be used as the texture.
    TexQuadObj CreateQuad(std::string texSource);
    // texOrig and texSize defines the piece of texSource to be displayed. Values are expected to be in pixel space. 
    TexQuadObj CreateQuad(std::string texSource, Vector2i texOrig, Vector2i texSize);
    // Update your quad.
    TexQuadObj UpdateQuad(TexQuadObj quad);
    // Check if quad is valid.
    bool IsValidTexQuad(TexQuadObj quad);

    // Init before you draw any quads.
    void InitQuadDrawing();
    // Draw and bind your quads as needed.
    void BindQuad(TexQuadObj quad);
    void DrawQuad();

    // 
};