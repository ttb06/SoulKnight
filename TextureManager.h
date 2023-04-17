#pragma once

#include "Game.h"

class TextureManager
{
public:
    static SDL_Texture *loadTexture(const char *path);
    static void Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
    static void drawRect(int xPos, int yPos, int width, int height, SDL_Color color);
    static void DrawAngle(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, SDL_Point center, double angle);
};