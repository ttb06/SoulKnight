#pragma once

#include <SDL2/SDL.h>
#include "TransformComponent.h"
#include "ECS.h"

class TileComponent : public Component
{
public:
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;
    SDL_Rect firstRect;
    Vector2D position;
    bool isAnim = false;

    int room = -1;
    bool isRoom = false;

    TileComponent() = default;

    ~TileComponent()
    {
        SDL_DestroyTexture(texture);
    }

    TileComponent(int srcX, int srcY, int xPos, int yPos, int tsize, int tscale, std::string id, bool anim = false, int r = -1)
    {
        texture = Game::assets->GetTexture(id);

        position.x = xPos;
        position.y = yPos;

        srcRect.x = srcX;
        srcRect.y = srcY;
        srcRect.w = srcRect.h = tsize;

        destRect.x = xPos;
        destRect.y = yPos;
        destRect.w = destRect.h = tsize * tscale;

        isAnim = anim;
        if (isAnim)
        {
            firstRect = srcRect;
        }
        room = r;
    }

    void update() override
    {
        destRect.x = position.x - Game::camera.x;
        destRect.y = position.y - Game::camera.y;
        if (isAnim)
        {
            srcRect.x = firstRect.x + 16 * (int)(SDL_GetTicks() / TILE_ANIM_SPEED % (TILE_ANIM_FRAMES));
        }
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
    }

    int getRoom()
    {
        return room;
    }
};