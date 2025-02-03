#ifndef Game_hpp
#define Game_hpp
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
class Game
{
private:
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;

public:
    Game();
    ~Game();

    void init(const char *title, int xpos, int ypos, int width, int height, bool fullScreen);
    void handleEvents();
    void update();
    void render();
    void clean();

    bool running()
    {
        return isRunning;
    }
};

#endif /* Game_hpp */