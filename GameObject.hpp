#ifndef GameObject_hpp
#define GameObject_hpp
#include "Game.hpp"

class GameObject
{
private:
    int xpos, ypos;
    SDL_Texture *objectTexture;
    SDL_Rect srcRect, destRect;
    SDL_Renderer *renderer;

public:
    GameObject(const char *textureSheet, SDL_Renderer *ren, int x, int y);
    ~GameObject();
    void Update();
    void Render();
};

#endif