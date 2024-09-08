#include "sprite.h"

Sprite::Sprite() {}

Sprite::~Sprite()
{
    if (m_vboID != 0)
        glDeleteBuffers(1, &m_vboID);
}

void Sprite::init(float x, float y, float w, float h)
{
    m_x = x;
    m_y = y;
    m_width = w;
    m_height = h;

    if (m_vboID == 0)
        glGenBuffers(1, &m_vboID);

    float vData[12]; // vertex total count * 2(bcz 2 dimension, x and y)

    // ----------------------- THIS BELOW WAS PARAMETER VERTEX POSITION---------------------------------
    // ----------------------------- IT HAS 2 TRIANGLES COMBINE-----------------------------------------

    // FIRST TRIANGLE
    // vertex top-right corner
    vData[0] = x + w;
    vData[1] = y + h;

    // vertex top-left corner
    vData[2] = x;
    vData[3] = y + h;

    // vertex bottom-left corner
    vData[4] = x;
    vData[5] = y;

    // SECOND TRIANGLE
    // vertex bottom-left corner
    vData[6] = x;
    vData[7] = y;

    // vertex bottom-right corner
    vData[8] = x + w;
    vData[9] = y;

    // vertex top-right corner
    vData[10] = x + w;
    vData[11] = y + h;

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vData), vData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vertex buffer
}

void Sprite::draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vertex buffer
}
