//
// Created by ruhld on 22.05.2025.
//

#ifndef SCENEMODULE_H
#define SCENEMODULE_H
#include "../Module.h"


class Event;
class Scene;

class SceneModule : public Module {
public:
    const Scene * scene;
    explicit SceneModule(const Scene * scene) : scene(scene) {}

    ~SceneModule() override = default;
    virtual void onEvent(Event *event) override {}
};


#endif //SCENEMODULE_H
