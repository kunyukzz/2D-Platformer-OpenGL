#include "imageloader.h"
#include "errorhandle.h"
#include "iomanager.h"
#include "picopng.h"

GLTexture ImageLoader::loadPNG(std::string filePath)
{
    GLTexture texture = {};

    std::vector<unsigned char> in;
    std::vector<unsigned char> out;

    unsigned long w, h;

    if (IOManager::readFiletoBuffer(filePath, in) == false)
        hdlError("Failed to load PNG to buffer!");

    int errorCode = decodePNG(out, w, h, &(in[0]), in.size());
    if (errorCode != 0)
        hdlError("decodePNG failed : " + std::to_string(errorCode));

    glGenTextures(1, &(texture.id));
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // default was gl_linear filter, but you can used nearest for much crisp image
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    texture.width = w;
    texture.height = h;

    return texture;
}
