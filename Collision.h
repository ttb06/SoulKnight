#pragma once

#include <SDL.h>
// #include "ECS/ColliderComponent.h"
class ColliderComponent;

class Collision
{
public:
    static bool AABB(const SDL_Rect &recA, const SDL_Rect &recB);
    static bool isFront(const SDL_Rect &recA, const SDL_Rect &recB);
};