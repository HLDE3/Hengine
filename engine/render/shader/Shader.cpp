//
// Created by ruhld on 21.05.2025.
//

#include "Shader.h"

#include <iostream>
#include <vector>
#include <GL/glew.h>

#include "VertexSources.h"

Shader::Shader(unsigned int vertex_shader, unsigned int fragment_shader) {
    this->shader_program = createProgram(new unsigned int[2] {vertex_shader, fragment_shader}, 2);
}

Shader::Shader(VertexSources::VertexSource* vertex_source, const char* fragment_shader_source) : Shader(
            createShader(vertex_source->source, GL_VERTEX_SHADER),
            createShader(fragment_shader_source, GL_FRAGMENT_SHADER)
    ) {
    this->vertex_source = vertex_source;
}

Shader::Shader(int shader_srogram) {
    this->shader_program = shader_srogram;
}

Shader::~Shader() {
    glDeleteProgram(this->shader_program);
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
    auto location = glGetUniformLocation(this->shader_program, name);
    switch (size) {
        case 1: glUniform1f(location, values[0]); break;
        case 2: glUniform2f(location, values[0], values[1]); break;
        case 3: glUniform3f(location, values[0], values[1], values[2]); break;
        case 4: glUniform4f(location, values[0], values[1], values[2], values[3]); break;
        default: ;
    }
}

void Shader::setUniformi(const char *name, int *values, size_t size) const {
    auto location = glGetUniformLocation(this->shader_program, name);
    switch (size) {
        case 1: glUniform1i(location, values[0]); break;
        case 2: glUniform2i(location, values[0], values[1]); break;
        case 3: glUniform3i(location, values[0], values[1], values[2]); break;
        case 4: glUniform4i(location, values[0], values[1], values[2], values[3]); break;
        default: ;
    }
}

void Shader::setUniform4x4f(const char *name, float * value) const {
    auto location = glGetUniformLocation(this->shader_program, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

unsigned int Shader::createShader(const char* shader_source, int shader_type) {
    unsigned int shader = glCreateShader(shader_type);

    glShaderSource(shader, 1, &shader_source, nullptr);
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