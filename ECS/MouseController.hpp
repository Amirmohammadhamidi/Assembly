#ifndef MouseController_hpp
#define MouseController_hpp
#include "../Game.hpp"
#include "ECS.hpp"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "../vector2D.hpp"
#include <iostream>

class MouseController : public Component
{
private:
    bool active = false;
    const vector2D *xBound;
    const vector2D *yBound;
    vector2D *clickedPosition = new vector2D();

public:
    TransformComponent *transform;

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
        if (Game::event.type == SDL_MOUSEBUTTONDOWN && (!entity->isLock()))
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if ((xBound->x < x) && (x < xBound->y) && (yBound->x < y) && (y < yBound->y))
            {
                activate();
                clickedPosition->x = x;
                clickedPosition->y = y;
            }
        }
    }
    vector2D *getClickedPosition() { return clickedPosition; }
    bool isActive() { return active; }
    void activate() { active = true; }
    void deActivate() { active = false; }
};

#endif