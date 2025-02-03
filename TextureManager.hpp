#ifndef TextureManager_hpp
#define TextureManager_hpp
#include "Game.hpp"

class TextureManager
{
public:
    static SDL_Texture *loadTexture(const char *filename);
    static void Draw(SDL_Texture *texture, SDL_Rect src, SDL_Rect dest);
};

#endif