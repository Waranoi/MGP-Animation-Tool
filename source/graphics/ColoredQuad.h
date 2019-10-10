#pragma once
#include "../core/Window/Window.h"
#include <string>
#include "vmath.h"
#include <memory>

struct ColoredQuadObject
{
    GLuint vbo, ebo;
    ColoredQuadObject();
    ~ColoredQuadObject();

    // Rule of three, deleting copy constructor and copy assignment operator as a lazy solution to handling copying of allocated data
    ColoredQuadObject(const ColoredQuadObject& other) = delete; // copy constructor
    ColoredQuadObject& operator=(const ColoredQuadObject& other) = delete; // copy assignment

};

namespace ColoredQuad
{
    // texDim and cellDim are the dimensions of the texture and a single texture cell. cell is which cell of the texSource to display.
    std::shared_ptr<ColoredQuadObject> CreateQuad(Vector2i quadOrig, Vector2i quadSize);
    // Update your quad.
    void UpdateQuad(std::shared_ptr<ColoredQuadObject> quad);

    // Init before you draw any quads.
    void InitQuadDrawing();
    // Draw and bind your quads as needed.
    void BindQuad(std::shared_ptr<ColoredQuadObject> quad);
    void DrawQuad();
};