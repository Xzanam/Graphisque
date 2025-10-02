//
// Created by Janam Shrestha on 10/2/25.
//

#ifndef LAYER_H
#define LAYER_H
#include "Event.h"

namespace  Core {
    class Layer {
    public :
        virtual ~Layer() = default;
        virtual void OnEvent(Event& event) {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}

    };
}

#endif //LAYER_H
