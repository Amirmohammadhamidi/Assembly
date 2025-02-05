#ifndef MouseController_hpp
#define MouseController_hpp
#include "../Game.hpp"
#include "ECS.hpp"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include <iostream>

class MouseController : public Component
{
public:
    TransformComponent *transform;
    bool clicked = false;
    void init() override

    {
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
        if (Game::event.type == SDL_MOUSEBUTTONDOWN)
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            transform->position.x = x;
            transform->position.y = y;
        }
    }

    MouseController() {

    };
    ~MouseController() {

    };
};

#endif