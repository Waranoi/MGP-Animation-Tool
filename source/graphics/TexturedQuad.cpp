#define STB_IMAGE_IMPLEMENTATION
#include "TexturedQuad.h"
#include "Shaders.h"
#include "stb_image.h"

static const int QUAD_ELEMENT_COUNT = 6;

// Invalid default value not set
static bool invalidSet = false;
// Invalid default value
static GLuint invalid;

TextureQuadObject::TextureQuadObject()
{

}

TextureQuadObject::~TextureQuadObject()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &tex);
}

TexturedQuad::TexQuadObj::TexQuadObj()
{
    // If invalid default value has not been set
    if (!invalidSet)
    {
        // Generate buffer value and set it as invalid default value
        invalidSet = true;
        glGenBuffers(1, &invalid);
    }
    
    // Set default value to be the same as the invalid default value
    vbo = ebo = tex = invalid;
}

TexturedQuad::TexQuadObj TexturedQuad::CreateQuad(std::string texSource)
{
    TexQuadObj newTexQuad;
    float quadVB[] = 
    {
		-1,	-1,	0,
        0, 0,
        
		-1,	1,	0,
        0, 1,	

		1,	1,	0,
        1, 1,	

		1,	-1,	0,
        1, 0
    };
    int quadEB[] = { 0, 1, 2, 0, 2, 3 };

	glGenBuffers(1, &newTexQuad.vbo);   
	glBindBuffer(GL_ARRAY_BUFFER, newTexQuad.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVB), quadVB, GL_STATIC_DRAW);

    glGenBuffers(1, &newTexQuad.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newTexQuad.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadEB), quadEB, GL_STATIC_DRAW);

    int comp, width, height;
    unsigned char *texData = stbi_load(texSource.c_str(), &width, &height, &comp, STBI_rgb_alpha);
    
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &newTexQuad.tex); 
    glBindTexture(GL_TEXTURE_2D, newTexQuad.tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

    return newTexQuad;
}

TexturedQuad::TexQuadObj TexturedQuad::CreateQuad(std::string texSource, Vector2i texOrig, Vector2i texSize)
{
    TexQuadObj newTexQuad;
    Vector2i size;
    Vector2f sizeInv, texOrigNorm, texSizeNorm, texBorderNorm;
    int comp;

    unsigned char *texData = stbi_load(texSource.c_str(), &size.x, &size.y, &comp, STBI_rgb_alpha);
    sizeInv = Vector2f(1.0f/size.x, 1.0f/size.y);
    texOrigNorm = sizeInv * texOrig;
    texSizeNorm = sizeInv * texSize;
    texBorderNorm = texOrigNorm + texSizeNorm;

    if (texOrigNorm.x < 0 || texOrigNorm.y < 0 || texOrigNorm.x > size.x || texOrigNorm.y > size.y ||
        texBorderNorm.x < 0 || texBorderNorm.y < 0 || texBorderNorm.x > size.x || texBorderNorm.y > size.y)
    {
        printf("WARNING: Texture coordinates extend beyond source image\n");
        return newTexQuad;
    }

    float quadVB[] = 
    {
		-1,	                            -1,                                 0,
        texOrigNorm.x,                  texOrigNorm.y,
        
		-1,	                            1,                                  0,
       texOrigNorm.x,                   texBorderNorm.y,	

		1,                              1,                                  0,
        texBorderNorm.x,                texBorderNorm.y,	

		1,	                            -1,	                                0,
        texBorderNorm.x,                texOrigNorm.y
    };
    int quadEB[] = { 0, 1, 2, 0, 2, 3 };

	glGenBuffers(1, &newTexQuad.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, newTexQuad.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVB), quadVB, GL_STATIC_DRAW);

    glGenBuffers(1, &newTexQuad.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newTexQuad.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadEB), quadEB, GL_STATIC_DRAW);
    
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &newTexQuad.tex); 
    glBindTexture(GL_TEXTURE_2D, newTexQuad.tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

    return newTexQuad;
}

TexturedQuad::TexQuadObj TexturedQuad::UpdateQuad(TexQuadObj quad)
{
    return quad;
}

bool TexturedQuad::IsValidTexQuad(TexQuadObj quad)
{
    // If quad values are the same as the invalid default value then the quad is invalid
    return quad.vbo != invalid && quad.ebo != invalid && quad.tex != invalid;
}

void TexturedQuad::InitQuadDrawing()
{
    // Use the right shader
    glUseProgram(Shaders::texQuadProgram);

    // Set VAO (Vertex Attribute Object)
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));
}

void TexturedQuad::BindQuad(TexQuadObj quad)
{
    glBindBuffer(GL_ARRAY_BUFFER, quad.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.ebo);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, quad.tex);
}

void TexturedQuad::DrawQuad()
{
    glDrawElements(GL_TRIANGLES, QUAD_ELEMENT_COUNT, GL_UNSIGNED_INT, nullptr);
}