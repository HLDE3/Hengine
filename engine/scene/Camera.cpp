//
// Created by ruhld on 22.05.2025.
//

#include "Camera.h"

#include <ext/matrix_clip_space.hpp>
#include <ext/matrix_transform.hpp>
#include "../Window.h"

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float fov)
    : position(position), direction(direction), up(up), fov(fov) {

}

[[nodiscard]] glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + direction, up);
}

glm::mat4 Camera::getProjectionMatrix(Window* window) const {
    auto aspect_ratio = static_cast<float>(window->width) / static_cast<float>(window->height);
    return glm::perspective(glm::radians(fov), aspect_ratio, 0.001f, 1000.0f);
}