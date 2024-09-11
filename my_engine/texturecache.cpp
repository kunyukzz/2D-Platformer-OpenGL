#include "texturecache.h"
#include "imageloader.h"
#include <iostream>

TextureCache::TextureCache() {}

TextureCache::~TextureCache() {}

GLTexture TextureCache::getTexture(std::string texturePath)
{
    // lookup texture and see if its inside the map
    auto mit = m_txMap.find(texturePath);

    // if it's not inside the map
    if (mit == m_txMap.end())
    {
        GLTexture newTexture = ImageLoader::loadPNG(texturePath); // load the texture
        m_txMap.insert(std::make_pair(texturePath, newTexture));  // put those texture inside map

        std::cout << "Load texture\n";
        return newTexture;
    }

    std::cout << "Used cache texture\n";
    return mit->second;
}
