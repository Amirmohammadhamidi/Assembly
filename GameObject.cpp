#include <iostream>
#include "GameObject.hpp"
#include "TextureManager.hpp"

GameObject::GameObject(const char *textureSheet, int x, int y)
{
    objectTexture = TextureManager::loadTexture(textureSheet);
    SDL_QueryTexture(objectTexture, NULL, NULL, &textureWidth, &textureHeight);
    std::cout << textureWidth;
    xpos = x;
    ypos = y;
}
void GameObject::Update()
{
    // mov diognal
    xpos++;
    ypos++;

    srcRect.x = 0;             // Start at the top-left corner
    srcRect.y = 0;             // Start at the top-left corner
    srcRect.w = textureWidth;  // Width of the entire texture
    srcRect.h = textureHeight; // Height of the entire texture

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.h = 64;
    destRect.w = 64;

    xpos++;
}
void GameObject::Render()
{
    SDL_RenderCopy(Game::renderer, objectTexture, &srcRect, &destRect);
}