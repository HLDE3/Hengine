//
// Created by ruhld on 22.05.2025.
//

#include "RenderModule.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "../../../../utils/stb_image.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "post_processing.h"
#include "../../../../Core.h"
#include "../../../../render/BufferBuilder.h"
#include "../../../../render/FrameBuffer.h"
#include "../../../../render/shader/Shader.h"
#include "../../../../render/shader/ShaderPrograms.h"


RenderModule::RenderModule(const Scene *scene): SceneModule(scene) {
    camera = new Camera(
        {0, 0, 0},
        {0, 0, -1},
        {0, 1, 0},
        45.0f
    );
}
GLuint loadTexture(const char* path) {
    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 4);
    if (!data) {
        std::cerr << "Не удалось загрузить картинку: " << path << std::endl;
        return 0;
    }
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    return tex;
}

void RenderModule::move() {

    auto window = Core::getInstance().window;

    static float yaw = -90.0f;
    static float pitch = 0.0f;
    // C++
    float cameraSpeed = 0.01f;
    float sensitivity = 1.0f; // чувствительность поворота
    GLFWwindow* win = window->window;

    // --- Движение ---
    glm::vec3 right = glm::normalize(glm::cross(camera->direction, camera->up));
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) camera->position += cameraSpeed * camera->direction;
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) camera->position -= cameraSpeed * camera->direction;
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) camera->position += right * cameraSpeed;
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) camera->position -= right * cameraSpeed;
    if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) camera->position.y += cameraSpeed;
    if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera->position.y -= cameraSpeed;

    // --- Поворот камеры стрелками ---
    if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS)    pitch += sensitivity;
    if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS)  pitch -= sensitivity;
    if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS)  yaw   -= sensitivity;
    if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) yaw   += sensitivity;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Пересчёт направления взгляда
    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera->direction = glm::normalize(dir);
}

void resize_buffer(FrameBuffer *buffer) {
    auto window = Core::getInstance().window;
    int width = window->width, height = window->height;
    if (buffer->width != width || buffer->height != height) {
        buffer->resize(width, height);
    }
}

void RenderModule::render_scene() const {

    auto window = Core::getInstance().window;

    static GLuint tex = loadTexture("C:/Users/ruhld/CLionProjects/Hengine/assets/bob.jpg");

    auto model = glm::mat4(1.0f);
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjectionMatrix(window);

    {
        auto shader = ShaderPrograms::position_color;

        glUseProgram(shader->shader_program);

        shader->setUniform4x4f("model", glm::value_ptr(model));

        shader->setUniform4x4f("view", glm::value_ptr(view));

        shader->setUniform4x4f("projection", glm::value_ptr(projection));

        auto buffer = new BufferBuilder(shader->vertex_source);

        buffer->position(-5.5f, -5.5f, 5.5f)->color(1.0, 0.0, 0.0, 1.0)->next();
        buffer->position(5.5f, -5.5f, 5.5f)->color(0.0, 1.0, 0.0, 1.0)->next();
        buffer->position(5.5f, 5.5f, 5.5f)->color(0.0, 0.0, 1.0, 1.0)->next();
        buffer->position(-5.5f, 5.5f, 5.5f)->color(1.0, 1.0, 0.0, 1.0)->next();
        buffer->build();
        buffer->draw(GL_TRIANGLE_FAN);

        delete buffer;

        glUseProgram(0);
    }

    {
        auto shader = ShaderPrograms::position_color_texture;

        glUseProgram(shader->shader_program);

        shader->setUniform4x4f("model", glm::value_ptr(model));

        shader->setUniform4x4f("view", glm::value_ptr(view));

        shader->setUniform4x4f("projection", glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        shader->setUniform1i("tex", 0);

        auto buffer = new BufferBuilder(shader->vertex_source);

        buffer->position(-0.5f, -0.5f, -0.5f)->color(1.0, 0.0, 0.0, 1.0)->uv(0.0f, 0.0f)->next();
        buffer->position(0.5f, -0.5f, -0.5f)->color(0.0, 1.0, 0.0, 1.0)->uv(1.0f, 0.0f)->next();
        buffer->position(0.5f, 0.5f, -0.5f)->color(0.0, 0.0, 1.0, 1.0)->uv(1.0f, 1.0f)->next();
        buffer->position(-0.5f, 0.5f, -0.5f)->color(1.0, 1.0, 0.0, 1.0)->uv(0.0f, 1.0f)->next();
        buffer->build();
        buffer->draw(GL_TRIANGLE_FAN);

        delete buffer;

        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    }
}


void RenderModule::render() {

    auto window = Core::getInstance().window;

    static GLuint tex = loadTexture("C:/Users/ruhld/CLionProjects/Hengine/assets/bob.jpg");

    static auto framebuffer = new FrameBuffer(window->width, window->height);
    resize_buffer(framebuffer);

    static auto result = new FrameBuffer(window->width, window->height);
    if (result->width != window->width || result->height != window->height)
        result->resize(window->width, window->height);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ZERO);


    glfwPollEvents();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    int display_w, display_h;
    glfwGetFramebufferSize(window->window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    move();

    framebuffer->bind_write();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_scene();

    framebuffer->unbind_write();

    for (auto function : post_processing::functions) {
        function.second(framebuffer);
    }

    framebuffer->blit();

    //post_processing(framebuffer);

    glfwSwapBuffers(window->window);
}
