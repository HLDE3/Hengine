//
// Created by ruhld on 21.05.2025.
//

#include "Shader.h"

#include <iostream>
#include <vector>
#include <GL/glew.h>

#include "VertexSources.h"

Shader::Shader(unsigned int vertexShader, unsigned int fragmentShader) {
    this->shaderProgram = createProgram(new unsigned int[2] {vertexShader, fragmentShader}, 2);
}

Shader::Shader(VertexSources::VertexSource* vertexSource, const char* fragmentShaderSource) : Shader(
            createShader(vertexSource->source, GL_VERTEX_SHADER),
            createShader(fragmentShaderSource, GL_FRAGMENT_SHADER)
    ) {
    this->vertexSource = vertexSource;
}

Shader::Shader(int shaderProgram) {
    this->shaderProgram = shaderProgram;
}

Shader::~Shader() {
    glDeleteProgram(this->shaderProgram);
}

bool check_program_link(GLuint program)
{
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> infoLog(logLength);
        glGetProgramInfoLog(program, logLength, nullptr, infoLog.data());
        std::cerr << "Ошибка линковки шейдерной программы:\n" << infoLog.data() << std::endl;
        return false;
    }
    return true;
}

void Shader::setUniformf(const char *name, float *values, size_t size) const {
    auto location = glGetUniformLocation(this->shaderProgram, name);
    switch (size) {
        case 1: glUniform1f(location, values[0]); break;
        case 2: glUniform2f(location, values[0], values[1]); break;
        case 3: glUniform3f(location, values[0], values[1], values[2]); break;
        case 4: glUniform4f(location, values[0], values[1], values[2], values[3]); break;
        default: ;
    }
}

void Shader::setUniform4x4f(const char *name, float * value) {
    auto location = glGetUniformLocation(this->shaderProgram, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

unsigned int Shader::createShader(const char* shaderSource, int shaderType) {
    unsigned int shader = glCreateShader(shaderType);

    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    return shader;
}

int Shader::createProgram(unsigned int shader, bool destroy) {
    unsigned int shaders[] = {shader};
    return createProgram(shaders, 1, destroy);
}

int Shader::createProgram(unsigned int shaders[], size_t size, bool destroy) {
    if (!shaders || size == 0) {
        return 0;
    }

    int shaderProgram = glCreateProgram();
    if (!shaderProgram) {
        return 0;
    }

    for (size_t i = 0; i < size; i++) {
        if (shaders[i]) {
            glAttachShader(shaderProgram, shaders[i]);
        }
    }

    glLinkProgram(shaderProgram);

    if (!check_program_link(shaderProgram)) {
        if (destroy) {
            for (size_t i = 0; i < size; i++) {
                if (shaders[i]) {
                    glDeleteShader(shaders[i]);
                }
            }
        }
        glDeleteProgram(shaderProgram);
        return 0;
    }

    if (destroy) {
        for (size_t i = 0; i < size; i++) {
            if (shaders[i]) {
                glDetachShader(shaderProgram, shaders[i]);
                glDeleteShader(shaders[i]);
            }
        }
    }

    return shaderProgram;
}