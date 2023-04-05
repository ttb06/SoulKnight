#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Game
{
public:
    Game();
    ~Game();

    void init(const char* title, int xPos, int yPos, int width, int height, bool fullScreen);

    void handleEvents();
    void update();
    bool running() { return isRunning; }
    void render();
    void clean();

    static SDL_Renderer *renderer;
    static SDL_Event event;
    
private:
    SDL_Window *window;

    bool isRunning;
};