#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <algorithm>
#include <utility>
#include <string>

class Shader
{
private:
    GLuint program;

public:
    Shader() : program((GLuint)0) {};
    Shader(const std::string& vertex_file_path, const std::string& fragment_file_path);
    ~Shader();

    // Copy Constructor and Copy Assignment cannot be implemented. If a copied Shader were deleted
    // the other shaders using the same program would be invalidated. If a shader must be shared,
    // use a pointer to a heap-allocated Shader
    Shader(const Shader& rhs) = delete;
    Shader& operator=(Shader& rhs) = delete;
    Shader(Shader&& rhs) : program(rhs.program) { rhs.program = 0; }
    Shader& operator=(Shader&& rhs) { program = rhs.program; rhs.program = 0; return *this; }

    bool SetShaders(const std::string& vertex_file_path, const std::string& fragment_file_path);
    void Use() const { glUseProgram(this->program); }
    void Unuse() const { glUseProgram(0); }   // Not working, not sure why. Don't use
    GLint GetUniform(const char* var) const { return glGetUniformLocation(this->program, var); }

	GLuint GetProgram() { return program; };
};

#endif
