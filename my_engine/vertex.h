#pragma once
#include <GL/glew.h>

struct Position
{
    float x;
    float y;
};

struct Color
{
    // 4 bytes for a color (r g b a)
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
};

struct UV
{
    float u;
    float v;
};

struct Vertex
{
    Position pos;
    Color color;
    UV uv;

    void setPos(float x, float y)
    {
        pos.x = x;
        pos.y = y;
    }

    void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
    {
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
    }

    void setUV(float u, float v)
    {
        uv.u = u;
        uv.v = v;
    }
};
