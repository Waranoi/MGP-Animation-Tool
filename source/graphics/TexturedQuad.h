#pragma once
#include "../core/Window/Window.h"
#include <string>
#include "vmath.h"
#include <memory>

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
    // texSource is the absolute path to the .png source that will be used as the texture.
    std::shared_ptr<TextureQuadObject> CreateQuad(std::string texSource);
    // texOrig and texSize defines the piece of texSource to be displayed. Values are expected to be in pixel space. 
    std::shared_ptr<TextureQuadObject> CreateQuad(std::string texSource, Vector2i texOrig, Vector2i texSize);
    // texDim and cellDim are the dimensions of the texture and a single texture cell. cell is which cell of the texSource to display.
    std::shared_ptr<TextureQuadObject> CreateQuad(std::string texSource, Vector2i texDim, Vector2i cellDim, int cell);
    // Update your quad.
    void UpdateQuad(std::shared_ptr<TextureQuadObject> quad);

    // Init before you draw any quads.
    void InitQuadDrawing();
    // Draw and bind your quads as needed.
    void BindQuad(std::shared_ptr<TextureQuadObject> quad);
    void DrawQuad();
};