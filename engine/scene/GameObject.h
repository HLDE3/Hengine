//
// Created by ruhld on 22.05.2025.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "module/ModuleContainer.h"

class Scene;

class GameObject : public EventListener, public ModuleContainer {
private:
    Scene* scene;
public:
    explicit GameObject(Scene* scene) : scene(scene) {}

    ~GameObject() override = default;

    void onEvent(Event *event) override {
        for (auto module : modules) module->onEvent(event);
    }
};

#endif //GAMEOBJECT_H
