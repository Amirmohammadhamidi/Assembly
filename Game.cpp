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
auto &sinIcon = manager.addEntity();
auto &convexIcon = manager.addEntity();
auto &lineIcon = manager.addEntity();

const vector2D *pitchSize;
const vector2D *shooterPoint;
const vector2D *iconSize;

const vector2D *pitchXBound;
const vector2D *pitchYBound;
const vector2D *sinIconXBound;
const vector2D *sinIconYBound;
const vector2D *convexIconXBound;
const vector2D *convexIconYBound;
const vector2D *lineIconXBound;
const vector2D *lineIconYBound;

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
    handleGameElements(width, height);
}
void Game::handleGameElements(int width, int height)
{
    pitchSize = new vector2D(width, height);
    iconSize = new vector2D(0.06 * pitchSize->x, 0.092 * pitchSize->y);

    pitchXBound = new vector2D(0, width);
    pitchYBound = new vector2D(iconSize->x, pitchSize->y - iconSize->y);

    pitch.addComponent<TransformComponent>();
    pitch.addComponent<SpriteComponent>("assets/Environment/background.jpg", 1.0f);

    shooterPoint = new vector2D(0.121 * pitchSize->x, 0.492 * pitchSize->y);
    ball.addComponent<TransformComponent>(shooterPoint->x, shooterPoint->y);
    ball.addComponent<SpriteComponent>("assets/ball/ball.png", 0.4f);

    target.addComponent<TransformComponent>();
    target.addComponent<MouseController>(pitchXBound, pitchYBound);
    target.addComponent<SpriteComponent>("assets/MainIcons/target.png", 0.2f, true);

    sinIconXBound = new vector2D(0.08 * pitchSize->x, 0.08 * pitchSize->x + iconSize->x);
    sinIconYBound = new vector2D(pitchSize->y - iconSize->y, pitchSize->y);
    sinIcon.addComponent<TransformComponent>(sinIconXBound->x, sinIconYBound->x);
    sinIcon.addComponent<MouseController>(sinIconXBound, sinIconYBound);
    sinIcon.addComponent<SpriteComponent>("assets/MainIcons/sin.png", (int)iconSize->x, (int)iconSize->y);

    convexIconXBound = new vector2D(sinIconXBound->y, sinIconXBound->y + iconSize->x);
    convexIconYBound = new vector2D(pitchSize->y - iconSize->y, pitchSize->y);
    convexIcon.addComponent<TransformComponent>(convexIconXBound->x, convexIconYBound->x);
    convexIcon.addComponent<MouseController>(convexIconXBound, convexIconYBound);
    convexIcon.addComponent<SpriteComponent>("assets/MainIcons/convex.png", (int)iconSize->x, (int)iconSize->y);

    lineIconXBound = new vector2D(convexIconXBound->y, convexIconXBound->y + iconSize->x);
    lineIconYBound = new vector2D(pitchSize->y - iconSize->y, pitchSize->y);
    lineIcon.addComponent<TransformComponent>(lineIconXBound->x, lineIconYBound->x);
    lineIcon.addComponent<MouseController>(lineIconXBound, lineIconYBound);
    lineIcon.addComponent<SpriteComponent>("assets/MainIcons/line.png", (int)iconSize->x, (int)iconSize->y);
}

void Game::handleEvents()
{
    handleInputs();
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT)
    {
        isRunning = false;
        return;
    }
}

void Game::handleInputs()
{
    if (target.getComponent<MouseController>().isActive())
    {
        target.getComponent<TransformComponent>().position.x = target.getComponent<MouseController>().getClickedPosition()->x;
        target.getComponent<TransformComponent>().position.y = target.getComponent<MouseController>().getClickedPosition()->y;
        target.getComponent<MouseController>().deActivate();
    }

    if (lineIcon.getComponent<MouseController>().isActive())
    {
        float deltaY = target.getComponent<MouseController>().getClickedPosition()->y - ball.getComponent<TransformComponent>().position.y;
        float deltaX = target.getComponent<MouseController>().getClickedPosition()->x - ball.getComponent<TransformComponent>().position.x;
        ball.getComponent<TransformComponent>().velocity.x = (deltaX / 500);
        ball.getComponent<TransformComponent>().velocity.y = (deltaY / 500);
        lineIcon.getComponent<MouseController>().deActivate();
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
