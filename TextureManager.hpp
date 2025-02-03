#ifndef TextureManager_hpp
#define TextureManager_hpp
#include "Game.hpp"

class TextureManager
{
public:
    static SDL_Texture *loadTexture(const char *filename, SDL_Renderer *renderer);
};

#endif