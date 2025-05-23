//
// Created by ruhld on 21.05.2025.
//

#ifndef BUFFERBUILDER_H
#define BUFFERBUILDER_H
#include <vector>
#include "shader/VertexSources.h"


class BufferBuilder {
private:
    std::size_t current_vertex = 0;
    std::vector<float> data;
    void data_check();

public:

    unsigned int vboID = 0, vaoID = 0;

    std::size_t vertex_size = 0;

    VertexSources::VertexSource* vertex_source;

    explicit BufferBuilder(VertexSources::VertexSource* vertex_source);
    ~BufferBuilder();

    BufferBuilder * position(float x, float y, float z);
    BufferBuilder * color(float r, float g, float b, float a);
    BufferBuilder * uv(float u, float v);
    BufferBuilder * next();
    BufferBuilder * vertex(size_t index);

    void create_vao_vbo(unsigned int &vaoID, unsigned int &vboID, const float *vertices, size_t vertex_count) const;

    [[nodiscard]] float get(size_t index) const;
    void set(size_t index, float value);

    void build();
    void update();

    void draw(int type) const;
    void clear();

    [[nodiscard]] size_t size() const;
};



#endif //BUFFERBUILDER_H
