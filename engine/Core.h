//
// Created by ruhld on 22.05.2025.
//

#ifndef CORE_H
#define CORE_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"

class Core {
private:
    Core() = default;

    Core(const Core&);
    Core(Core&&) = delete;
    Core& operator=(const Core&) = delete;
    Core& operator=(Core&&) = delete;

public:

    Window* window = nullptr;

    static Core& getInstance() {
        static Core instance;
        return instance;
    }

    int init();

    ~Core() = default;
};



#endif //CORE_H
