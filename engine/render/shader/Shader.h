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
    int shaderProgram;
    VertexSources::VertexSource* vertexSource = nullptr;
    explicit Shader(unsigned int vertexShader, unsigned int fragmentShader);

    Shader(VertexSources::VertexSource* vertexSource, const char *fragmentShaderSource);

    explicit Shader(int shaderProgram);
    ~Shader();

    void setUniformf(const char* name, float * values, size_t size) const;

    void setUniform1x1f(const char* name, float value);
    void setUniform2x2f(const char* name, float value);
    void setUniform3x3f(const char* name, float value);
    void setUniform4x4f(const char* name, float * value);

    static unsigned int createShader(const char* shaderSource, int shaderType);
    static int createProgram(unsigned int shader, bool destroy = true);
    static int createProgram(unsigned int shaders[], size_t size, bool destroy = true);
};



#endif //SHADER_H
