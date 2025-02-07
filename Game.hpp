#ifndef Game_hpp
#define Game_hpp
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Game
{

public:
    static SDL_Renderer *renderer;
    static SDL_Event event;
    Game()
    {
    }
    ~Game()
    {
    }

    void init(const char *title, int xpos, int ypos, int width, int height, bool fullScreen);
    void handleGameElements(int width, int height);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() { return isRunning; }
    void handleInputs();
    void setXvalues();
    void lineConverter(float a, float b);
    void concaveConverter(float a, float b, float c);
    void sinConverter(float w);

private:
    bool isRunning;
    int position = 0;
    SDL_Window *window;
};

#endif