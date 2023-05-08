#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include "Constant.h"

using std::vector;

class ColliderComponent;
class AssetManager;
class Menu;

class Game
{
public:
    Game();
    ~Game();

    void init(const char *title, int xPos, int yPos, int width, int height, bool fullScreen);

    void handleEvents();
    void update();
    bool running() { return isRunning; }
    void loadNextLevel();
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
    static int numberOfRooms;
    static int curRoom;
    static vector<int> roomEnermies; 
    
    //bfs
    static int visit[maxN][maxN];
    
    enum groupLabels : std::size_t
    {
        groupMap,
        groupHigherMap,
        groupAnimMap,
        groupDoor,
        groupPlayers,
        groupColliders,
        groupDoorColliders,
        groupProjectiles,
        groupEnermies,
        groupRoom,
        groupWeapons
    };

private:
    SDL_Window *window;
};