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

std::shared_ptr<ColoredQuadObject> ColoredQuad::CreateQuad(Aabb3f quad)
{
    float quadVB[] = 
    {
		quad.min.x,	    quad.min.y,	    0,
		quad.min.x,	    quad.max.y,	    0,
		quad.max.x,	    quad.max.y,	    0,
		quad.max.x,	    quad.min.y,	    0,
    };
    int quadEB[] = { 0, 1, 2, 0, 2, 3 };

    ColoredQuadObject *newQuad = new ColoredQuadObject();

	glGenBuffers(1, &newQuad->vbo);   
	glBindBuffer(GL_ARRAY_BUFFER, newQuad->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVB), quadVB, GL_STATIC_DRAW);

    glGenBuffers(1, &newQuad->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newQuad->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadEB), quadEB, GL_STATIC_DRAW);

    return std::shared_ptr<ColoredQuadObject>(newQuad);
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
}

void ColoredQuad::BindQuad(std::shared_ptr<ColoredQuadObject> quad)
{
    if (!quad)
        return;

    // Order matters. Have to bind buffer before doing anything related to vertex attribs
    glBindBuffer(GL_ARRAY_BUFFER, quad->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad->ebo);

    // Set VAO (Vertex Attribute Object)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
}

void ColoredQuad::DrawQuad()
{
    glDrawElements(GL_TRIANGLES, QUAD_ELEMENT_COUNT, GL_UNSIGNED_INT, nullptr);
}