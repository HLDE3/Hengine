//
// Created by ruhld on 22.05.2025.
//

#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

class Event;

class EventListener {
public:

    virtual ~EventListener() = default;
    virtual void onEvent(Event *event) {};
};

#endif //EVENTLISTENER_H
