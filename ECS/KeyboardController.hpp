#ifndef KeyboardController_hpp
#define KeyboardController_hpp
#include "../Game.hpp"
#include "ECS.hpp"
#include "TransformComponent.hpp"

class KeyboardController : public Component
{
public:
    TransformComponent *transform;

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override;
    KeyboardController() {

    };
    ~KeyboardController() {

    };
};

#endif