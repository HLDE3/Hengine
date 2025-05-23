//
// Created by ruhld on 22.05.2025.
//

#ifndef SCENEEVENTS_H
#define SCENEEVENTS_H

#include "Event.h"

class Scene;

class SceneEvent : public Event {
public:
    Scene* scene;
    explicit SceneEvent(Scene* scene) : scene(scene) {}
};

class SceneRenderEvent : public SceneEvent {
public:
    explicit SceneRenderEvent(Scene* scene) : SceneEvent(scene) {}
};

#endif //SCENEEVENTS_H
