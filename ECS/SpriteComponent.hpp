#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_HPP

#include "TransformComponent.hpp"
#include "SDL2/SDL.h"
#include "../TextureManager.hpp"
#include <iostream>
class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;
    int width, height;
    float scale;
    bool centerize = false;
    bool lock = false;

public:
    SpriteComponent() = default;
    SpriteComponent(const char *path, float scale)
    {
        setTexture(path);
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        this->scale = scale;
    }
    SpriteComponent(const char *path, float scale, bool centerize)
    {
        setTexture(path);
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        this->scale = scale;
        this->centerize = true;
    }
    void setTexture(const char *path)
    {
        texture = TextureManager::loadTexture(path);
    }
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        srcRect.x = srcRect.y = 0;
        srcRect.w = width;
        srcRect.h = height;
        destRect.w = (int)(width * scale);
        destRect.h = (int)(height * scale);
    }

    void update() override
    {
        if (!lock)
        {
            if (centerize)
            {
                destRect.x = (int)(transform->position.x - (0.5 * destRect.w));
                destRect.y = (int)(transform->position.y - (0.5 * destRect.h));
            }
            else
            {
                destRect.x = (int)transform->position.x;
                destRect.y = (int)transform->position.y;
            }
        }
    }

    void draw()
    {
        TextureManager::Draw(texture, srcRect, destRect);
    }
    void lockSprite()
    {
        lock = true;
    }
    void unlockSprite()
    {
        lock = false;
    }
};
#endif
