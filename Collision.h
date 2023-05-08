#pragma once

#include <SDL2/SDL.h>
#include "Vector2D.h"

// #include "ECS/ColliderComponent.h"
class ColliderComponent;

class Collision
{
public:
    static bool AABB(const SDL_Rect &recA, const SDL_Rect &recB);
    static bool isFront(const SDL_Rect &recA, const SDL_Rect &recB);
    static bool Circle(const int xPos, const int yPos,  Vector2D rPos, const SDL_Rect dest);
};