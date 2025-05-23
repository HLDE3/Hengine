//
// Created by ruhld on 22.05.2025.
//

#include "SceneManager.h"

SceneManager::SceneManager() = default;

SceneManager::~SceneManager() = default;

Scene * SceneManager::createScene(scene_data data) {
    auto scene = new Scene(data);
    this->scenes.push_back(scene);;
    scene->init();
    return scene;
}

void SceneManager::onEvent(Event *event) {
    for (auto scene : this->scenes) {
        scene->onEvent(event);
    }
}
