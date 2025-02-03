#include "Game.hpp"
using namespace std;

SDL_Texture *playerTexture;
SDL_Rect srcR, destR;

Game::Game()
{
}
Game::~Game()
{
}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullScreen)
{
    int flags = 0;
    if (fullScreen)
        flags = SDL_WINDOW_FULLSCREEN;
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window)
        {
            cout << "window is created\n";
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            // set default color to renderer:
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            cout << "renderer is created\n";
        }

        isRunning = true;
    }
    else
    {
        isRunning = false;
    }

    SDL_Surface *tmp_surface = IMG_Load("assets/images.jpeg");
    playerTexture = SDL_CreateTextureFromSurface(renderer, tmp_surface);
    SDL_FreeSurface(tmp_surface);
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;

    default:
        break;
    }
}

void Game::update()
{
    destR.h = 60;
    destR.w = 60;
}

void Game::render()
{
    SDL_RenderClear(renderer);
    // add stufs to render:
    SDL_RenderCopy(renderer, playerTexture, NULL, &destR);
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
