#include "ColoredQuad.h"
#include "Shaders.h"

static const int QUAD_ELEMENT_COUNT = 6;

ColoredQuadObject::ColoredQuadObject()
{

}

ColoredQuadObject::~ColoredQuadObject()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

std::shared_ptr<ColoredQuadObject> ColoredQuad::CreateQuad(Vector2i quadOrig, Vector2i quadSize)
{
    return std::shared_ptr<ColoredQuadObject>();
}

void ColoredQuad::UpdateQuad(std::shared_ptr<ColoredQuadObject> quad)
{
    if (!quad)
        return;

    // To be done, maybe
}

void ColoredQuad::InitQuadDrawing()
{
    // Use the right shader
    glUseProgram(Shaders::colQuadProgram);

    // Set VAO (Vertex Attribute Object)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
}

void ColoredQuad::BindQuad(std::shared_ptr<ColoredQuadObject> quad)
{
    if (!quad)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, quad->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad->ebo);
}

void ColoredQuad::DrawQuad()
{
    glDrawElements(GL_TRIANGLES, QUAD_ELEMENT_COUNT, GL_UNSIGNED_INT, nullptr);
}