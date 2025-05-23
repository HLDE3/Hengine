//
// Created by ruhld on 22.05.2025.
//

#ifndef VERTEXSOURCES_H
#define VERTEXSOURCES_H
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "../../utils/type_utils.h"

namespace VertexSources {

    class Layout {
    public:
        int location;
        int count;
        int type;
        int normalized;
        size_t type_size;
        int offset = 0;
        const void* pointer = nullptr;
        Layout(int location, int count, int type, bool normalized = false) :
            location(location), count(count), type(type), normalized(normalized) {
            type_size = type_utils::size_of_gl_enum(type);
        }
    };

    class VertexSource {
    public:

        const char* source;
        std::vector<Layout*> layouts;

        VertexSource(const char* source, const std::vector<Layout*> &layouts) :
            source(source), layouts(layouts) {
            auto offset = 0;
            for (auto layout : layouts) {
                layout->pointer = reinterpret_cast<const void*>(offset * layout->type_size);
                layout->offset = offset * layout->type_size;
                offset += layout->count;
            }
        }
    };

    inline auto position_color = new VertexSource {
        R"(#version 330 core

            layout (location = 0) in vec3 position;
            layout (location = 1) in vec4 color;

            uniform mat4 projection;
            uniform mat4 model;
            uniform mat4 view;

            out vec4 outColor;

            void main() {
                gl_Position = projection * view * model * vec4(position, 1.0);
                outColor = color;
            })",
        {
            new Layout(0, 3, GL_FLOAT),
            new Layout(1, 4, GL_FLOAT)
        }
    };

    inline auto position_color_texture = new VertexSource {
        R"(#version 330 core

            layout (location = 0) in vec3 position;
            layout (location = 1) in vec4 color;
            layout (location = 2) in vec2 texCoord;

            uniform mat4 projection;
            uniform mat4 model;
            uniform mat4 view;

            out vec4 outColor;
            out vec2 outTexCoord;

            void main() {
                gl_Position = projection * view * model * vec4(position, 1.0);
                outColor = color;
                outTexCoord = texCoord;
            })",
        {
            new Layout(0, 3, GL_FLOAT),
            new Layout(1, 4, GL_FLOAT),
            new Layout(2, 2, GL_FLOAT)
        }
    };

    inline auto position_texture = new VertexSource {
        R"(#version 330 core

            layout (location = 0) in vec3 position;
            layout (location = 1) in vec2 texCoord;

            uniform mat4 projection;
            uniform mat4 model;
            uniform mat4 view;

            out vec2 outTexCoord;

            void main() {
                gl_Position = projection * view * model * vec4(position, 1.0);
                outTexCoord = texCoord;
            })",
        {
            new Layout(0, 3, GL_FLOAT),
            new Layout(1, 2, GL_FLOAT)
        }
    };
}

#endif //VERTEXSOURCES_H
