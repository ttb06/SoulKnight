#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

class ColliderComponent;

class Game
{
public:
    Game();
    ~Game();

    void init(const char *title, int xPos, int yPos, int width, int height, bool fullScreen);

    void handleEvents();
    void update();
    bool running() { return isRunning; }
    void render();
    void clean();

    static SDL_Renderer *renderer;
    static SDL_Event event;
    static bool isRunning;
    static SDL_Rect camera;

    enum groupLabels : std::size_t
    {
        groupMap,
        groupPlayers,
        groupColliders
    };

private:
    SDL_Window *window;
};