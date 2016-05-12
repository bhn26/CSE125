#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <algorithm>
#include <utility>

class Shader
{
private:
    GLuint program;

public:
    Shader() : program((GLuint)0) {};
    Shader(const char* vertex_file_path, const char* fragment_file_path);
    ~Shader();

    Shader(const Shader& rhs) = delete;
    Shader& operator=(Shader& rhs) = delete;
    Shader(Shader&& rhs) : program(rhs.program) { rhs.program = 0; }
    Shader& operator=(Shader&& rhs) { program = rhs.program; rhs.program = 0; return *this; }

    bool SetShaders(const char* vertex_file_path, const char* fragment_file_path);
    void Use() const { glUseProgram(this->program); }
    void Unuse() const { glUseProgram(0); }   // Not working, not sure why. Don't use
    GLint GetUniform(const char* var) const { return glGetUniformLocation(this->program, var); }
};

#endif
