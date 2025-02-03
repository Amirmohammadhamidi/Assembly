#include "TextureManager.hpp"
#include <iostream>

SDL_Texture *TextureManager ::loadTexture(const char *texture_path, SDL_Renderer *renderer)
{
    SDL_Surface *surface = IMG_Load(texture_path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}