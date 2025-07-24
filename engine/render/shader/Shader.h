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

    explicit Shader(int shader_program);
    ~Shader();

    void setUniform1i(const char* name, int v1) const;
    void setUniform2i(const char* name, int v1, int v2) const;
    void setUniform3i(const char* name, int v1, int v2, int v3) const;
    void setUniform4i(const char* name, int v1, int v2, int v3, int v4) const;

    void setUniform1f(const char* name, float v1) const;
    void setUniform2f(const char* name, float v1, float v2) const;
    void setUniform3f(const char* name, float v1, float v2, float v3) const;
    void setUniform4f(const char* name, float v1, float v2, float v3, float v4) const;

    void setUniform2x2f(const char* name, float * value) const;
    void setUniform3x3f(const char* name, float * value) const;
    void setUniform4x4f(const char* name, float * value) const;

    static unsigned int createShader(const char* shader_source, int shader_type);
    static int createProgram(unsigned int shader, bool destroy = true);
    static int createProgram(unsigned int shaders[], size_t size, bool destroy = true);
};



#endif //SHADER_H
