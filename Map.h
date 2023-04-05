#pragma once

#include "Game.h"

class Map
{
public:
    Map();
    ~Map();

    void LoadMap(int arr[20][25]);
    void DrawMap();

private:
    SDL_Rect src, dest;
    SDL_Texture *water;
    SDL_Texture *floor;
    SDL_Texture *wall;

    int map[20][25];
};