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

struct Glyph
{
    GLuint texture;
    float depth;

    Vertex topLeft;
    Vertex bottomLeft;
    Vertex topRight;
    Vertex bottomRight;
};

class RenderBatch
{
  public:
    RenderBatch(GLuint offSet, GLuint NUMVertices, GLuint Txture)
        : offset(offSet)
        , numVertices(NUMVertices)
        , texture(Txture) {};

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

    std::vector<Glyph*> m_glyph;
    std::vector<RenderBatch> m_renderBatch;

  public:
    SpriteBatch();
    ~SpriteBatch();

    void init();
    void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
    void end();
    void draw(const glm::vec4& desRect, const glm::vec4& uvRect, GLuint texture, float depth, Color& color);
    void renderBatch();
};
