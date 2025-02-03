#include "Game.hpp"
#include "TextureManager.hpp"
#include "GameObject.hpp"
#include <iostream>
#include "Map.hpp"
using namespace std;

GameObject *player;
GameObject *enemy;
Map *map;
SDL_Renderer *Game::renderer = nullptr; // initialize static renderer in this part

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
    player = new GameObject("assets/taylor.jpeg", 0, 0);
    enemy = new GameObject("assets/jennie.jpeg", 50, 50);
    map = new Map();
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
    player->Update();
    enemy->Update();
}

void Game::render()
{
    SDL_RenderClear(renderer);
    map->DrawMap();
    player->Render();
    enemy->Render();
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
