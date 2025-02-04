#ifndef Game_hpp
#define Game_hpp
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Game
{

public:
    static int width, height;
    static SDL_Renderer *renderer;
    static SDL_Event event;
    static SDL_Texture *background;
    static SDL_Rect src, dest;
    Game()
    {
    }

    ~Game()
    {
    }

    void init(const char *title, const char *path, int xpos, int ypos, int width, int height, bool fullScreen);
    void handleGameElements();
    void handleEvents();
    void update();
    void render();
    void clean();

    bool running()
    {
        return isRunning;
    }
    void setWindowSize()
    {
        SDL_GetWindowSize(window, &width, &height);
    }

private:
    bool isRunning;
    int position = 0;
    SDL_Window *window;
};

#endif