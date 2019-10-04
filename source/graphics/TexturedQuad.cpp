#define STB_IMAGE_IMPLEMENTATION
#include "TexturedQuad.h"
#include "Shaders.h"
#include "stb_image.h"

static const int QUAD_ELEMENT_COUNT = 6;

TextureQuadObject::TextureQuadObject()
{

}

TextureQuadObject::~TextureQuadObject()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &tex);
}

std::shared_ptr<TextureQuadObject> TexturedQuad::CreateQuad(std::string texSource)
{
    int comp, width, height;
    unsigned char *texData = stbi_load(texSource.c_str(), &width, &height, &comp, STBI_rgb_alpha);
    if (!texData)
    {
        printf("WARNING: Failed to create quad from source %s\n\n", texSource.c_str());
        return std::shared_ptr<TextureQuadObject>();
    }

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

    TextureQuadObject *newTexQuad = new TextureQuadObject();

	glGenBuffers(1, &newTexQuad->vbo);   
	glBindBuffer(GL_ARRAY_BUFFER, newTexQuad->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVB), quadVB, GL_STATIC_DRAW);

    glGenBuffers(1, &newTexQuad->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newTexQuad->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadEB), quadEB, GL_STATIC_DRAW);
    
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &newTexQuad->tex); 
    glBindTexture(GL_TEXTURE_2D, newTexQuad->tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

    stbi_image_free(texData);

    return std::shared_ptr<TextureQuadObject>(newTexQuad);
}

std::shared_ptr<TextureQuadObject> TexturedQuad::CreateQuad(std::string texSource, Vector2i texOrig, Vector2i texSize)
{
    int comp;
    Vector2i size;
    unsigned char *texData = stbi_load(texSource.c_str(), &size.x, &size.y, &comp, STBI_rgb_alpha);
    if (!texData)
    {
        printf("WARNING: Failed to create quad from source %s\n\n", texSource.c_str());
        return std::shared_ptr<TextureQuadObject>();
    }

    Vector2f sizeInv = Vector2f(1.0f/size.x, 1.0f/size.y);
    Vector2f texOrigNorm = sizeInv * texOrig;
    Vector2f texSizeNorm = sizeInv * texSize;
    Vector2f texBorderNorm = texOrigNorm + texSizeNorm;

    if (texOrigNorm.x < 0 || texOrigNorm.y < 0 || texOrigNorm.x > size.x || texOrigNorm.y > size.y ||
        texBorderNorm.x < 0 || texBorderNorm.y < 0 || texBorderNorm.x > size.x || texBorderNorm.y > size.y)
    {
        printf("WARNING: Texture coordinates extend beyond source image\n\n");
        return std::shared_ptr<TextureQuadObject>();
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

    TextureQuadObject *newTexQuad = new TextureQuadObject();

	glGenBuffers(1, &newTexQuad->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, newTexQuad->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVB), quadVB, GL_STATIC_DRAW);

    glGenBuffers(1, &newTexQuad->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newTexQuad->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadEB), quadEB, GL_STATIC_DRAW);
    
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &newTexQuad->tex); 
    glBindTexture(GL_TEXTURE_2D, newTexQuad->tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

    stbi_image_free(texData);

    return std::shared_ptr<TextureQuadObject>(newTexQuad);
}

std::shared_ptr<TextureQuadObject> TexturedQuad::CreateQuad(std::string texSource, Vector2i texDim, Vector2i cellDim, int cell)
{
    Vector2i texOrig(cell * cellDim.x % texDim.x, texDim.y-(cell * cellDim.x / texDim.x * cellDim.y)-cellDim.y);
    return CreateQuad(texSource, texOrig, cellDim);
}

void TexturedQuad::UpdateQuad(std::shared_ptr<TextureQuadObject> quad)
{
    if (!quad)
        return;

    // To be done, maybe
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

void TexturedQuad::BindQuad(std::shared_ptr<TextureQuadObject> quad)
{
    if (!quad)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, quad->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad->ebo);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, quad->tex);
}

void TexturedQuad::DrawQuad()
{
    glDrawElements(GL_TRIANGLES, QUAD_ELEMENT_COUNT, GL_UNSIGNED_INT, nullptr);
}