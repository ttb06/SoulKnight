#include "Collision.h"
#include "ECS/ColliderComponent.h"

bool Collision::AABB(const SDL_Rect &recA, const SDL_Rect &recB)
{
    if (recA.x + recA.w > recB.x &&
        recB.x + recB.w > recA.x &&
        recA.y + recA.h > recB.y &&
        recB.y + recB.h > recA.y)
    {
        return true;
    }
    return false;
}

bool Collision::isFront(const SDL_Rect &recA, const SDL_Rect &recB)
{
    if (recA.y + recA.h > recB.y + recB.h)
        return true;
    return false;
}

bool Collision::Circle(const int xPos, const int yPos, const Vector2D rPos, const SDL_Rect dest)
{
    int xDest = dest.x + dest.w / 2;
    int yDest = dest.y + dest.h / 2;
    double rDest = sqrt(dest.w * dest.w + dest.h * dest.h);
    Vector2D distance(xDest - xPos, yDest - yPos);
    if (distance.len() > rPos.len() + rDest)
    {
        return false;
    }
    double angle = acos((rPos.x * distance.x + rPos.y * distance.y) / (rPos.len() * distance.len()));
    if (abs(angle) > M_PI_2)
    {
        return false;
    }
    std::cout << "[collision] trung cmnr" << std::endl;
    return true;
}