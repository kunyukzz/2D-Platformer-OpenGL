#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D
{
    int m_scW, m_scH;
    bool m_needUpdateMatrix;
    float m_scale;
    glm::vec2 m_position;
    glm::mat4 m_camMatrix;
    glm::mat4 m_orthoMatrix;

  public:
    Camera2D();
    ~Camera2D();

    void init(int scWidth, int scHeight);
    void update();

    void setPosition(const glm::vec2& newPosition)
    {
        m_position = newPosition;
        m_needUpdateMatrix = true;
    }
    void setScale(float newScale)
    {
        m_scale = newScale;
        m_needUpdateMatrix = true;
    }

    glm::vec2 getPosition() { return m_position; }
    float getScale() { return m_scale; }
    glm::mat4 getCamMatrix() { return m_camMatrix; }
};
