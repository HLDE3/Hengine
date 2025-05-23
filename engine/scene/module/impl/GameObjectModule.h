//
// Created by ruhld on 22.05.2025.
//

#ifndef GAMEOBJECTMODULE_H
#define GAMEOBJECTMODULE_H
#include "../Module.h"
#include "../../event/Event.h"

class GameObjectModule : public Module {
public:
    const GameObject * game_object;
    explicit GameObjectModule(const GameObject * game_object) : game_object(game_object) {}

    virtual ~Module() override = default;
    virtual void onEvent(Event *event) override {}
};

#endif //GAMEOBJECTMODULE_H
