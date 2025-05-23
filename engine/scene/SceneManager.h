//
// Created by ruhld on 22.05.2025.
//

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <vector>

#include "Scene.h"
#include "event/EventListener.h"

struct scene_data;

class SceneManager : public EventListener {
private:
    std::vector<Scene*> scenes;
public:
    SceneManager();
    ~SceneManager() override;

    Scene * createScene(scene_data data);
    void onEvent(Event* event) override;
};



#endif //SCENEMANAGER_H
