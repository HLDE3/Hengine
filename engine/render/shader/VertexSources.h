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

            layout (location = 0) in vec3 positionIn;
            layout (location = 1) in vec4 colorIn;

            uniform mat4 projection;
            uniform mat4 model;
            uniform mat4 view;

            out vec4 color;

            void main() {
                gl_Position = projection * view * model * vec4(positionIn, 1.0);
                color = colorIn;
            })",
        {
            new Layout(0, 3, GL_FLOAT),
            new Layout(1, 4, GL_FLOAT)
        }
    };

    inline auto position_color_texture = new VertexSource {
        R"(#version 330 core

            layout (location = 0) in vec3 positionIn;
            layout (location = 1) in vec4 colorIn;
            layout (location = 2) in vec2 texCoordIn;

            uniform mat4 projection;
            uniform mat4 model;
            uniform mat4 view;

            out vec4 color;
            out vec2 texCoord;

            void main() {
                gl_Position = projection * view * model * vec4(positionIn, 1.0);
                color = colorIn;
                texCoord = texCoordIn;
            })",
        {
            new Layout(0, 3, GL_FLOAT),
            new Layout(1, 4, GL_FLOAT),
            new Layout(2, 2, GL_FLOAT)
        }
    };

    inline auto position_texture = new VertexSource {
        R"(#version 330 core

            layout (location = 0) in vec3 positionIn;
            layout (location = 1) in vec2 texCoordIn;

            uniform mat4 projection;
            uniform mat4 model;
            uniform mat4 view;

            out vec2 texCoord;

            void main() {
                gl_Position = projection * view * model * vec4(positionIn, 1.0);
                texCoord = texCoordIn;
            })",
        {
            new Layout(0, 3, GL_FLOAT),
            new Layout(1, 2, GL_FLOAT)
        }
    };

    inline auto position_texture_nmvp = new VertexSource {
        R"(#version 330 core

            layout (location = 0) in vec3 positionIn;
            layout (location = 1) in vec2 texCoordIn;

            out vec2 texCoord;

            void main() {
                gl_Position = vec4(positionIn, 1.0);
                texCoord = texCoordIn;
            })",
        {
            new Layout(0, 3, GL_FLOAT),
            new Layout(1, 2, GL_FLOAT)
        }
    };
}

#endif //VERTEXSOURCES_H
