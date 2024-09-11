#pragma once
#include "texturecache.h"

class ResourceManager
{
    static TextureCache m_textureCache;

  public:
    static GLTexture getTexture(std::string texturePath);
};
