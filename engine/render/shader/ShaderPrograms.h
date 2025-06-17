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

    inline Shader* bloom_extract;

    inline Shader* bloom_blur;

    inline Shader* bloom_combine;

    inline void init() {

        // bloom_extract = new Shader(
        //     VertexSources::position_texture_nmvp,
        //     R"(#version 330 core
        //         out vec4 outColor;
        //         in vec2 texCoord;
        //         uniform sampler2D tex;
        //         void main() {
        //             vec3 c = texture(tex, texCoord).rgb;
        //             float brightness = dot(c, vec3(0.2126, 0.7152, 0.0722));
        //             outColor = brightness > 0.8 ? vec4(c, 1.0) : vec4(0.0);
        //         }
        //     )"
        // );

        bloom_extract = new Shader(
            VertexSources::position_texture_nmvp,
            R"(#version 330 core
                in vec2 texCoord;
                out vec4 outColor;
                uniform sampler2D tex;
                void main() {
                    vec3 c = texture(tex, texCoord).rgb;
                    float brightness = dot(c, vec3(0.2126, 0.7152, 0.0722));
                    outColor = brightness > 1.0 ? vec4(c, 1.0) : vec4(0.0);
                }
            )"
        );

        bloom_blur = new Shader(
            VertexSources::position_texture_nmvp,
            R"(#version 330 core
        out vec4 outColor;
        in vec2 texCoord;
        uniform sampler2D tex;
        uniform bool horizontal;
        void main() {
            float weight[9] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216, 0.004, 0.002, 0.001, 0.0005);
            vec2 tex_offset = 1.0 / textureSize(tex, 0);
            vec3 result = texture(tex, texCoord).rgb * weight[0];
            for(int i = 1; i < 9; ++i) {
                if(horizontal) {
                    vec2 offset = vec2(tex_offset.x * i, 0.0);
                    if(texCoord.x + offset.x <= 1.0 && texCoord.x + offset.x >= 0.0)
                        result += texture(tex, texCoord + offset).rgb * weight[i];
                    if(texCoord.x - offset.x <= 1.0 && texCoord.x - offset.x >= 0.0)
                        result += texture(tex, texCoord - offset).rgb * weight[i];
                } else {
                    vec2 offset = vec2(0.0, tex_offset.y * i);
                    if(texCoord.y + offset.y <= 1.0 && texCoord.y + offset.y >= 0.0)
                        result += texture(tex, texCoord + offset).rgb * weight[i];
                    if(texCoord.y - offset.y <= 1.0 && texCoord.y - offset.y >= 0.0)
                        result += texture(tex, texCoord - offset).rgb * weight[i];
                }
            }
            outColor = vec4(result, 1.0);
        }
    )"
        );

        bloom_combine = new Shader(
            VertexSources::position_texture_nmvp,
            R"(#version 330 core
                out vec4 outColor;
                in vec2 texCoord;
                uniform sampler2D tex1;
                uniform sampler2D tex2;
                void main() {
                    vec3 col1 = texture(tex1, texCoord).rgb;
                    vec3 col2 = texture(tex2, texCoord).rgb;
                    outColor = vec4(col1 + col2, 1.0);
                }
            )"
        );

        position_color = new Shader(
            VertexSources::position_color,
            R"(#version 330 core
                in vec4 color;
                out vec4 outColor;

                void main() {
                    outColor = color;
                }
            )"
        );

        position_color_texture = new Shader(
            VertexSources::position_color_texture,
            R"(#version 330 core
                in vec4 color;
                in vec2 texCoord;
                out vec4 outColor;
                uniform sampler2D tex;
                void main() {
                    outColor = texture(tex, vec2(texCoord.x, 1.0 - texCoord.y)) * color;
                }
            )"
        );

        position_texture = new Shader(
            VertexSources::position_texture,
            R"(#version 330 core
                in vec2 texCoord;
                out vec4 outColor;
                uniform sampler2D tex;
                void main() {
                    outColor = texture(tex, texCoord);
                }
            )"
        );
    }
}

#endif //SHADERPROGRAMS_H
