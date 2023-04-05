#pragma once

#include "Game.h"

class TextureManager
{
public:
    static SDL_Texture *loadTexture(const char *path);
    static SDL_Texture *Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest);
};