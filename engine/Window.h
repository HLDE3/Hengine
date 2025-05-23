//
// Created by ruhld on 22.05.2025.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:

    GLFWwindow* window = nullptr;
    int x, y, width, height;
    const char* title;
    Window(float width, float height, const char* title);

    void init();

    void update();
};



#endif //WINDOW_H
