#include "camera2d.h"

Camera2D::Camera2D()
    : m_scW(500)
    , m_scH(500)
    , m_needUpdateMatrix(true)
    , m_scale(1.0f)
    , m_position(0.0f, 0.0f)
    , m_camMatrix(1.0f)
    , m_orthoMatrix(1.0f) {};

Camera2D::~Camera2D() {}

void Camera2D::init(int scWidth, int scHeight)
{
    m_scW = scWidth;
    m_scH = scHeight;
    m_orthoMatrix = glm::ortho(0.0f, (float)m_scW, 0.0f, (float)m_scH);
}

void Camera2D::update()
{
    if (m_needUpdateMatrix)
    {
        // get the position
        glm::vec3 translate(-m_position.x + m_scW / 2.0, -m_position.y + m_scH / 2.0, 0.0f);
        m_camMatrix = glm::translate(m_orthoMatrix, translate);

        // get the scale
        glm::vec3 scale(m_scale, m_scale, 0.0f);
        m_camMatrix = glm::scale(glm::mat4(1.0f), scale) * m_camMatrix;

        m_needUpdateMatrix = false;
    }
}

glm::vec2 Camera2D::convScreenToWorld(glm::vec2 screenCoord)
{
    // invert Y direction
    screenCoord.y = m_scH - screenCoord.y;

    // make zero coordinate to center of screen
    screenCoord += glm::vec2(m_scW / 2, m_scH / 2);

    // scale coordinate
    screenCoord /= m_scale;

    // translate with camera position
    screenCoord += m_position;
    return screenCoord;
}

