#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <cstdlib>
#include <cstring>

#include <GL/glew.h>

#include "Shader.h"
#include "Basic/Utils.h"

Shader::Shader(const std::string& vertex_file_path, const std::string& fragment_file_path)
    : m_program((GLuint)0)
{
    SetShaders(vertex_file_path, fragment_file_path);
}

bool Shader::SetShaders(const std::string& vertex_file_path, const std::string& fragment_file_path)
{
    bool success = true;
    // Create the shaders
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string vertexShaderCode;
    std::ifstream vertexShaderStream(vertex_file_path.c_str(), std::ios::in);
    if (!Utils::ReadFile(vertex_file_path, vertexShaderCode))
    {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the "
               "FAQ !\n",
               vertex_file_path.c_str());
        return false;
    }

    // Read the Fragment Shader code from the file
    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream(fragment_file_path, std::ios::in);
    if (!Utils::ReadFile(fragment_file_path, fragmentShaderCode))
    {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the "
               "FAQ !\n",
               fragment_file_path.c_str());
        return false;
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path.c_str());
    char const* VertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(vertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
        printf("%s\n", &vertexShaderErrorMessage[0]);
        success &= false;
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path.c_str());
    char const* fragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
    glCompileShader(fragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
        printf("%s\n", &fragmentShaderErrorMessage[0]);
        success &= false;
    }

    // Link the m_program
    printf("Linking m_program\n");
    GLuint m_programID = glCreateProgram();
    glAttachShader(m_programID, vertexShaderID);
    glAttachShader(m_programID, fragmentShaderID);
    glLinkProgram(m_programID);

    // Check the m_program
    glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
    glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> m_programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(m_programID, infoLogLength, NULL, &m_programErrorMessage[0]);
        printf("%s\n", &m_programErrorMessage[0]);
        success &= false;
    }

    glDetachShader(m_programID, vertexShaderID);
    glDetachShader(m_programID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    m_program = m_programID;
    return success;
}

Shader::~Shader()
{
    if (m_program)
    {
        glDeleteProgram(m_program);
    }
}
