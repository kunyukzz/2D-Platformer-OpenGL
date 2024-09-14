#include "glslprog.h"
#include "errorhandle.h"
#include <fstream>
#include <vector>

GLSLProgram::GLSLProgram()
    : m_numAttr(0)
    , m_progID(0)
    , m_vertShID(0)
    , m_fragShID(0) {};

GLSLProgram::~GLSLProgram() {}

void GLSLProgram::compShader(const std::string& filePath, GLuint id)
{
    // open file data --------------------------------------
    std::ifstream vertFile(filePath);
    if (vertFile.fail())
        hdlError("fail to open " + filePath);

    // read file data --------------------------------------
    std::string fileCont = "";
    std::string line;
    while (std::getline(vertFile, line))
    {
        fileCont += line + "\n";
    }

    vertFile.close();

    const char* contentPtr = fileCont.c_str();
    glShaderSource(id, 1, &contentPtr, nullptr);

    glCompileShader(id);

    // shader error handling --------------------------------
    GLint success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(id); // Don't leak the shader.
        hdlError("shader " + filePath + " failed to compile");
    }
}

void GLSLProgram::compAllShader(const std::string& vertShFilePath, const std::string& fragShFilePath)
{
    m_progID = glCreateProgram();

    m_vertShID = glCreateShader(GL_VERTEX_SHADER);
    if (m_vertShID == 0)
        hdlError("vertex shader failed to created!");

    m_fragShID = glCreateShader(GL_FRAGMENT_SHADER);
    if (m_fragShID == 0)
        hdlError("fragment shader failed to created!");

    compShader(vertShFilePath, m_vertShID);
    compShader(fragShFilePath, m_fragShID);
}

void GLSLProgram::linkShader()
{
    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    // m_progID = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(m_progID, m_vertShID);
    glAttachShader(m_progID, m_fragShID);

    // Link our program
    glLinkProgram(m_progID);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(m_progID, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_progID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<char> infoLog(maxLength);
        glGetProgramInfoLog(m_progID, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(m_progID);

        // Don't leak shaders either.
        glDeleteShader(m_vertShID);
        glDeleteShader(m_fragShID);
    }

    // Always detach shaders after a successful link.
    glDetachShader(m_progID, m_vertShID);
    glDetachShader(m_progID, m_fragShID);
    glDeleteShader(m_vertShID);
    glDeleteShader(m_fragShID);
}

void GLSLProgram::addAttr(const std::string& attrName)
{
    glBindAttribLocation(m_progID, m_numAttr++, attrName.c_str());
}

GLint GLSLProgram::getUniformLoc(const std::string& uniformName)
{
    GLuint location = glGetUniformLocation(m_progID, uniformName.c_str());
    if (location == GL_INVALID_INDEX)
        hdlError("Uniform " + uniformName + " not found in shader!");

    return location;
}

void GLSLProgram::use()
{
    glUseProgram(m_progID);
    for (int i = 0; i < m_numAttr; i++)
    {
        glEnableVertexAttribArray(i);
    }
}

void GLSLProgram::unused()
{
    glUseProgram(0);
    for (int i = 0; i < m_numAttr; i++)
    {
        glDisableVertexAttribArray(i);
    }
}
