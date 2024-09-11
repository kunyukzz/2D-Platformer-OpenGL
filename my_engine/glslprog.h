#pragma once

#include <GL/glew.h>
#include <string>

class GLSLProgram
{
    int m_numAttr;
    GLuint m_progID;
    GLuint m_vertShID;
    GLuint m_fragShID;

    void compShader(const std::string& filePath, GLuint id);

  public:
    GLSLProgram();
    ~GLSLProgram();

    void compAllShader(const std::string& vertShFilePath, const std::string& fragShFilePath);
    void linkShader();
    void addAttr(const std::string& attrName);

    GLint getUniformLoc(const std::string& uniformName);

    void use();
    void unused();
};
