#pragma once

#include <vector>
#include <cstdint>
#include "shader/VertexSources.h"

class BufferBuilder {
private:
    std::size_t current_vertex = 0;
    std::vector<uint8_t> data;
    void data_check();

public:
    unsigned int vbo_id = 0, vao_id = 0;
    std::size_t vertex_size = 0;
    VertexSources::VertexSource* vertex_source;

    explicit BufferBuilder(VertexSources::VertexSource* vertex_source);
    ~BufferBuilder();

    template<typename T>
    int layout(size_t index, const T* src_data);

    BufferBuilder* position(float x, float y, float z);
    BufferBuilder* color(float r, float g, float b, float a);
    BufferBuilder* uv(float u, float v);
    BufferBuilder* next();
    BufferBuilder* vertex(size_t index);

    void create_vao_vbo(unsigned int& vaoID, unsigned int& vboID, const uint8_t* vertices, size_t byte_size) const;

    template<typename T>
    [[nodiscard]] T get(size_t index) const;

    template<typename T>
    void set(size_t index, T value);

    void build();
    void update();

    void draw(int type) const;
    void clear();

    [[nodiscard]] size_t size() const; // возвращает размер в байтах
};