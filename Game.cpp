#include <iostream>
#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "ECS/Components.hpp"
#include "vector2D.hpp"
using namespace std;

Manager manager;
SDL_Renderer *Game::renderer = nullptr; // initialize static renderer in this part
SDL_Texture *Game::background;
SDL_Event Game::event;
int Game::width = 0;
int Game::height = 0;
SDL_Rect Game::src;
SDL_Rect Game::dest;

auto &ball = manager.addEntity();
auto &player = manager.addEntity();
auto &enemy = manager.addEntity();

void Game::init(const char *title, const char *path, int xpos, int ypos, int width, int height, bool fullScreen)
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
    Game::background = TextureManager::loadTexture(path);
    setWindowSize();
    Game::src = {0, 0, Game::width, Game::height};
    Game::dest = {0, 0, Game::width, Game::height};
    handleGameElements();
}
void Game::handleGameElements()
{
    ball.addComponent<TransformComponent>();
    ball.addComponent<SpriteComponent>("assets/ball/trail.png", 1.0f);
    ball.addComponent<KeyboardController>();
}

void Game::handleEvents()
{
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
    manager.refresh();
    manager.update();
}

void Game::render()
{
    SDL_RenderClear(renderer);
    TextureManager::Draw(Game::background, Game::src, Game::dest);
    manager.draw();
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
