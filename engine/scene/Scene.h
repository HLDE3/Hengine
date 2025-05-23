//
// Created by ruhld on 22.05.2025.
//

#ifndef SCENE_H
#define SCENE_H
#include <thread>
#include <vector>

#include "event/EventListener.h"
#include "GameObject.h"

class Event;

struct scene_data {
    int tps;
};

class Scene : public EventListener, public ModuleContainer {
private:
    scene_data data;
public:

    std::vector<std::thread> threads;
    bool running = true;
    std::vector<GameObject*> gameObjects;

    explicit Scene(scene_data data) : data(data) {}
    ~Scene() override;

    void init();

    void onEvent(Event *event) override;

    GameObject * createGameObject();
    void removeGameObject(GameObject * game_object);
};

#endif //SCENE_H
