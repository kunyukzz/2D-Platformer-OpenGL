#pragma once
#include "gltexture.h"
#include <map>
#include <string>

class TextureCache
{
    std::map<std::string, GLTexture> m_txMap;

  public:
    TextureCache();
    ~TextureCache();

    GLTexture getTexture(std::string texturePath);
};
