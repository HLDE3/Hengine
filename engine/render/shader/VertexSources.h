//
// Created by ruhld on 22.05.2025.
//

#ifndef VERTEXSOURCES_H
#define VERTEXSOURCES_H
#include <vector>
#include <GL/glew.h>

namespace VertexSources {

    struct Layout {
        int location;
        int count;
        int type;
        int normalized;
    };

    struct VertexSource {
        const char* source;
        std::vector<Layout> layouts;
    };

    inline auto position_color_texture = new VertexSource {
        R"(#version 330 core"

            layout (location = 0) in vec3 position;
            layout (location = 1) in vec4 color;
            layout (location = 2) in vec2 texCoord;

            out vec4 outColor;
            out vec2 outTexCoord;

            void main() {
                gl_Position = vec4(position, 1.0);
                outColor = color;
                outTexCoord = texCoord;
            })",
        {
            {0, 3, GL_FLOAT, GL_FALSE},
            {1, 4, GL_FLOAT, GL_FALSE},
            {2, 2, GL_FLOAT, GL_FALSE}
        }
    };

    inline auto position_color = new VertexSource {
        R"(#version 330 core

            layout (location = 0) in vec3 position;
            layout (location = 1) in vec4 color;

            out vec4 outColor;

            void main() {
                gl_Position = vec4(position, 1.0);
                outColor = color;
            })",
        {
            {0, 3, GL_FLOAT, GL_FALSE},
            {1, 4, GL_FLOAT, GL_FALSE}
        }
    };

    inline auto position = new VertexSource {
        R"(#version 330 core"

            layout (location = 0) in vec3 position;

            void main() {
                gl_Position = vec4(position, 1.0);
            })",
        {
            {0, 3, GL_FLOAT, GL_FALSE}
        }
    };

    inline auto position_color_pvm = new VertexSource {
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
            {0, 3, GL_FLOAT, GL_FALSE},
            {1, 4, GL_FLOAT, GL_FALSE}
        }
    };

}

#endif //VERTEXSOURCES_H
