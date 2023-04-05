#pragma once

#include "Game.h"

class GameObject
{
public:
    GameObject(const char *textureSheet, int _xPos, int _yPos);
    ~GameObject();

    void update();
    void render();

private:
    int xPos, yPos;
    SDL_Texture *objTexture;
    SDL_Rect srcRect, destRect;
};