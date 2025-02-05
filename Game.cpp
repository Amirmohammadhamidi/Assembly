#include <iostream>
#include "Game.hpp"
#include "TextureManager.hpp"
#include "ECS/Components.hpp"
#include "vector2D.hpp"
using namespace std;

Manager manager;
SDL_Renderer *Game::renderer = nullptr; // initialize static renderer in this part
SDL_Event Game::event;

auto &pitch = manager.addEntity();
auto &target = manager.addEntity();
auto &ball = manager.addEntity();
auto &player = manager.addEntity();
auto &enemy = manager.addEntity();

const vector2D *shooterPoint = new vector2D(180.0f, 478.0f);

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
    handleGameElements();
}
void Game::handleGameElements()
{
    pitch.addComponent<TransformComponent>();
    pitch.addComponent<SpriteComponent>("assets/Environment/background.jpg", 1.0f);

    ball.addComponent<TransformComponent>(shooterPoint->x, shooterPoint->y);
    ball.addComponent<SpriteComponent>("assets/ball/ball.png", 0.4f);
    ball.addComponent<KeyboardController>();

    target.addComponent<TransformComponent>();
    target.addComponent<MouseController>();
    target.addComponent<SpriteComponent>("assets/MainIcons/target.png", 0.2f, true);
}

void Game::handleEvents()
{
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
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
    manager.draw();
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
