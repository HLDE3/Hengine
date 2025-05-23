//
// Created by ruhld on 22.05.2025.
//

#ifndef MODULE_H
#define MODULE_H

#include "../event/Event.h"
#include "../event/EventListener.h"

class GameObject;

class Module : public EventListener {
public:
     virtual ~Module() = default;

     virtual void onEvent(Event *event) override {}
};

#endif //MODULE_H
