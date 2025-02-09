#include <iostream>
#include "Game.hpp"
#include "TextureManager.hpp"
#include "ECS/Components.hpp"
#include "vector2D.hpp"
#include <chrono>
#include <random>
using namespace std;

extern "C" void fill_array(float *array, size_t size, float percision, float x0);
extern "C" void line_handler(size_t size, float *x_array, float *y_array, float a, float b);
extern "C" void concave_handler(size_t size, float *x_array, float *y_array, float a, float b, float c);
extern "C" void sin_handler(size_t size, float *x_array, float *y_array, float x0, float y0, float w_sin, float w_cos);

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
auto &sinIcon = manager.addEntity();
auto &concaveIcon = manager.addEntity();
auto &lineIcon = manager.addEntity();
auto &enemyIcon = manager.addEntity();
auto &timerIcon = manager.addEntity();

vector2D *acceleration = new vector2D();
vector2D *velocity = new vector2D();
vector2D *targetXBound;
vector2D *targetYBound;
vector2D *playerXBound;
vector2D *playerYBound;

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
const vector2D *enemyIconXBound;
const vector2D *enemyIconYBound;
const vector2D *timerIconXBound;
const vector2D *timerIconYBound;

const int m = 25;
const int n = 16 * m;
float percision;
float x_values[n];
float y_values[n];

int movementcounter = 0;

bool graphicalActivation = false;
bool enemyActivation = false;

float ballX, ballY;
float targetX, targetY;

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

    player.addComponent<TransformComponent>(shooterPoint->x - iconSize->x, shooterPoint->y - (iconSize->y * 0.5f));
    player.addComponent<KeyboardController>();
    player.addComponent<SpriteComponent>("assets/MainIcons/player.png", (int)iconSize->x, (int)iconSize->y);
    playerXBound = new vector2D(shooterPoint->x - iconSize->x, shooterPoint->x);
    playerYBound = new vector2D(shooterPoint->y - (0.5f * iconSize->y), shooterPoint->y + (0.5f * iconSize->y));

    targetXBound = new vector2D(enemyPoint->x, enemyPoint->x + target.getComponent<SpriteComponent>().getDestWidth());
    targetYBound = new vector2D(enemyPoint->y, enemyPoint->y + target.getComponent<SpriteComponent>().getDestHeight());

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

    enemyIconXBound = new vector2D(lineIconXBound->y, lineIconXBound->y + iconSize->x);
    enemyIconYBound = new vector2D(pitchSize->y - iconSize->y, pitchSize->y);
    enemyIcon.addComponent<TransformComponent>(enemyIconXBound->x, enemyIconYBound->x);
    enemyIcon.addComponent<MouseController>(enemyIconXBound, enemyIconYBound);
    enemyIcon.addComponent<SpriteComponent>("assets/MainIcons/enemyIcon.png", (int)iconSize->x, (int)iconSize->y);

    timerIconXBound = new vector2D(enemyIconXBound->y, enemyIconXBound->y + iconSize->x);
    timerIconYBound = new vector2D(pitchSize->y - iconSize->y, pitchSize->y);
    timerIcon.addComponent<TransformComponent>(timerIconXBound->x, timerIconYBound->x);
    timerIcon.addComponent<MouseController>(timerIconXBound, timerIconYBound);
    timerIcon.addComponent<SpriteComponent>("assets/MainIcons/time.png", (int)iconSize->x, (int)iconSize->y);

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
    if (enemyActivation)
    {
        if (timerIcon.getComponent<MouseController>().isActive())
        {
            ball.getComponent<TransformComponent>().speedUp();
            timerIcon.getComponent<MouseController>().deActivate();
        }
        ballX = ball.getComponent<TransformComponent>().position.x;
        ballY = ball.getComponent<TransformComponent>().position.y;
        targetX = target.getComponent<TransformComponent>().position.x;
        targetY = target.getComponent<TransformComponent>().position.y;

        target.getComponent<TransformComponent>().position.y = ballY + 0.01f * iconSize->y;

        playerXBound->x = player.getComponent<TransformComponent>().position.x;
        playerXBound->y = playerXBound->x + iconSize->x;
        playerYBound->x = player.getComponent<TransformComponent>().position.y;
        playerYBound->y = playerYBound->x + iconSize->y;

        targetYBound->x = target.getComponent<TransformComponent>().position.y;
        targetYBound->y = targetYBound->x + target.getComponent<SpriteComponent>().getDestHeight();
        targetXBound->x = target.getComponent<TransformComponent>().position.x;
        targetXBound->y = targetXBound->x + target.getComponent<SpriteComponent>().getDestWidth();

        if ((ballX >= playerXBound->x) && (ballX <= playerXBound->y) && (ballY >= playerYBound->x) && (ballY <= playerYBound->y))
        {
            vector2D *vec = generateRandomPath(ballX, ballY, enemyPoint->x, false);
            ball.getComponent<TransformComponent>().velocity.x = vec->x;
            ball.getComponent<TransformComponent>().velocity.y = vec->y;
        }

        if ((ballX >= targetXBound->x))
        {
            vector2D *vec = generateRandomPath(ballX, ballY, shooterPoint->x, true);
            ball.getComponent<TransformComponent>().velocity.x = vec->x;
            ball.getComponent<TransformComponent>().velocity.y = vec->y;
        }

        if (ballX <= 0.5 * shooterPoint->x)
        {
            enemyActivation = false;
            enemyIcon.getComponent<MouseController>().deActivate();

            target.getComponent<TransformComponent>().position.x = enemyPoint->x;
            target.getComponent<TransformComponent>().position.y = enemyPoint->y;
            target.getComponent<SpriteComponent>().generateSprite("assets/MainIcons/target.png", (int)iconSize->x, (int)iconSize->y);
            player.getComponent<TransformComponent>().position.x = shooterPoint->x - iconSize->x;
            player.getComponent<TransformComponent>().position.y = shooterPoint->y - (iconSize->y * 0.5f);
            ball.getComponent<TransformComponent>().position.x = shooterPoint->x;
            ball.getComponent<TransformComponent>().position.y = shooterPoint->y;
            ball.getComponent<TransformComponent>().velocity.x = 0.0f;
            ball.getComponent<TransformComponent>().velocity.y = 0.0f;
        }
    }
    else if (enemyIcon.getComponent<MouseController>().isActive())
    {
        target.getComponent<TransformComponent>().position.x = enemyPoint->x;
        target.getComponent<TransformComponent>().position.y = enemyPoint->y;
        target.getComponent<SpriteComponent>().generateSprite("assets/MainIcons/enemy.png", (int)iconSize->x, (int)iconSize->y);
        ball.getComponent<TransformComponent>().position.x = shooterPoint->x;
        ball.getComponent<TransformComponent>().position.y = shooterPoint->y;

        lineIcon.getComponent<MouseController>().deActivate();
        concaveIcon.getComponent<MouseController>().deActivate();
        sinIcon.getComponent<MouseController>().deActivate();
        graphicalActivation = false;
        enemyActivation = true;
        movementcounter = 0;
    }

    else if (target.getComponent<MouseController>().isActive())
    {
        target.getComponent<TransformComponent>().position.x = target.getComponent<MouseController>().getClickedPosition()->x;
        target.getComponent<TransformComponent>().position.y = target.getComponent<MouseController>().getClickedPosition()->y;
        target.getComponent<SpriteComponent>().update();

        targetXBound->x = target.getComponent<TransformComponent>().position.x;
        targetXBound->x = target.getComponent<TransformComponent>().position.x + target.getComponent<SpriteComponent>().getDestWidth();
        targetYBound->x = target.getComponent<TransformComponent>().position.y;
        targetYBound->x = target.getComponent<TransformComponent>().position.y + target.getComponent<SpriteComponent>().getDestHeight();

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
        target.lockEntity();
        lineIcon.getComponent<MouseController>().deActivate();
    }

    else if (concaveIcon.getComponent<MouseController>().isActive())
    {
        float a = 4.9f * pixelSize;
        float b = averageVelocity - (a * (shooterPoint->x + target.getComponent<TransformComponent>().position.x));
        float c = shooterPoint->y - (a * (shooterPoint->x * shooterPoint->x)) - (b * shooterPoint->x);

        concaveConverter(a, b, c);

        graphicalActivation = true;
        target.lockEntity();
        concaveIcon.getComponent<MouseController>().deActivate();
    }

    else if (sinIcon.getComponent<MouseController>().isActive())
    {
        double w = atan(averageVelocity);
        float x = (ball.getComponent<TransformComponent>().position.x - shooterPoint->x) * pixelSize;

        sinConverter(static_cast<float>(w));

        graphicalActivation = true;
        target.lockEntity();
        sinIcon.getComponent<MouseController>().deActivate();
    }

    if (graphicalActivation)
    {
        if (movementcounter == n)
        {
            ball.getComponent<TransformComponent>().position.x = shooterPoint->x;
            ball.getComponent<TransformComponent>().position.y = shooterPoint->y;
            movementcounter = 0;
            graphicalActivation = false;
            target.unlockEntity();
        }
        else
        {
            ball.getComponent<TransformComponent>().position.x = x_values[movementcounter];
            ball.getComponent<TransformComponent>().position.y = y_values[movementcounter];
            movementcounter++;
        }
    }
}

void Game::setXvalues()
{
    auto start_asm = std::chrono::high_resolution_clock::now();
    fill_array(x_values, n, percision, shooterPoint->x);
    auto end_asm = std::chrono::high_resolution_clock::now();
    duration_asm += (end_asm - start_asm);

    auto start_cpp = std::chrono::high_resolution_clock::now();
    x_values[0] = shooterPoint->x;
    for (int i = 1; i < n; i++)
    {
        x_values[i] = x_values[i - 1] + percision;
    }
    auto end_cpp = std::chrono::high_resolution_clock::now();
    duration_cpp += (end_cpp - start_cpp);
}

void Game::lineConverter(float a, float b)
{
    // asm duration checker :
    auto start_asm = std::chrono::high_resolution_clock::now();
    line_handler(n, x_values, y_values, a, b);
    auto end_asm = std::chrono::high_resolution_clock::now();
    duration_asm += (end_asm - start_asm);
    cout << "assembly setting duration time for straight line is : " << duration_asm.count() * 1000 << " miliseconds" << endl;

    // cpp duration checker :
    auto start_cpp = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++)
    {
        y_values[i] = a * x_values[i] + b;
    }
    auto end_cpp = std::chrono::high_resolution_clock::now();
    duration_cpp += (end_cpp - start_cpp);
    cout << "cpp setting duration time for straight line is : " << duration_cpp.count() * 1000 << " miliseconds" << endl;

    cout << "assembly usage makes code : " << (duration_cpp.count() / duration_asm.count()) << " times faster!" << endl;
}

void Game::concaveConverter(float a, float b, float c)
{
    // asm duration checker :
    auto start_asm = std::chrono::high_resolution_clock::now();
    concave_handler(n, x_values, y_values, a, b, c);
    auto end_asm = std::chrono::high_resolution_clock::now();
    duration_asm += (end_asm - start_asm);
    cout << "assembly setting duration time for concave is : " << duration_asm.count() * 1000 << " miliseconds" << endl;

    // cpp duration checker :
    auto start_cpp = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++)
    {
        y_values[i] = (a * (x_values[i] * x_values[i])) + (b * x_values[i]) + c;
    }
    auto end_cpp = std::chrono::high_resolution_clock::now();
    duration_cpp += (end_cpp - start_cpp);
    cout << "cpp setting duration time for concave is : " << duration_cpp.count() * 1000 << " miliseconds" << endl;
    cout << "assembly usage makes code : " << (duration_cpp.count() / duration_asm.count()) << " times faster!" << endl;
}

void Game::sinConverter(float w)
{
    int N = 50;

    // asm duration checker :
    // auto start_asm = std::chrono::high_resolution_clock::now();
    // sin_handler(n, x_values, y_values, shooterPoint->x, shooterPoint->y, static_cast<float>(sin(w)), static_cast<float>(w));
    // auto end_asm = std::chrono::high_resolution_clock::now();
    // duration_asm += (end_asm - start_asm);
    // cout << "assembly setting duration time for sin : " << duration_asm.count() * 1000 << " miliseconds" << endl;

    // cpp duration checker :
    auto start_cpp = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++)
    {
        float x = x_values[i] - shooterPoint->x;
        float x1 = x / cos(w);
        y_values[i] = x1 * sin(w) - cos(w) * sin(x1 / N) * N + (shooterPoint->y);
        cout << "y is : " << y_values[i] << endl;
    }

    auto end_cpp = std::chrono::high_resolution_clock::now();
    duration_cpp += (end_cpp - start_cpp);
    cout << "cpp setting duration time for sin : " << duration_cpp.count() * 1000 << " miliseconds" << endl;
    cout << "assembly usage makes code : " << (duration_cpp.count() / duration_asm.count()) << " times faster!" << endl;
}

vector2D *Game::generateRandomPath(float x0, float y0, float x1, bool flag)
{
    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator
    std::uniform_real_distribution<> dis(pitchYBound->x, pitchYBound->y);

    float randomY = dis(gen);
    float deltaY = randomY - y0;
    float deltaX = x1 - x0;
    float v = deltaY / deltaX;

    vector2D *vec = new vector2D(1.0f, v);
    vector2D::normalizeVector(*vec, flag);

    return vec;
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
