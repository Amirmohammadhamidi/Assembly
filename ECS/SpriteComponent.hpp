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

public:
    SpriteComponent() = default;
    SpriteComponent(const char *path, float scale)
    {
        setTexture(path);
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        this->scale = scale;
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
        destRect.x = (int)transform->position.x;
        destRect.y = (int)transform->position.y;
    }

    void draw()
    {
        TextureManager::Draw(texture, srcRect, destRect);
    }
};
