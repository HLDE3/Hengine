//
// Created by ruhld on 21.05.2025.
//

#ifndef SHADER_H
#define SHADER_H
#include <string>

namespace VertexSources {
    struct VertexSource;
}

class Shader {
public:
    int shader_program;
    VertexSources::VertexSource* vertex_source = nullptr;
    explicit Shader(unsigned int vertex_shader, unsigned int fragment_shader);

    Shader(VertexSources::VertexSource* vertex_source, const char *fragment_shader_source);

    explicit Shader(int shader_srogram);
    ~Shader();

    void setUniformf(const char* name, float * values, size_t size) const;
    void setUniformi(const char* name, int * values, size_t size) const;

    void setUniform1x1f(const char* name, float value);
    void setUniform2x2f(const char* name, float value);
    void setUniform3x3f(const char* name, float value);
    void setUniform4x4f(const char* name, float * value) const;

    static unsigned int createShader(const char* shader_source, int shader_type);
    static int createProgram(unsigned int shader, bool destroy = true);
    static int createProgram(unsigned int shaders[], size_t size, bool destroy = true);
};



#endif //SHADER_H
