#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Sprite
{
    float m_x;
    float m_y;
    float m_width;
    float m_height;
    GLuint m_vboID;     // vertex buffer object ID using openGL

  public:
    Sprite();
    ~Sprite();

    void init(float x, float y, float w, float h);
    void draw();
};
