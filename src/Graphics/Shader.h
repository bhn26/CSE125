#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <algorithm>
#include <utility>
#include <string>

class Shader
{
    friend class ShaderManager;

private:
    GLuint m_program;

    Shader() : m_program((GLuint)0){}; // To get a shader use the config file and ShaderManager
    Shader(const std::string& vertex_file_path, const std::string& fragment_file_path);

public:
    ~Shader();

    // Copy Constructor and Copy Assignment cannot be implemented. If a copied Shader were deleted
    // the other shaders using the same m_program would be invalidated. If a shader must be shared,
    // use a pointer to a heap-allocated Shader
    Shader(const Shader& rhs) = delete;
    Shader& operator=(Shader& rhs) = delete;
    Shader(Shader&& rhs) : m_program(std::move(rhs.m_program)) {}
    Shader& operator=(Shader&& rhs)
    {
        m_program = std::move(rhs.m_program);
        return *this;
    }

    bool SetShaders(const std::string& vertex_file_path, const std::string& fragment_file_path);
    void Use() const { glUseProgram(this->m_program); }
    void Unuse() const { glUseProgram(0); } // Not working, not sure why. Don't use
    GLint GetUniform(const char* var) const { return glGetUniformLocation(this->m_program, var); }

    GLuint GetProgram() { return m_program; };
};

#endif
