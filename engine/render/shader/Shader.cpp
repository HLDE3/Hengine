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

Shader::Shader(int shader_program) {
    this->shader_program = shader_program;
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

void Shader::setUniform1i(const char *name, int v1) const {
    glUniform1i(glGetUniformLocation(this->shader_program, name), v1);
}

void Shader::setUniform2i(const char *name, int v1, int v2) const {
    glUniform2i(glGetUniformLocation(this->shader_program, name), v1, v2);
}

void Shader::setUniform3i(const char *name, int v1, int v2, int v3) const {
    glUniform3i(glGetUniformLocation(this->shader_program, name), v1, v2, v3);
}

void Shader::setUniform4i(const char *name, int v1, int v2, int v3, int v4) const {
    glUniform4i(glGetUniformLocation(this->shader_program, name), v1, v2, v3, v4);
}

void Shader::setUniform1f(const char *name, float v1) const {
    glUniform1f(glGetUniformLocation(this->shader_program, name), v1);
}

void Shader::setUniform2f(const char *name, float v1, float v2) const {
    glUniform2f(glGetUniformLocation(this->shader_program, name), v1, v2);
}

void Shader::setUniform3f(const char *name, float v1, float v2, float v3) const {
    glUniform3f(glGetUniformLocation(this->shader_program, name), v1, v2, v3);
}

void Shader::setUniform4f(const char *name, float v1, float v2, float v3, float v4) const {
    glUniform4f(glGetUniformLocation(this->shader_program, name), v1, v2, v3, v4);
}

void Shader::setUniform2x2f(const char *name, float * value) const {
    auto location = glGetUniformLocation(this->shader_program, name);
    glUniformMatrix2fv(location, 1, GL_FALSE, value);
}


void Shader::setUniform3x3f(const char *name, float * value) const {
    auto location = glGetUniformLocation(this->shader_program, name);
    glUniformMatrix3fv(location, 1, GL_FALSE, value);
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