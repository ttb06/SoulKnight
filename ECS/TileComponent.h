#pragma once

#include <SDL.h>
#include "TransformComponent.h"
#include "ECS.h"

class TileComponent : public Component
{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;

    SDL_Rect tileRect;
    int tileID;
    char *path;
    TileComponent() = default;

    TileComponent(int x, int y, int w, int h, int id)
    {
        tileRect.x = x;
        tileRect.y = y;
        tileRect.w = w;
        tileRect.h = h;
        tileID = id;

        switch (id)
        {
        case 1:
            path = "assets/tileset_1.png";
            break;
        case 2:
            path = "assets/tileset_2.png";
            break;
        case 3:
            path = "assets/tileset_3.png";
        default:
            break;
        }
    }
    
    void init() override
    {
        entity->addComponent<TransformComponent>(tileRect.x, tileRect.y, tileRect.w, tileRect.h, 1);
        transform = &entity->getComponent<TransformComponent>();

        entity->addComponent<SpriteComponent>(path);
        sprite = &entity->getComponent<SpriteComponent>();
    }
};