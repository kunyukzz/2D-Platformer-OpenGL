#pragma once

#include "vertex.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

enum class GlyphSortType
{
    NONE,
    FRONT_TO_BACK,
    BACK_TO_FRONT,
    TEXTURE
};

class Glyph
{
  public:
    Glyph(){};
    Glyph(const glm::vec4& desRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const Color& color)
        : texture(Texture)
        , depth(Depth)
    {
        // JUST FOR REMINDER, OPENGL WINDING SYSTEM WAS ANTI-CLOCKWISE!!!! ----------------------------------------------------------
        // SO DON'T EVER PUT RANDOM ORDER TO DEFINE VERTEX BUFFER!!! ----------------------------------------------------------------

        // top left vertex
        topLeft.color = color;
        topLeft.setPos(desRect.x, desRect.y + desRect.w);
        topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

        // bottom left vertex
        bottomLeft.color = color;
        bottomLeft.setPos(desRect.x, desRect.y);
        bottomLeft.setUV(uvRect.x, uvRect.y);

        // top right vertex
        bottomRight.color = color;
        bottomRight.setPos(desRect.x + desRect.z, desRect.y);
        bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

        // bottom right vertex
        topRight.color = color;
        topRight.setPos(desRect.x + desRect.z, desRect.y + desRect.w);
        topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
    };

    GLuint texture;
    float depth;

    Vertex topLeft;
    Vertex bottomLeft;
    Vertex bottomRight;
    Vertex topRight;
};

class RenderBatch
{
  public:
    RenderBatch(GLuint offSet, GLuint NUMVertices, GLuint Txture)
        : offset(offSet)
        , numVertices(NUMVertices)
        , texture(Txture){};

    GLuint offset;
    GLuint numVertices;
    GLuint texture;
};

class SpriteBatch
{
    void createRenderBatch();
    void createVertexArray();
    void sortGlyphs();

    static bool compareFrontToBack(Glyph* a, Glyph* b);
    static bool compareBackToFront(Glyph* a, Glyph* b);
    static bool compareTexture(Glyph* a, Glyph* b);

    GLuint m_vbo;
    GLuint m_vao;

    GlyphSortType m_sortType;

    std::vector<Glyph*> m_glyphPointer; // this is for sorting only
    std::vector<Glyph> m_glyph;         // this is for actual glyps
    std::vector<RenderBatch> m_renderBatch;

  public:
    SpriteBatch();
    ~SpriteBatch();

    void init();
    void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
    void end();
    void draw(const glm::vec4& desRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color);
    void renderBatch();
};
