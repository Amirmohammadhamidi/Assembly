#ifndef MouseController_hpp
#define MouseController_hpp
#include "../Game.hpp"
#include "ECS.hpp"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "vector2D.hpp"
#include <iostream>

class MouseController : public Component
{
public:
    TransformComponent *transform;
    bool active;
    const vector2D *xBound;
    const vector2D *yBound;

    MouseController(const vector2D *Xvec, const vector2D *Yvec)
    {
        xBound = Xvec;
        yBound = Yvec;
    }
    ~MouseController() {

    };

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
            if ((xBound->x < x) && (x < xBound->y) && (yBound->x < y) && (y < yBound->y))
            {
                active = true;
            }
            transform->position.x = x;
            transform->position.y = y;
        }
    }

    bool isActive() { return active; }
};

#endif