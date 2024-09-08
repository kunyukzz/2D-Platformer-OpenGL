#pragma once

#include "gltexture.h"
#include <string>

class ImageLoader
{
  public:
    GLTexture loadPNG(std::string filePath);
};
