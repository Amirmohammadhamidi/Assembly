#include <iostream>
#include "GameObject.hpp"
#include "TextureManager.hpp"

GameObject::GameObject(const char *textureSheet, SDL_Renderer *ren, int x, int y)
{
    renderer = ren;
    objectTexture = TextureManager::loadTexture(textureSheet, ren);
    xpos = x;
    ypos = y;
}
void GameObject::Update()
{
    // mov diognal
    xpos++;
    ypos++;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.h = 64;
    destRect.w = 64;

    xpos++;
}
void GameObject::Render()
{
    SDL_RenderCopy(renderer, objectTexture, NULL, &destRect);
}