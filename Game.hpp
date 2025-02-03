#ifndef Game_hpp
#define Game_hpp
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Game
{
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

private:
    bool isRunning;
    int position = 0;
    SDL_Window *window;
    SDL_Renderer *renderer;
};

#endif /* Game_hpp */