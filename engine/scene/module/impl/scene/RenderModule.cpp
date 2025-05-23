//
// Created by ruhld on 22.05.2025.
//

#include "RenderModule.h"

#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

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

void RenderModule::render() {


    auto window = Core::getInstance().window;
    static auto framebuffer = new FrameBuffer(window->width, window->height);
    if (framebuffer->width != window->width || framebuffer->height != window->height)
        framebuffer->resize(window->width, window->height);

    glEnable(GL_DEPTH_TEST);

    glfwPollEvents();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    int display_w, display_h;
    glfwGetFramebufferSize(window->window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);


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


    auto model = glm::mat4(1.0f);
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjectionMatrix(window);

    auto shader = ShaderPrograms::position_color_pvm;

    glUseProgram(shader->shaderProgram);

    shader->setUniform4x4f("model", glm::value_ptr(model));

    shader->setUniform4x4f("view", glm::value_ptr(view));

    shader->setUniform4x4f("projection", glm::value_ptr(projection));

    // Пример изменения цвета (или других данных) каждый кадр
    auto buffer = new BufferBuilder(shader->vertexSource);
    framebuffer->bind_write();


    buffer->clear();
    // Нижняя грань (z = -0.5)
    buffer->vertex(0)->position(-0.5f, -0.5f, -0.5f)->color(1.0, 0.0, 0.0, 1.0)->next();
    buffer->position(0.5f, -0.5f, -0.5f)->color(0.0, 1.0, 0.0, 1.0)->next();
    buffer->position(0.5f, 0.5f, -0.5f)->color(0.0, 0.0, 1.0, 1.0)->next();
    buffer->position(-0.5f, 0.5f, -0.5f)->color(1.0, 1.0, 0.0, 1.0)->next();
    buffer->update();
    buffer->draw(GL_TRIANGLE_FAN);

    buffer->clear();
    // Нижняя грань (z = -0.5)
    buffer->vertex(0)->position(-0.5f, -0.5f, 0.5f)->color(.5, 0.5, 0.5, 1.0)->next();
    buffer->position(0.5f, -0.5f, 0.5f)->color(0.5, .5, 0.5, 1.0)->next();
    buffer->position(0.5f, 0.5f, 0.5f)->color(0.5, 0.5, .5, 1.0)->next();
    buffer->position(-0.5f, 0.5f, 0.5f)->color(.5, .5, 0.5, 1.0)->next();
    buffer->update();
    buffer->draw(GL_TRIANGLE_FAN);


    framebuffer->unbind_write();

    // Привязка фреймбуфера для чтения и копирование содержимого на экран
    framebuffer->bind_read();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Назначаем экран как draw framebuffer
    glBlitFramebuffer(
        0, 0, framebuffer->width, framebuffer->height, // src rect
        0, 0, framebuffer->width, framebuffer->height, // dst rect
        GL_COLOR_BUFFER_BIT, GL_NEAREST
    );
    framebuffer->unbind_read();
    framebuffer->clear();


    glfwSwapBuffers(window->window);
}
