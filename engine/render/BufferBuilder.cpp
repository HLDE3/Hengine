//
// Created by ruhld on 21.05.2025.
//

#include "BufferBuilder.h"

#include <iostream>
#include <GL/glew.h>

void BufferBuilder::data_check() {
    const size_t required_size = (current_vertex + 1) * vertex_size;
    if (data.size() < required_size) {
        data.resize(required_size);
    }
}

float BufferBuilder::get(size_t index) const {
    if (index > vertex_size) {
        std::cerr << "Index out of bounds: " << index << " > " << vertex_size << std::endl;
        return 0;
    }
    return data.at(current_vertex * vertex_size + index);
}

void BufferBuilder::set(size_t index, float value) {
    if (index > vertex_size) {
        std::cerr << "Index out of bounds: " << index << " > " << vertex_size << std::endl;
        return;
    }
    data.at(current_vertex * vertex_size + index) = value;
}


BufferBuilder::BufferBuilder(VertexSources::VertexSource* vertex_source) : vertex_source(vertex_source) {
    for (auto layout : vertex_source->layouts) {
        vertex_size += layout.count;
    }
}

BufferBuilder::~BufferBuilder() {
    data.clear();
    data.shrink_to_fit();
    if (vaoID) glDeleteVertexArrays(1, &vaoID);
    if (vboID) glDeleteBuffers(1, &vboID);
    vaoID = 0;
    vboID = 0;
}

BufferBuilder * BufferBuilder::position(float x, float y, float z) {
    data_check();
    set(0, x);
    set(1, y);
    set(2, z);
    return this;
}

BufferBuilder * BufferBuilder::color(float r, float g, float b, float a) {
    data_check();
    set(3, r);
    set(4, g);
    set(5, b);
    set(6, a);
    return this;
}

BufferBuilder * BufferBuilder::uv(float u, float v) {
    data_check();
    set(7, u);
    set(8, v);
    return this;
}

BufferBuilder * BufferBuilder::next() {
    current_vertex++;
    return this;
}

BufferBuilder * BufferBuilder::vertex(size_t index) {
    this->current_vertex = index;
    data_check();
    return this;
}

void BufferBuilder::create_vao_vbo(unsigned int& vaoID, unsigned int& vboID, const float* vertices, size_t vertex_count) const {
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(float), vertices, GL_STATIC_DRAW);

    GLsizei stride = sizeof(float) * vertex_size;

    size_t offset = 0;
    for (const auto& layout : vertex_source->layouts) {
        glVertexAttribPointer(
            layout.location,
            layout.count,
            layout.type,
            layout.normalized,
            stride,
            reinterpret_cast<const void*>(offset * sizeof(float))
        );
        glEnableVertexAttribArray(layout.location);
        offset += layout.count;
    }
}

void BufferBuilder::build() {
    if (size() == 0) return;
    create_vao_vbo(vaoID, vboID, data.data(), size());
}

void BufferBuilder::update() {
    if (!vboID || !vaoID) build();
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size() * sizeof(float), data.data());
}

void BufferBuilder::draw(int type) const {
    glBindVertexArray(vaoID);
    glDrawArrays(type, 0, size() / vertex_size);

}

void BufferBuilder::clear() {
    data.clear();
    data.shrink_to_fit();
    vertex(0);
}

size_t BufferBuilder::size() const {
    return data.size();
}

