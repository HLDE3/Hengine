//
// Created by ruhld on 22.05.2025.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "glm.hpp"

class Window;

class Camera {
public:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;

    float fov = 45.0f;

    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float fov);

    [[nodiscard]] glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix(Window* window) const;

};



#endif //CAMERA_H
