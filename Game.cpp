#include <iostream>
#include "Game.hpp"
#include "TextureManager.hpp"
#include "ECS/Components.hpp"
#include "vector2D.hpp"
using namespace std;

const float pixelSize = 0.27 * 0.001;
float deltaX, deltaY, averageVelocity;
double dist;

Manager manager;
SDL_Renderer *Game::renderer = nullptr; // initialize static renderer in this part
SDL_Event Game::event;

auto &pitch = manager.addEntity();
auto &target = manager.addEntity();
auto &ball = manager.addEntity();
auto &player = manager.addEntity();
auto &enemy = manager.addEntity();
auto &sinIcon = manager.addEntity();
auto &concaveIcon = manager.addEntity();
auto &lineIcon = manager.addEntity();

vector2D *acceleration = new vector2D();
vector2D *velocity = new vector2D();

const vector2D *pitchSize;
const vector2D *shooterPoint;
const vector2D *enemyPoint;
const vector2D *iconSize;

const vector2D *pitchXBound;
const vector2D *pitchYBound;
const vector2D *sinIconXBound;
const vector2D *sinIconYBound;
const vector2D *concaveIconXBound;
const vector2D *concaveIconYBound;
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

    enemyPoint = new vector2D(0.868 * pitchSize->x, 0.499 * pitchSize->y);
    target.addComponent<TransformComponent>(enemyPoint->x, enemyPoint->y);
    target.addComponent<MouseController>(pitchXBound, pitchYBound);
    target.addComponent<SpriteComponent>("assets/MainIcons/target.png", 0.2f, true);

    sinIconXBound = new vector2D(0.08 * pitchSize->x, 0.08 * pitchSize->x + iconSize->x);
    sinIconYBound = new vector2D(pitchSize->y - iconSize->y, pitchSize->y);
    sinIcon.addComponent<TransformComponent>(sinIconXBound->x, sinIconYBound->x);
    sinIcon.addComponent<MouseController>(sinIconXBound, sinIconYBound);
    sinIcon.addComponent<SpriteComponent>("assets/MainIcons/sin.png", (int)iconSize->x, (int)iconSize->y);

    concaveIconXBound = new vector2D(sinIconXBound->y, sinIconXBound->y + iconSize->x);
    concaveIconYBound = new vector2D(pitchSize->y - iconSize->y, pitchSize->y);
    concaveIcon.addComponent<TransformComponent>(concaveIconXBound->x, concaveIconYBound->x);
    concaveIcon.addComponent<MouseController>(concaveIconXBound, concaveIconYBound);
    concaveIcon.addComponent<SpriteComponent>("assets/MainIcons/concave.png", (int)iconSize->x, (int)iconSize->y);

    lineIconXBound = new vector2D(concaveIconXBound->y, concaveIconXBound->y + iconSize->x);
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
        deltaX = target.getComponent<TransformComponent>().position.x - shooterPoint->x;
        deltaY = target.getComponent<TransformComponent>().position.y - shooterPoint->y;
        averageVelocity = deltaY / deltaX;
        dist = sqrt((deltaX * deltaX) + (deltaY * deltaY)) * pixelSize;
        target.getComponent<MouseController>().deActivate();
    }

    if (lineIcon.getComponent<MouseController>().isActive())
    {
        acceleration->x = acceleration->y = 0.0f;
        velocity->x = 1.0f;
        velocity->y = averageVelocity;
        vector2D::normalizeVector(*velocity);
        ball.getComponent<TransformComponent>().velocity.x = velocity->x;
        ball.getComponent<TransformComponent>().velocity.y = velocity->y;
        lineIcon.getComponent<MouseController>().deActivate();
    }

    if (concaveIcon.getComponent<MouseController>().isActive())
    {
        acceleration->x = 0.0f;
        acceleration->y = 9.8f * pixelSize;

        float v0 = averageVelocity - 0.5 * acceleration->y * (target.getComponent<TransformComponent>().position.x + shooterPoint->x);

        velocity->x = 1.0f;
        velocity->y = acceleration->y * ball.getComponent<TransformComponent>().position.x + v0;
        vector2D::normalizeVector(*velocity);

        ball.getComponent<TransformComponent>().velocity.x = velocity->x;
        ball.getComponent<TransformComponent>().velocity.y = velocity->y;
    }

    if (sinIcon.getComponent<MouseController>().isActive())
    {
        double w = atan(averageVelocity);
        float x = (ball.getComponent<TransformComponent>().position.x - shooterPoint->x) * pixelSize;

        velocity->x = static_cast<float>(cos(w));
        // velocity->y = static_cast<float>(sin(w) - cos(w) * cos(x * 2 * M_PI * 5 / dist));
        velocity->y = static_cast<float>(sin(w) - cos(w) * cos(x * 20 * M_PI));
        vector2D::normalizeVector(*velocity);

        ball.getComponent<TransformComponent>().velocity.x = velocity->x;
        ball.getComponent<TransformComponent>().velocity.y = velocity->y;
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
