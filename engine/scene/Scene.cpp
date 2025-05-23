//
// Created by ruhld on 22.05.2025.
//

#include "Scene.h"
#include <thread>
#include <algorithm>
#include <iostream>

#include "event/SceneEvents.h"

Scene::~Scene() {
    for (auto& thread : threads) {
        if (thread.joinable()) thread.detach();
    }
    threads.clear();
    for (const auto game_object : gameObjects) {
        game_object->~GameObject();
    }
    gameObjects.clear();
}

void Scene::init() {

    threads.emplace_back([this]() {
        while (this->running) {
            auto start = std::chrono::steady_clock::now();

            //onEvent(new SceneRenderEvent(this));

            auto end = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            auto frameTime = std::chrono::milliseconds(1000 / this->data.tps);
            if (elapsed < frameTime) {
                std::this_thread::sleep_for(frameTime - elapsed);
            }
        }
    });
}

void Scene::onEvent(Event *event) {
    for (auto module : modules) {
        module->onEvent(event);
    }
    for (auto game_object : gameObjects) game_object->onEvent(event);
}

GameObject * Scene::createGameObject() {
    auto gameObject = new GameObject(this);
    gameObjects.push_back(gameObject);
    return gameObject;
}

void Scene::removeGameObject(GameObject * game_object) {
    if (const auto it = std::ranges::find(gameObjects, game_object); it != gameObjects.end()) {
        delete *it;
        gameObjects.erase(it);
    }
}
