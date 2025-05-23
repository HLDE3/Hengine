//
// Created by ruhld on 22.05.2025.
//

#include "Window.h"

#include <cstdio>
#include <iostream>
#include <wchar.h>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

void setup_window_hints() {
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
}


GLFWwindow* create_window(int width, int height, const char* title) {
    setup_window_hints();
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create window" << std::endl;
    }
    return window;
}

bool initialize_glfw_and_context(GLFWwindow*& window, int width, int height, const char* title) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    window = create_window(width, height, title);
    if (!window) {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    return true;
}

bool initialize_glew() {
    glewExperimental = GL_TRUE; // На некоторых системах требуется для корректной работы
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to init GLEW" << std::endl;
        return false;
    }
    return true;
}


Window::Window(float width, float height, const char* title) : x(0), y(0), width(width), height(height), title(title) {
}

void Window::init() {

    initialize_glfw_and_context(window, 1280, 720, "OpenGL");
    initialize_glew();

}

void Window::update() {
    glfwGetWindowSize(window, &width, &height);
    glfwGetWindowPos(window, &x, &y);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

