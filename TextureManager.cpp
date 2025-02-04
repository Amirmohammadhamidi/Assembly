#include "TextureManager.hpp"
#include <iostream>

SDL_Texture *TextureManager ::loadTexture(const char *texture_path)
{
    SDL_Surface *surface = IMG_Load(texture_path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}
void TextureManager::Draw(SDL_Texture *texture, SDL_Rect src, SDL_Rect dest)
{
    SDL_RenderCopy(Game::renderer, texture, &src, &dest);
}