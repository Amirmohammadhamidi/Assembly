#ifndef GameObject_hpp
#define GameObject_hpp
#include "Game.hpp"

class GameObject
{
private:
    int xpos, ypos;
    int textureWidth, textureHeight;
    SDL_Texture *objectTexture;
    SDL_Rect srcRect, destRect;

public:
    GameObject(const char *textureSheet, int x, int y);
    ~GameObject();
    void Update();
    void Render();
};

#endif