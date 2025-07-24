//
// Created by ruhld on 22.05.2025.
//

#ifndef RENDERMODULE_H
#define RENDERMODULE_H

#include "../SceneModule.h"
#include "../../../Camera.h"
#include "../../../event/Event.h"
#include "../../../event/SceneEvents.h"

class FrameBuffer;

class RenderModule final : public SceneModule {
public:

    Camera* camera = nullptr;

    explicit RenderModule(const Scene *scene);

    void move();

    void render_scene() const;

    void render();

    void onEvent(Event* event) override {
        if (event->is<SceneRenderEvent>()) {
            render();
        }
    }
};

#endif //RENDERMODULE_H
