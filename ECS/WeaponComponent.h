#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Game.h"
// #include "../Timer.h"

#include <SDL.h>

extern Entity &player;

class WeaponComponent : public Component
{
private:
    TransformComponent transform;
public:
    Uint32 CD_NormalAttack = 1000;
    Uint32 curTime_NormalAttack = 0;
    Uint32 lastTime_NormalAttack = 0;
    
    int scale;
    DirectionComponent dir;
    SDL_Rect src;
    SDL_Rect dest;
    SDL_Point center;
    double angle;

    bool isAtacking = false;
    Uint32 attackCounter = 0;

    WeaponComponent(){};

    WeaponComponent(std::string id, int width, int height, int scl)
    {
        scale = scl;

        // tex = Game::assets->GetTexture(id);

        src = {0, 0, width, height};
        // range->collider.x = transform->position.x + 5;
        // range->collider.y = transform->position.y + 5;
    }

    ~WeaponComponent(){};

    void init() override
    {
    }

    void update() override
    {
        transform = player.getComponent<TransformComponent>();
        dir = player.getComponent<DirectionComponent>();
        angle = dir.angle;

        center = {0, dest.h / 2};
        dest = {(int)transform.position.x - Game::camera.x + transform.width * transform.scale / 2, (int)transform.position.y - Game::camera.y + transform.height * transform.scale / 3 * 2, src.w * scale, src.h * scale};

        if (dir.flip == SDL_FLIP_HORIZONTAL)
        {
            dest.x -= dest.w;
            dest.y -= dest.h;
            center = {dest.w, dest.h / 2};
        }

        if (dir.flip == SDL_FLIP_HORIZONTAL)
        {
            dest.y += dest.h;
        }

    }

    void draw() override
    {
        // TextureManager::DrawAngle(tex, src, dest, dir.flip, center, angle);
        // std::cout << "[WeaponComponent.h]: Weapon position: " << dest.x << " " << dest.y << std::endl;
    }
};