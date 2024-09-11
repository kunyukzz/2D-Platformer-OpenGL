#include "resmanager.h"

TextureCache ResourceManager::m_textureCache;

// this function was only for warper to give access to textureCache
GLTexture ResourceManager::getTexture(std::string texturePath)
{
    return m_textureCache.getTexture(texturePath);
}
