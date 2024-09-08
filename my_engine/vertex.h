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

struct Vertex
{
    Position pos;
    Color color;
};
