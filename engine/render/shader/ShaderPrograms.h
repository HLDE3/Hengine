//
// Created by ruhld on 22.05.2025.
//

#ifndef SHADERPROGRAMS_H
#define SHADERPROGRAMS_H

#include "Shader.h"
#include "VertexSources.h"

namespace ShaderPrograms {


    inline Shader* position_color;

    inline Shader* position_color_texture;

    inline Shader* position_texture;

    inline void init() {

        position_color = new Shader(
            VertexSources::position_color,
            R"(#version 330 core
                in vec4 outColor;
                out vec4 color;

                void main() {
                    color = outColor;
                }
            )"
        );

        position_color_texture = new Shader(
            VertexSources::position_color_texture,
            R"(#version 330 core
                in vec4 outColor;
                in vec2 outTexCoord;
                out vec4 color;
                uniform sampler2D tex;
                void main() {
                    color = texture(tex, outTexCoord) * outColor;
                }
            )"
        );

        position_color_texture = new Shader(
        VertexSources::position_texture,
        R"(#version 330 core
                in vec2 outTexCoord;
                out vec4 color;
                uniform sampler2D tex;
                void main() {
                    color = texture(tex, outTexCoord);
                }
            )"
        );
    }
}

#endif //SHADERPROGRAMS_H
