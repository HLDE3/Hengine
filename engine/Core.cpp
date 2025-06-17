//
// Created by ruhld on 22.05.2025.
//

#include "Core.h"

#include "render/FrameBuffer.h"
#include "render/shader/ShaderPrograms.h"
#include "scene/SceneManager.h"
#include "scene/event/SceneEvents.h"
#include "scene/module/impl/scene/RenderModule.h"

int Core::init() {

    window = new Window(1280, 720, "CLionProjects");
    window->init();

    ShaderPrograms::init();

    auto manager = new SceneManager();
    auto scene = manager->createScene({60});

    auto module = new RenderModule(scene);
    scene->modules.emplace_back(module);

    while (!glfwWindowShouldClose(window->window))
    {
        window->update();
        manager->onEvent(new SceneRenderEvent(scene));
    }

    glfwTerminate();
    return 0;
}
