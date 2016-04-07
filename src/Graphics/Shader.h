#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <algorithm>

class Shader
{
private:
    GLuint program;

public:
    Shader() : program((GLuint)0) {};
    Shader(const char* vertex_file_path, const char* fragment_file_path);
    Shader(const Shader& rhs) { program = rhs.program; }
    ~Shader();

    void operator=(Shader rhs) { std::swap(rhs.program, program); }

    bool SetShaders(const char* vertex_file_path, const char* fragment_file_path);
    void Use() { glUseProgram(this->program); }
    GLint GetUniform(const char* var) { return glGetUniformLocation(this->program, var); }
};

#endif
