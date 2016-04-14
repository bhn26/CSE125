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
    Shader(const Shader& rhs) { program = rhs.program; }
    ~Shader();

    Shader& operator=(Shader rhs) { std::swap(rhs.program, program); return *this; }


    bool SetShaders(const char* vertex_file_path, const char* fragment_file_path);
    void Use() { glUseProgram(this->program); }
    void Unuse() { glUseProgram(0); }   // Not working, not sure why. Don't use
    GLint GetUniform(const char* var) { return glGetUniformLocation(this->program, var); }
};

#endif
