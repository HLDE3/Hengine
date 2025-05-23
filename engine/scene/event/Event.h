//
// Created by ruhld on 22.05.2025.
//

#ifndef EVENT_H
#define EVENT_H
#include <typeinfo>


class Event {
public:
    virtual ~Event() = default; // Добавьте эту строку

    template <typename T>
    [[nodiscard]] bool is() const {
        return dynamic_cast<const T*>(this) != nullptr;
    }
};
#endif //EVENT_H
