#include "sprite.h"
#include "resmanager.h"
#include "vertex.h"
#include <cstddef>

Sprite::Sprite() {}

Sprite::~Sprite()
{
    if (m_vboID != 0)
        glDeleteBuffers(1, &m_vboID);
}

void Sprite::init(float x, float y, float w, float h, std::string texturePath)
{
    m_x = x;
    m_y = y;
    m_width = w;
    m_height = h;

    m_texture = ResourceManager::getTexture(texturePath);

    if (m_vboID == 0)
        glGenBuffers(1, &m_vboID);

    Vertex vData[6]; // vertex total count * 2(bcz 2 dimension, x and y)

    // ----------------------- THIS BELOW WAS PARAMETER VERTEX POSITION---------------------------------
    // ----------------------------- IT HAS 2 TRIANGLES COMBINE-----------------------------------------

    // FIRST TRIANGLE
    vData[0].setPos(x + w, y + h); // vertex top-right corner
    vData[0].setUV(1.0f, 1.0f);
    vData[1].setPos(x, y + h); // vertex top-left corner
    vData[1].setUV(0.0f, 1.0f);
    vData[2].setPos(x, y); // vertex bottom-left corner
    vData[2].setUV(0.0f, 0.0f);

    // SECOND TRIANGLE
    vData[3].setPos(x, y); // vertex bottom-left corner
    vData[3].setUV(0.0f, 0.0f);
    vData[4].setPos(x + w, y); // vertex bottom-right corner
    vData[4].setUV(1.0f, 0.0f);
    vData[5].setPos(x + w, y + h); // vertex top-right corner
    vData[5].setUV(1.0f, 1.0f);

    // give color per vertex
    for (int i = 0; i < 6; i++)
    {
        vData[i].setColor(255, 0, 255, 255); // base color for all vertex
    }

    // top left corner
    vData[1].setColor(200, 0, 105, 255);

    // bottom right color
    vData[4].setColor(255, 255, 0, 255);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vData), vData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vertex buffer
}

void Sprite::draw()
{
    // bind texture id
    glBindTexture(GL_TEXTURE_2D, m_texture.id);

    // bind buffer object
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

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

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vertex buffer
}
