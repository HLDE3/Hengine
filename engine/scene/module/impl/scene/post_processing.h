//
// Created by ruhld on 24.05.2025.
//

#pragma once

#include <functional>

#include "../../../../Core.h"
#include "../../../../render/FrameBuffer.h"
#include "../../../../render/shader/ShaderPrograms.h"
#include "../../../../render/BufferBuilder.h"


namespace post_processing {

    inline float exposure = 1.0f;
    inline float nv_saturation = 1.0f;

    inline Shader* night_vision_shader() {
        static auto shader = new Shader(
            VertexSources::position_texture_nmvp,
            R"(#version 330 core
                in vec2 texCoord;
                out vec4 outColor;
                uniform sampler2D tex;
                uniform float time;
                uniform float sat;
                float rand(vec2 co) {
                    return fract(sin(dot(co.xy ,vec2(12.9898,78.233)) + time * 100.0) * 43758.5453);
                }
                void main() {
                    vec3 color = texture(tex, texCoord).rgb;
                    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
                    float blue = brightness * 1.0;
                    float green = brightness * 0.8;
                    float red = brightness * 0.6;
                    float noise = rand(texCoord * 100.0);
                    outColor = vec4(vec3(red + noise * 0.04, green + noise * 0.08, blue + noise * 0.1) * sat, 1.0);
                }
            )"
        );
        return shader;
    }


    inline Shader* hdr_shader() {
        static auto shader = new Shader(
            VertexSources::position_texture_nmvp,
            R"(#version 330 core
                in vec2 texCoord;
                out vec4 outColor;
                uniform sampler2D tex;
                uniform float exposure;
                void main() {
                    const float gamma = 2.2;
                    vec3 hdrColor = texture(tex, texCoord).rgb;

                    // exposure tone mapping
                    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
                    // gamma correction
                    mapped = pow(mapped, vec3(1.0 / gamma));

                    outColor = vec4(mapped, 1.0);
                }
            )"
        );
        return shader;
    }

    inline std::vector<std::pair<const char*, std::function<void(FrameBuffer*)>>> functions = {
        {
            "HDR",
            [](FrameBuffer* framebufferIn) {


                auto window = Core::getInstance().window;

                static auto framebuffer = new FrameBuffer();

                framebuffer->resize(window->width, window->height);

                auto shader = hdr_shader();

                framebuffer->bind_write();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glUseProgram(shader->shader_program);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, framebufferIn->texture);
                shader->setUniform1i("tex", 0);
                shader->setUniform1f("exposure", exposure);
                auto buffer = new BufferBuilder(shader->vertex_source);
                buffer->position(-1, -1, 0)->uv(0, 0)->next();
                buffer->position(1, -1, 0)->uv(1, 0)->next();
                buffer->position(1, 1, 0)->uv(1, 1)->next();
                buffer->position(-1, 1, 0)->uv(0, 1)->next();
                buffer->build();
                buffer->draw(GL_TRIANGLE_FAN);

                delete buffer;
                glUseProgram(0);
                framebuffer->unbind_write();

                framebufferIn->bind_write();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                framebuffer->blit();
                framebufferIn->unbind_write();

            }
        },
        {
            "NIGHT_VISION",
            [](FrameBuffer* framebufferIn) {
                if (true) return;

                auto window = Core::getInstance().window;
                static auto fbo = new FrameBuffer(window->width, window->height);
                fbo->resize(window->width, window->height);

                auto shader = night_vision_shader();

                if (glfwGetKey(window->window, GLFW_KEY_E) == 1) {
                    nv_saturation += 0.1;
                } else if (glfwGetKey(window->window, GLFW_KEY_Q) == 1) {
                    nv_saturation -= 0.1;
                }

                fbo->bind_write();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                framebufferIn->bind_read();

                glUseProgram(shader->shader_program);
                shader->setUniform1i("tex", 0);
                shader->setUniform1f("time", glfwGetTime());
                shader->setUniform1f("sat", nv_saturation);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, framebufferIn->texture);

                auto buffer = new BufferBuilder(shader->vertex_source);
                buffer->position(-1, -1, 0)->uv(0, 0)->next();
                buffer->position(1, -1, 0)->uv(1, 0)->next();
                buffer->position(1, 1, 0)->uv(1, 1)->next();
                buffer->position(-1, 1, 0)->uv(0, 1)->next();
                buffer->build();
                buffer->draw(GL_TRIANGLE_FAN);
                delete buffer;

                glUseProgram(0);

                framebufferIn->unbind_read();
                fbo->unbind_write();


                framebufferIn->bind_write();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                fbo->blit();
                framebufferIn->unbind_write();
            }
        },
        {
            "BLOOM",
            [](FrameBuffer* framebufferIn) {

                auto window = Core::getInstance().window;

                static auto extractFBO = new FrameBuffer(window->width, window->height);
                extractFBO->resize(window->width, window->height);

                static auto blurFBO = new FrameBuffer*[2] {new FrameBuffer(window->width, window->height), new FrameBuffer(window->width, window->height)};

                blurFBO[0]->resize(window->width, window->height);
                blurFBO[1]->resize(window->width, window->height);

                static auto bloomFBO = new FrameBuffer(window->width, window->height);
                bloomFBO->resize(window->width, window->height);

                static auto resultFBO = new FrameBuffer(window->width, window->height);
                resultFBO->resize(window->width, window->height);

                extractFBO->bind_write();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                {
                    auto shader = ShaderPrograms::bloom_extract;
                    glUseProgram(shader->shader_program);
                    shader->setUniform1i("tex", 0);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, framebufferIn->texture);
                    // Нарисовать fullscreen quad
                    auto buffer = new BufferBuilder(shader->vertex_source);

                    buffer->position(-1, -1, 0)->uv(0, 0)->next();
                    buffer->position(1, -1, 0)->uv(1, 0)->next();
                    buffer->position(1, 1, 0)->uv(1, 1)->next();
                    buffer->position(-1, 1, 0)->uv(0, 1)->next();
                    buffer->build();

                    buffer->draw(GL_TRIANGLE_FAN);

                    delete buffer;

                    glUseProgram(0);
                }
                extractFBO->unbind_write();

                auto horizontal = true;
                for (unsigned int i = 0; i < 10; ++i) {
                    blurFBO[horizontal]->bind_write();
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    {
                        auto shader = ShaderPrograms::bloom_blur;
                        glUseProgram(shader->shader_program);
                        shader->setUniform1i("tex", 0);
                        shader->setUniform1i("horizontal", horizontal);
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, i == 0 ? extractFBO->texture : blurFBO[!horizontal]->texture);

                        // Нарисовать fullscreen quad
                        auto buffer = new BufferBuilder(shader->vertex_source);

                        buffer->position(-1, -1, 0)->uv(0, 0)->next();
                        buffer->position(1, -1, 0)->uv(1, 0)->next();
                        buffer->position(1, 1, 0)->uv(1, 1)->next();
                        buffer->position(-1, 1, 0)->uv(0, 1)->next();
                        buffer->build();

                        buffer->draw(GL_TRIANGLE_FAN);

                        delete buffer;

                        glUseProgram(0);
                        horizontal = !horizontal; // Переключаем направление размытия
                    }
                    blurFBO[!horizontal]->unbind_write();
                }

                 bloomFBO->bind_write();
                 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                 {
                     auto shader = ShaderPrograms::bloom_combine;

                     glUseProgram(shader->shader_program);
                     shader->setUniform1i("tex1", 0);
                     shader->setUniform1i("tex2", 1);
                     shader->setUniform1f("exposure", exposure);
                     glActiveTexture(GL_TEXTURE0);
                     glBindTexture(GL_TEXTURE_2D, blurFBO[0]->texture);

                     glActiveTexture(GL_TEXTURE1);
                     glBindTexture(GL_TEXTURE_2D, blurFBO[1]->texture);

                     // Нарисовать fullscreen quad
                     auto buffer = new BufferBuilder(shader->vertex_source);

                     buffer->position(-1, -1, 0)->uv(0, 0)->next();
                     buffer->position(1, -1, 0)->uv(1, 0)->next();
                     buffer->position(1, 1, 0)->uv(1, 1)->next();
                     buffer->position(-1, 1, 0)->uv(0, 1)->next();
                     buffer->build();

                     buffer->draw(GL_TRIANGLE_FAN);

                     delete buffer;

                     glUseProgram(0);
                 }
                 bloomFBO->unbind_write();


                 resultFBO->bind_write();
                 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                 {
                     auto shader = ShaderPrograms::bloom_combine;
                     glUseProgram(shader->shader_program);
                     shader->setUniform1i("tex1", 0);
                     shader->setUniform1i("tex2", 1);
                     glActiveTexture(GL_TEXTURE0);
                     glBindTexture(GL_TEXTURE_2D, framebufferIn->texture);

                     glActiveTexture(GL_TEXTURE1);
                     glBindTexture(GL_TEXTURE_2D, bloomFBO->texture);

                     // Нарисовать fullscreen quad
                     auto buffer = new BufferBuilder(shader->vertex_source);

                     buffer->position(-1, -1, 0)->uv(0, 0)->next();
                     buffer->position(1, -1, 0)->uv(1, 0)->next();
                     buffer->position(1, 1, 0)->uv(1, 1)->next();
                     buffer->position(-1, 1, 0)->uv(0, 1)->next();
                     buffer->build();

                     buffer->draw(GL_TRIANGLE_FAN);

                     delete buffer;

                     glUseProgram(0);
                 }
                resultFBO->unbind_write();



                framebufferIn->bind_write();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                resultFBO->blit();
                framebufferIn->unbind_write();

            }
        }
    };


}