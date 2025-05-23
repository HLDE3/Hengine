#include "BufferBuilder.h"
#include <cstring>
#include <iostream>
#include <GL/glew.h>

BufferBuilder::BufferBuilder(VertexSources::VertexSource* vertex_source) : vertex_source(vertex_source) {
    for (auto layout : vertex_source->layouts) {
        vertex_size += layout->count * layout->type_size;
    }
}

BufferBuilder::~BufferBuilder() {
    data.clear();
    data.shrink_to_fit();
    if (vao_id) glDeleteVertexArrays(1, &vao_id);
    if (vbo_id) glDeleteBuffers(1, &vbo_id);
    vao_id = 0;
    vbo_id = 0;
}

void BufferBuilder::data_check() {
    const size_t required_size = (current_vertex + 1) * vertex_size;
    if (required_size > data.max_size()) {
        std::cerr << "BufferBuilder: required_size too large!" << std::endl;
        std::abort();
    }
    if (data.size() < required_size) {
        data.resize(required_size);
    }
}

template<typename T>
BufferBuilder* BufferBuilder::layout(size_t index, const T* src_data) {
    if (index >= vertex_source->layouts.size()) {
        std::cerr << "Index out of bounds: " << index << " >= " << vertex_source->layouts.size() << std::endl;
        return this;
    }
    auto layout = vertex_source->layouts[index];
    size_t offset = layout->offset; // offset уже в байтах
    data_check();
    std::memcpy(&data[current_vertex * vertex_size + offset], src_data, layout->count * layout->type_size);
    return this;
}

template<typename T>
T BufferBuilder::get(size_t index) const {
    if (index >= vertex_size) {
        std::cerr << "Index out of bounds: " << index << " >= " << vertex_size << std::endl;
        return T{};
    }
    T value;
    std::memcpy(&value, &data[current_vertex * vertex_size + index * sizeof(T)], sizeof(T));
    return value;
}

template<typename T>
void BufferBuilder::set(size_t index, T value) {
    if (index >= vertex_size) {
        std::cerr << "Index out of bounds: " << index << " >= " << vertex_size << std::endl;
        return;
    }
    std::memcpy(&data[current_vertex * vertex_size + index * sizeof(T)], &value, sizeof(T));
}

BufferBuilder* BufferBuilder::position(float x, float y, float z) {
    float arr[3] {x, y, z};
    layout(0, arr);
    return this;
}

BufferBuilder* BufferBuilder::color(float r, float g, float b, float a) {
    float arr[4] {r, g, b, a};
    layout(1, arr);
    return this;
}

BufferBuilder* BufferBuilder::uv(float u, float v) {
    float arr[2] {u, v};
    layout(2, arr);
    return this;
}

BufferBuilder* BufferBuilder::next() {
    current_vertex++;
    return this;
}

BufferBuilder* BufferBuilder::vertex(size_t index) {
    this->current_vertex = index;
    data_check();
    return this;
}

void BufferBuilder::create_vao_vbo(unsigned int& vaoID, unsigned int& vboID, const uint8_t* vertices, size_t byte_size) const {
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, byte_size, vertices, GL_STATIC_DRAW);

    for (const auto& layout : vertex_source->layouts) {
        glVertexAttribPointer(
            layout->location,
            layout->count,
            layout->type,
            layout->normalized,
            vertex_size,
            layout->pointer
        );
        glEnableVertexAttribArray(layout->location);
    }
}

void BufferBuilder::build() {
    if (size() == 0) return;
    create_vao_vbo(vao_id, vbo_id, data.data(), size());
}

void BufferBuilder::update() {
    if (!vbo_id || !vao_id) build();
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size(), data.data());
}

void BufferBuilder::draw(int type) const {
    glBindVertexArray(vao_id);
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