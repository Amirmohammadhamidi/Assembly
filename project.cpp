#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdint>
#include "Game.hpp"

using namespace std;
Game *game = nullptr;

int main(int argc, char *argv[])
{
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    const int pitchWidth = 1500;
    const int pitchHeight = 970;
    uint32_t frameStart;
    int frameTime;

    game = new Game();

    game->init("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pitchWidth, pitchHeight, false);
    while (game->running())
    {
        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }
    game->clean();
    return 0;
}