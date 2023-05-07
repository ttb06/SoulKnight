#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "Constant.h"

using std::vector;
// class LTimer;
class ColliderComponent;
class AssetManager;

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

    static int curLevel;
    static int collisionMap[105][105];
    static SDL_Renderer *renderer;
    static SDL_Event event;
    static bool isRunning;
    static SDL_Rect camera;
    static AssetManager *assets;
    static int total_scale;
    
    static vector<vector<int>> roomCoordinate;
    static vector<int> roomEnermies; 
    
    //bfs
    static int visit[maxN][maxN];
    
    enum groupLabels : std::size_t
    {
        groupMap,
        groupHigherMap,
        groupAnimMap,
        groupPlayers,
        groupColliders,
        groupProjectiles,
        groupEnermies,
        groupWeapons
    };

private:
    SDL_Window *window;
};