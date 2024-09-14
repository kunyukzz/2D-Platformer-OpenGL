#include "spritebatch.h"
#include <algorithm>

SpriteBatch::SpriteBatch()
    : m_vbo(0)
    , m_vao(0){};

SpriteBatch::~SpriteBatch() {}

void SpriteBatch::createRenderBatch()
{
    std::vector<Vertex> vertices;
    vertices.resize(m_glyphPointer.size() * 6);

    if (m_glyphPointer.empty())
        return;

    int offset = 0;
    int cV = 0; // current vertex
    m_renderBatch.emplace_back(offset, 6, m_glyphPointer[0]->texture);
    vertices[cV++] = m_glyphPointer[0]->topLeft;
    vertices[cV++] = m_glyphPointer[0]->bottomLeft;
    vertices[cV++] = m_glyphPointer[0]->bottomRight;
    vertices[cV++] = m_glyphPointer[0]->bottomRight;
    vertices[cV++] = m_glyphPointer[0]->topRight;
    vertices[cV++] = m_glyphPointer[0]->topLeft;
    offset += 6;

    // loop inside current glyphs
    for (unsigned int cg = 1; cg < m_glyphPointer.size(); cg++)
    {
        if (m_glyphPointer[cg]->texture != m_glyphPointer[cg - 1]->texture)
        {
            m_renderBatch.emplace_back(offset, 6, m_glyphPointer[cg]->texture);
        }
        else
        {
            m_renderBatch.back().numVertices += 6;
        }
        vertices[cV++] = m_glyphPointer[cg]->topLeft;
        vertices[cV++] = m_glyphPointer[cg]->bottomLeft;
        vertices[cV++] = m_glyphPointer[cg]->bottomRight;
        vertices[cV++] = m_glyphPointer[cg]->bottomRight;
        vertices[cV++] = m_glyphPointer[cg]->topRight;
        vertices[cV++] = m_glyphPointer[cg]->topLeft;
        offset += 6;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // orphan the buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    // upload the data
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteBatch::createVertexArray()
{
    if (m_vao == 0)
        glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    if (m_vbo == 0)
        glGenVertexArrays(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // tell openGL to use first attribute array
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // this is position attribute color
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    // this is color attribute pointer
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    // this is uv attribute pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindVertexArray(0);
}

void SpriteBatch::sortGlyphs()
{
    switch (m_sortType)
    {
    case GlyphSortType::BACK_TO_FRONT:
        std::stable_sort(m_glyphPointer.begin(), m_glyphPointer.end(), compareBackToFront);
        break;
    case GlyphSortType::FRONT_TO_BACK:
        std::stable_sort(m_glyphPointer.begin(), m_glyphPointer.end(), compareFrontToBack);
        break;
    case GlyphSortType::TEXTURE:
        std::stable_sort(m_glyphPointer.begin(), m_glyphPointer.end(), compareTexture);
        break;
    case GlyphSortType::NONE:
        std::stable_sort(m_glyphPointer.begin(), m_glyphPointer.end(), compareTexture);
        break;
    }
}

bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
{
    return a->depth < b->depth;
}

bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
{
    return a->depth > b->depth;
}

bool SpriteBatch::compareTexture(Glyph* a, Glyph* b)
{
    return a->texture < b->texture;
}

void SpriteBatch::init()
{
    createVertexArray();
}
void SpriteBatch::begin(GlyphSortType sortType)
{
    m_sortType = sortType;
    m_renderBatch.clear();
    m_glyph.clear();
}

void SpriteBatch::end()
{
    m_glyphPointer.resize(m_glyph.size());
    for (unsigned int i = 0; i < m_glyph.size(); i++)
    {
        m_glyphPointer[i] = &m_glyph[i];
    }

    sortGlyphs();
    createRenderBatch();
}

void SpriteBatch::draw(const glm::vec4& desRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color)
{
    m_glyph.emplace_back(desRect, uvRect, texture, depth, color);
}
void SpriteBatch::renderBatch()
{
    glDisable(GL_CULL_FACE);
    glBindVertexArray(m_vao);

    for (uint i = 0; i < m_renderBatch.size(); i++)
    {
        glBindTexture(GL_TEXTURE_2D, m_renderBatch[i].texture);
        glDrawArrays(GL_TRIANGLES, m_renderBatch[i].offset, m_renderBatch[i].numVertices);
    }

    glBindVertexArray(0);
}
