#include "spritebatch.h"
#include <algorithm>

SpriteBatch::SpriteBatch()
    : m_vbo(0)
    , m_vao(0) {};

SpriteBatch::~SpriteBatch() {}

void SpriteBatch::createRenderBatch()
{
    std::vector<Vertex> vertices;
    vertices.resize(m_glyph.size() * 6);

    if (m_glyph.empty())
        return;

    int offset = 0;
    int cV = 0; // current vertex
    m_renderBatch.emplace_back(0, 6, m_glyph[0]->texture);
    vertices[cV++] = m_glyph[0]->topLeft;
    vertices[cV++] = m_glyph[0]->bottomLeft;
    vertices[cV++] = m_glyph[0]->bottomRight;
    vertices[cV++] = m_glyph[0]->bottomRight;
    vertices[cV++] = m_glyph[0]->topRight;
    vertices[cV++] = m_glyph[0]->topLeft;
    offset += 6;

    // loop inside current glyphs
    for (int cg = 1; cg < m_glyph.size(); cg++)
    {
        if (m_glyph[cg]->texture != m_glyph[cg - 1]->texture)
        {
            m_renderBatch.emplace_back(offset, 6, m_glyph[cg]->texture);
        }
        else
        {
            m_renderBatch.back().numVertices += 6;
        }
        vertices[cV++] = m_glyph[cg]->topLeft;
        vertices[cV++] = m_glyph[cg]->bottomLeft;
        vertices[cV++] = m_glyph[cg]->bottomRight;
        vertices[cV++] = m_glyph[cg]->bottomRight;
        vertices[cV++] = m_glyph[cg]->topRight;
        vertices[cV++] = m_glyph[cg]->topLeft;
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
        std::stable_sort(m_glyph.begin(), m_glyph.end(), compareBackToFront);
        break;
    case GlyphSortType::FRONT_TO_BACK:
        std::stable_sort(m_glyph.begin(), m_glyph.end(), compareFrontToBack);
        break;
    case GlyphSortType::TEXTURE:
        std::stable_sort(m_glyph.begin(), m_glyph.end(), compareTexture);
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

    for (int i = 0; i < m_glyph.size(); i++)
    {
        delete m_glyph[i];
    }
    m_glyph.clear();
}

void SpriteBatch::end()
{
    sortGlyphs();
    createRenderBatch();
}

void SpriteBatch::draw(const glm::vec4& desRect, const glm::vec4& uvRect, GLuint texture, float depth, Color& color)
{
    Glyph* newGlyph = new Glyph;
    newGlyph->texture = texture;
    newGlyph->depth = depth;

    // top left vertex
    newGlyph->topLeft.color = color;
    newGlyph->topLeft.setPos(desRect.x, desRect.y + desRect.w);
    newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

    // bottom left vertex
    newGlyph->bottomLeft.color = color;
    newGlyph->bottomLeft.setPos(desRect.x, desRect.y);
    newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

    // top right vertex
    newGlyph->topRight.color = color;
    newGlyph->topRight.setPos(desRect.x + desRect.z, desRect.y);
    newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y);

    // bottom right vertex
    newGlyph->bottomRight.color = color;
    newGlyph->bottomRight.setPos(desRect.x + desRect.z, desRect.y + desRect.w);
    newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

    m_glyph.push_back(newGlyph);
}
void SpriteBatch::renderBatch()
{
    glBindVertexArray(m_vao);

    for (int i = 0; i < m_renderBatch.size(); i++)
    {
        glBindTexture(GL_TEXTURE_2D, m_renderBatch[i].texture);
        glDrawArrays(GL_TRIANGLES, m_renderBatch[i].offset, m_renderBatch[i].numVertices);
    }

    glBindVertexArray(0);
}
