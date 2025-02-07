#include <iostream>
#include "Game.hpp"
#include "TextureManager.hpp"
#include "ECS/Components.hpp"
#include "vector2D.hpp"
#include <chrono>
using namespace std;
extern "C" void fill_array(float *array, size_t size, float percision, float x0);
extern "C" void line_handler(size_t size, float *x_array, float *y_array, float a, float b);
extern "C" void concave_handler(size_t size, float *x_array, float *y_array, float a, float b, float c);
extern "C" void sin_handler(size_t size, float *x_array, float *y_array, float w, float x0, float y0);

const float pixelSize = 0.00027f;
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
vector2D *targetXBound;
vector2D *targetYBound;

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

const int m = 25;
const int n = 16 * m;
float percision;
float x_values[n];
float y_values[n];

int movementcounter = 0;

bool asmChecker;
bool graphicalActivation = false;

std::chrono::duration<double> duration_cpp(0.0);
std::chrono::duration<double> duration_asm(0.0);

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
            // cout << "window is created\n";
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            // set default color to renderer:
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            // cout << "renderer is created\n";
        }

        isRunning = true;
    }
    else
    {
        isRunning = false;
    }

    handleGameElements(width, height);

    int checkType;
    cout << "check type : 1:asm  2:cpp" << endl;
    cin >> checkType;
    if (checkType == 1)
        asmChecker = true;
    else
        asmChecker = false;
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

    deltaX = enemyPoint->x - shooterPoint->x;
    deltaY = enemyPoint->y - shooterPoint->y;
    averageVelocity = deltaY / deltaX;
    dist = sqrt((deltaX * deltaX) + (deltaY * deltaY)) * pixelSize;
    percision = deltaX / (n - 1);
    setXvalues();
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
        percision = deltaX / (n - 1);
        setXvalues();

        target.getComponent<MouseController>().deActivate();
    }

    else if (lineIcon.getComponent<MouseController>().isActive())
    {
        lineConverter(averageVelocity, shooterPoint->y - (averageVelocity * shooterPoint->x));
        graphicalActivation = true;
        lineIcon.getComponent<MouseController>().deActivate();
    }

    else if (concaveIcon.getComponent<MouseController>().isActive())
    {
        float a = 4.9f * pixelSize;
        float b = averageVelocity - (a * (shooterPoint->x + target.getComponent<TransformComponent>().position.x));
        float c = shooterPoint->y - (a * (shooterPoint->x * shooterPoint->x)) - (b * shooterPoint->x);

        concaveConverter(a, b, c);

        graphicalActivation = true;
        concaveIcon.getComponent<MouseController>().deActivate();
    }

    else if (sinIcon.getComponent<MouseController>().isActive())
    {
        double w = atan(averageVelocity);
        float x = (ball.getComponent<TransformComponent>().position.x - shooterPoint->x) * pixelSize;

        sinConverter(static_cast<float>(w));

        graphicalActivation = true;
        sinIcon.getComponent<MouseController>().deActivate();

        velocity->x = static_cast<float>(cos(w));
        // velocity->y = static_cast<float>(sin(w) - cos(w) * cos(x * 2 * M_PI * 5 / dist));
        velocity->y = static_cast<float>(sin(w) - cos(w) * cos(x * 20 * M_PI));
        vector2D::normalizeVector(*velocity);

        ball.getComponent<TransformComponent>().velocity.x = velocity->x;
        ball.getComponent<TransformComponent>().velocity.y = velocity->y;
    }

    if (graphicalActivation)
    {
        ball.getComponent<TransformComponent>().position.x = x_values[movementcounter];
        ball.getComponent<TransformComponent>().position.y = y_values[movementcounter];
        movementcounter++;
    }
}

void Game::setXvalues()
{
    if (asmChecker)
    {
        auto start_asm = std::chrono::high_resolution_clock::now();
        fill_array(x_values, n, percision, shooterPoint->x);
        auto end_asm = std::chrono::high_resolution_clock::now();
        duration_asm += (end_asm - start_asm);
    }
    else
    {
        auto start_cpp = std::chrono::high_resolution_clock::now();
        x_values[0] = shooterPoint->x;
        for (int i = 1; i < n; i++)
        {
            x_values[i] = x_values[i - 1] + percision;
        }
        auto end_cpp = std::chrono::high_resolution_clock::now();
        duration_cpp += (end_cpp - start_cpp);
    }
}

void Game::lineConverter(float a, float b)
{
    if (asmChecker)
    {
        auto start_asm = std::chrono::high_resolution_clock::now();
        line_handler(n, x_values, y_values, a, b);
        auto end_asm = std::chrono::high_resolution_clock::now();
        duration_asm += (end_asm - start_asm);
    }
    else
    {
        auto start_cpp = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++)
        {
            y_values[i] = a * x_values[i] + b;
        }
        auto end_cpp = std::chrono::high_resolution_clock::now();
        duration_cpp += (end_cpp - start_cpp);
    }
}

void Game::concaveConverter(float a, float b, float c)
{
    if (asmChecker)
    {
        auto start_asm = std::chrono::high_resolution_clock::now();
        concave_handler(n, x_values, y_values, a, b, c);
        auto end_asm = std::chrono::high_resolution_clock::now();
        duration_asm += (end_asm - start_asm);
    }
    else
    {
        auto start_cpp = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++)
        {
            y_values[i] = (a * (x_values[i] * x_values[i])) + (b * x_values[i]) + c;
        }
        auto end_cpp = std::chrono::high_resolution_clock::now();
        duration_cpp += (end_cpp - start_cpp);
    }
}

void Game::sinConverter(float w)
{
    if (asmChecker)
    {
        auto start_asm = std::chrono::high_resolution_clock::now();
        sin_handler(n, x_values, y_values, w, shooterPoint->x, shooterPoint->y);
        auto end_asm = std::chrono::high_resolution_clock::now();
        duration_asm += (end_asm - start_asm);
    }
    else
    {
        auto start_cpp = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++)
        {
        }
        auto end_cpp = std::chrono::high_resolution_clock::now();
        duration_cpp += (end_cpp - start_cpp);
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
